#pragma hdrstop
#include "rrNLEQ2Interface.h"
#include "rrExecutableModel.h"
#include "rrStringUtils.h"
#include "rrUtils.h"
#include "nleq2/nleq2.h"
#include "rrLogger.h"
#include "rrUtils.h"
#include "rrException.h"
#include "rrConfig.h"

#include <Poco/Mutex.h>
#include <assert.h>
#include <math.h>

namespace rr
{

// NLEQ is an ancient Fortran77 routine that assumes that there is only
// one program which has a hard coded function in it.
// So, there is no concept of a user suplied data block, have to store
// the model in this static location -- only a single thread at a time
// may use the nleq steady state.
static ExecutableModel* callbackModel = NULL;

// mutex to ensure only one thead
using Poco::Mutex;
static Mutex mutex;

// the NLEQ callback, we use same data types as f2c here.
static void ModelFunction(int* nx, double* y, double* fval, int* pErr);

static std::string ErrorForStatus(int error);

static bool isError(int e)
{
    return e != 4 && e != 5;
}

static bool isWarning(int e)
{
    //case 4:     return ("Warning: Superlinear or quadratic convergence slowed down near the solution");
    //case 5:     return ("Warning: Error Tolerance reached but solution is suspect");

    return e == 4 || e == 5;
}

NLEQ2Interface::NLEQ2Interface(ExecutableModel *_model) :
    nOpts(50),
    IWK(NULL),
    LIWK(0),
    LRWK(0),
    RWK(NULL),
    XScal(NULL),
    ierr(0),
    iopt(0),
    model(NULL),
    n(0),
    allowPreSim(false),
    preSimTolerance(0.0),
    preSimMaximumSteps(0),
    preSimTime(0.0),
    allowApprox(false),
    approxTolerance(0.0),
    approxMaximumSteps(0),
    approxTime(0.0),
    relativeTolerance(0.0),
    maxIterations(0),
    minDamping(0.0),
    broyden(0),
    linearity(0)
{
    model = _model;

    if(model)
    {
        setup();
    }
}

NLEQ2Interface::~NLEQ2Interface()
{
    delete[] IWK;
    delete[] RWK;
    delete[] XScal;
    delete[] iopt;
}

void NLEQ2Interface::setup()
{
    // size of state std::vector
    n = model->getStateVector(0);
    rrLog(Logger::LOG_DEBUG) << "NLEQ2Interface: size of state std::vector = " << n;

    // Allocate space, see NLEQ docs for details
    if (broyden == 1)
    {
        LRWK = (n + max(n,10) + 15)*n + 61;
    }
    else
    {
        LRWK = (n + 15)*n + 61;
    }
    LIWK = n + 52;

    XScal = new double[n];
    // Set up default scaling factors
    for (int i = 0; i < n; i++)
    {
        XScal[i] = 1.0;
    }

    iopt = new long[nOpts];
    for (int i = 0; i < nOpts; i++)
    {
        iopt[i] = 0;
    }

    // Set for Highly nonlinear problem
    iopt[31 - 1] = linearity;

    // Set for Broyden method
    iopt[32 - 1] = broyden;

    // Initialise all array elements to 0.0
    IWK = new long[LIWK];
    for (int i = 0; i < LIWK; i++)
    {
        IWK[i] = 0;
    }
    IWK[31 - 1] = maxIterations; // Max iterations

    RWK = new double[LRWK];
    for (int i = 0; i < LRWK; i++)
    {
        RWK[i] = 0.0;
    }
    RWK[22 - 1] = minDamping; // Minimal allowed damping factor
}

bool NLEQ2Interface::isAvailable()
{
    NLEQ2Interface *temp= new NLEQ2Interface(NULL);
    if(temp)
    {
        bool val = temp->getNumberOfModelEvaluations() ? true : false;
        delete temp;
        return val;
    }

    return false;
}

double NLEQ2Interface::solve()
{
    // lock so only one thread can be here.
    Mutex::ScopedLock lock(mutex);

    rrLog(Logger::LOG_DEBUG) << "NLEQ2Interface::solve";

    // Set up a dummy Jacobian, actual Jacobian is computed
    // by NLEQ2 using finite differences
    //    double* Jacobian = new double[1];

    ierr = 0;

    // Set up default scaling factors
    for (int i = 0; i < n; i++)
    {
        XScal[i] = 1.0;
    }

    for (int i = 0; i < nOpts; i++)
    {
        iopt[i] = 0;
    }

    iopt[31 - 1] = linearity; // Set for Highly nonlinear problem

    iopt[32 - 1] = broyden; // Set for Broyden method

    // Initialise all array elements to 0.0
    for (int i = 0; i < LIWK; i++)
    {
        IWK[i] = 0;
    }
    IWK[31 - 1] = maxIterations; // Max iterations

    for (int i = 0; i < LRWK; i++)
    {
        RWK[i] = 0.0;
    }
    RWK[22 - 1] = minDamping; // Minimal allowed damping factor

    // For some reason NLEQ2 modifies the tolerance value, use a local copy instead
    double tmpTol = relativeTolerance;

    // set up the thread local variables, only this thread
    // access them.
    if (callbackModel)
    {
        throw Exception("global callbackModel is set, this should never happen!");
    }

    // should be exception safe, just make sure

    try
    {
        callbackModel = model;
        std::vector<double> stateVector(n);
        model->getStateVector(&stateVector[0]);

        NLEQ2(  &n,
                &ModelFunction,
                NULL,
                &stateVector[0],
                XScal,
                &tmpTol,
                iopt,
                &ierr,
                &LIWK,
                IWK,
                &LRWK,
                RWK);

        // done, clear it.
        callbackModel = NULL;
    }
    catch(...)
    {
        // clear the global model and re-throw the exception.
        callbackModel = NULL;
        throw;
    }

    if(ierr > 0 )
    {
        if (isWarning(ierr)) {
            rrLog(Logger::LOG_WARNING) << ErrorForStatus(ierr);
        } else {
            std::string err = ErrorForStatus(ierr);
            rrLog(Logger::LOG_ERROR)<<"Error :"<<err;
            throw NLEQException(err);
        }
    }

    return computeSumsOfSquares();
}



/*     FCN(N,X,F,IFAIL) Ext    Function subroutine */
/*       N              Int    Number of std::vector components (input) */
/*       X(N)           Dble   Vector of unknowns (input) */
/*       F(N)           Dble   Vector of function values (output) */
/*       IFAIL          Int    FCN evaluation-failure indicator. (output) */
/*                             On input:  Has always value 0 (zero). */
/*                             On output: Indicates failure of FCN eval- */
/*                                uation, if having a value <= 2. */
/*                             If <0: NLEQ2 will be terminated with */
/*                                    error code = 82, and IFAIL stored */
/*                                    to IWK(23). */
/*                             If =1: A new trial Newton iterate will */
/*                                    computed, with the damping factor */
/*                                    reduced to it's half. */
/*                             If =2: A new trial Newton iterate will */
/*                                    computed, with the damping factor */
/*                                    reduced by a reduct. factor, which */
/*                                    must be output through F(1) by FCN, */
/*                                    and it's value must be >0 and < 1. */
/*                             Note, that if IFAIL = 1 or 2, additional */
/*                             conditions concerning the damping factor, */
/*                             e.g. the minimum damping factor or the */
/*                             bounded damping strategy may also influ- */
/*                             ence the value of the reduced damping */
/*                             factor. */

void ModelFunction(int* nx, double* y, double* fval, int* pErr)
{
    ExecutableModel* model = callbackModel;
    assert(model && "model is NULL");

    assert(*nx == model->getStateVector(0) && "incorrect state std::vector size");

    // sets the state std::vector
    model->setStateVector(y);

    model->getStateVectorRate(0, y, fval);

    if (rr::Logger::getLevel() >= Logger::LOG_DEBUG)
    {
        std::stringstream ss;
        
        ss << "NLEQ2 ModelFunction" << std::endl;

        ss << "y: [";
        for (int i = 0; i < *nx; ++i)
        {
            ss << y[i];
            if (i + 1 < *nx)
            {
                ss << ", ";
            }
        }
        ss << "]" << std::endl;

        ss << "dydt: [";
        for (int i = 0; i < *nx; ++i)
        {
            ss << fval[i];
            if (i + 1 < *nx)
            {
                ss << ", ";
            }
        }
        ss << "]" << std::endl;

        rrLog(Logger::LOG_DEBUG) << ss.str();
    }

    *pErr = 0;

    for (int i = 0; i < *nx; ++i)
    {
        if (fval[i] != fval[i])
        {
            *pErr = -1;
        }
    }
}

void NLEQ2Interface::setScalingFactors(const std::vector<double>& sx)
{
    for (int i = 0; i < n; i++)
    {
        XScal[i] = sx[i];
    }
}

int NLEQ2Interface::getNumberOfNewtonIterations()
{
    return IWK[0];
}

int NLEQ2Interface::getNumberOfCorrectorSteps()
{
    return IWK[2];
}

int NLEQ2Interface::getNumberOfModelEvaluations()
{
    return IWK[3];
}

int NLEQ2Interface::getNumberOfJacobianEvaluations()
{
    return IWK[4];
}

int NLEQ2Interface::getNumberOfModelEvaluationsForJacobian()
{
    return IWK[7];
}

std::string ErrorForStatus(int error)
{
    switch (error)
    {
    case 1:     return ("Jacobian matrix singular in NLEQ. Failed to converge to steady state. Check if Jacobian matrix is non-invertible or steady state solution does not exist.");
    case 2:     return ("Maximum iterations exceeded");
    case 3:     return ("Damping factor has became to small to continue");
    case 4:     return ("Warning: Superlinear or quadratic convergence slowed down near the solution");
    case 5:     return ("Warning: Error Tolerance reached but solution is suspect");
    case 10:    return ("Integer or real workspace too small in NLEQ");
    case 20:    return ("Bad input to size of model parameter");
    case 21:    return ("Nonpositive value for RTOL supplied to NLEQ");
    case 22:    return ("Negative scaling value via std::vector XSCAL supplied");
    case 30:    return ("One or more fields specified in IOPT are invalid (NLEQ)");
    case 80:    return ("Error signalled by linear solver routine N1FACT, in NLEQ");
    case 81:    return ("Error signalled by linear solver routine N1SOLV, in NLEQ");
    case 82:    return ("Possible negative concentrations in solution (NLEQ)");
    case 83:    return ("Error signalled by user routine JAC in NLEQ");
    default:    return (format("Unknown error in NLEQ, errCode = {0}", error));
    }
}


// steady state solver options
static const char* keys[] =
{
        "allowPreSim"
        "preSimTolerance"
        "preSimMaximumSteps"
        "preSimTime"
        "allowApprox"
        "approxTolerance"
        "approxMaximumSteps"
        "approxTime"
        "relativeTolerance"
        "maxIterations"
        "minDamping"
        "broyden"
        "linearity"

        "allowPreSim.description"
        "preSimTolerance.description"
        "preSimMaximumSteps.description"
        "preSimTime.description"
        "allowApprox.description"
        "approxTolerance.description"
        "approxMaximumSteps.description"
        "approxTime.description"
        "relativeTolerance.description"
        "maxIterations.description"
        "minDamping.description"
        "broyden.description"
        "linearity.description"

        "allowPreSim.hint"
        "preSimTolerance.hint"
        "preSimMaximumSteps.hint"
        "preSimTime.hint"
        "allowApprox.hint"
        "approxTolerance.hint"
        "approxMaximumSteps.hint"
        "approxTime.hint"
        "relativeTolerance.hint"
        "maxIterations.hint"
        "minDamping.hint"
        "broyden.hint"
        "linearity.hint"
};

void NLEQ2Interface::setItem(const std::string& key, const rr::Variant& value)
{
}

Variant NLEQ2Interface::getItem(const std::string& key) const
{
    return 0;
}

bool NLEQ2Interface::hasKey(const std::string& key) const
{
    return false;
}

size_t NLEQ2Interface::deleteItem(const std::string& key)
{
    return 0;
}

std::vector<std::string> NLEQ2Interface::getKeys() const
{
    return std::vector<std::string>(&keys[0], &keys[sizeof(keys)/sizeof(char*)]);
}

const Dictionary* NLEQ2Interface::getSteadyStateOptions()
{
    static BasicDictionary dict;

    dict.setItem("steadyState", "NLEQ2");
    dict.setItem("steadyState.hint", "NLEQ2 hint");
    dict.setItem("steadyState.description", "NLEQ2 description");

    dict.setItem("allowPreSim", Config::getBool(Config::STEADYSTATE_PRESIMULATION));
    dict.setItem("preSimMaximumSteps", Config::getInt(Config::STEADYSTATE_PRESIMULATION_MAX_STEPS));
    dict.setItem("preSimTime", Config::getDouble(Config::STEADYSTATE_PRESIMULATION_TIME));
    dict.setItem("allowApprox", Config::getBool(Config::STEADYSTATE_APPROX));
    dict.setItem("approxTolerance", Config::getDouble(Config::STEADYSTATE_APPROX_TOL));
    dict.setItem("approxMaximumSteps", Config::getInt(Config::STEADYSTATE_APPROX_MAX_STEPS));
    dict.setItem("approxTime", Config::getDouble(Config::STEADYSTATE_APPROX_TIME));
    dict.setItem("relativeTolerance", Config::getDouble(Config::STEADYSTATE_RELATIVE));
    dict.setItem("maxIterations", Config::getInt(Config::STEADYSTATE_MAXIMUM_NUM_STEPS));
    dict.setItem("minDamping", Config::getDouble(Config::STEADYSTATE_MINIMUM_DAMPING));
    dict.setItem("broyden", Config::getInt(Config::STEADYSTATE_BROYDEN));
    dict.setItem("linearity", Config::getInt(Config::STEADYSTATE_LINEARITY));

    dict.setItem("allowPreSim.description", "startWithApprox.description");
    dict.setItem("preSimTolerance.description", "preSimTolerance.description");
    dict.setItem("preSimMaximumSteps.description", "preSimMaximumSteps.description");
    dict.setItem("preSimTime.description", "preSimTime.description");
    dict.setItem("allowApprox.description", "allowApprox.description");
    dict.setItem("approxTolerance.description", "approxTolerance.description");
    dict.setItem("approxMaximumSteps.description", "approxMaximumSteps.description");
    dict.setItem("approxTime.description", "approxTime.description");
    dict.setItem("relativeTolerance.description", "relativeTolerance.description");
    dict.setItem("maxIterations.description", "maxIterations.description");
    dict.setItem("minDamping.description", "minDamping.description");
    dict.setItem("broyden.description", "broyden.description");
    dict.setItem("linearity.description", "linearity.description");

    dict.setItem("allowPreSim.hint", "startWithApprox.hint");
    dict.setItem("preSimTolerance.hint", "preSimTolerance.hint");
    dict.setItem("preSimMaximumSteps.hint", "preSimMaximumSteps.hint");
    dict.setItem("preSimTime.hint", "preSimTime.hint");
    dict.setItem("allowApprox.hint", "allowApprox.hint");
    dict.setItem("approxTolerance.hint", "approxTolerance.hint");
    dict.setItem("approxMaximumSteps.hint", "approxMaximumSteps.hint");
    dict.setItem("approxTime.hint", "approxTime.hint");
    dict.setItem("relativeTolerance.hint", "relativeTolerance.hint");
    dict.setItem("maxIterations.hint", "maxIterations.hint");
    dict.setItem("minDamping.hint", "minDamping.hint");
    dict.setItem("broyden.hint", "broyden.hint");
    dict.setItem("linearity.hint", "linearity.hint");

    return &dict;
}

double NLEQ2Interface::computeSumsOfSquares()
{
    double sum = 0;
    std::vector<double> rates(model->getStateVector(0));
    model->getStateVectorRate(0, 0, &rates[0]);

    for (int i = 0; i < n; i++)
    {
        sum = sum + pow(rates[i], 2.0);
    }
    return sqrt(sum);
}



}    //end of namespace


