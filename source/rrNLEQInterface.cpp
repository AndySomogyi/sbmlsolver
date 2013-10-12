#ifdef USE_PCH
#include "rr_pch.h"
#endif
#pragma hdrstop
#include <math.h>
#include "rrException.h"
#include "rrNLEQInterface.h"
#include "rrExecutableModel.h"
#include "rrStringUtils.h"
#include "rrUtils.h"
#include "rrRoadRunner.h"
#include "nleq/nleq1.h"
#include "rrLogger.h"
#include "rrUtils.h"
#include <Poco/ThreadLocal.h>

using Poco::ThreadLocal;

namespace rr
{

static ThreadLocal<ExecutableModel*> threadModel;

// the NLEQ callback
static void ModelFunction(int* nx, double* y, double* fval, int* pErr);

string ErrorForStatus(const int& error);

NLEQInterface::NLEQInterface(ExecutableModel *_model) :
    SteadyStateSolver("NLEQ2", "NLEQ2 Steady State Solver"),
    IWK(0),
    LIWK(0),
    LWRK(0),
    RWK(0),
    XScal(0),
    ierr(0),
    iopt(0),
    model(0),
    nOpts(50),
    defaultMaxInterations(100),
    maxIterations(defaultMaxInterations),
    defaultTolerance(1.e-4),
    relativeTolerance(defaultTolerance)
{
    model = _model;

    if(model)
    {
        setup();
    }
}

NLEQInterface::~NLEQInterface()
{
    delete[] IWK;
    delete[] RWK;
    delete[] XScal;
    delete[] iopt;
}

void NLEQInterface::setup()
{
    n = model->getNumIndependentSpecies();

    // Allocate space, see NLEQ docs for details
    LWRK = (n + 2 + 15)*n + 61;
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
    iopt[31 - 1] = 4;

    // Initialise all array elements to 0.0
    IWK = new long[LIWK];
    for (int i = 0; i < LIWK; i++)
    {
        IWK[i] = 0;
    }
    IWK[31 - 1] = maxIterations; // Max iterations


    RWK = new double[LWRK];
    for (int i = 0; i < LWRK; i++)
    {
        RWK[i] = 0.0;
    }

    RWK[22 - 1] = 1E-16; // Minimal allowed damping factor
}

bool NLEQInterface::isAvailable()
{
    NLEQInterface *temp= new NLEQInterface(NULL);
    if(temp)
    {
        bool val = temp->getNumberOfModelEvaluations() ? true : false;
        delete temp;
        return val;
    }

    return false;
}

double NLEQInterface::solve(const vector<double>& yin)
{
    if (yin.size() == 0)
    {
        return 0;
    }

    // Set up a dummy Jacobian, actual Jacobian is computed
    // by NLEQ using finite differences
    //    double* Jacobian = new double[1];

    ierr = 0;
    IWK[31 - 1] = maxIterations; // Max iterations

    // Set up default scaling factors
    for (int i = 0; i < n; i++)
    {
        XScal[i] = 1.0;
    }

    for (int i = 0; i < nOpts; i++)
    {
        iopt[i] = 0;
    }

    iopt[31 - 1] = 3; // Set for Highly nonlinear problem

    // Initialise all array elements to 0.0
    for (int i = 0; i < LIWK; i++)
    {
        IWK[i] = 0;
    }

    IWK[31 - 1] = maxIterations; // Max iterations
    for (int i = 0; i < LWRK; i++)
    {
        RWK[i] = 0.0;
    }

    RWK[22 - 1] = 1E-20; // Minimal allowed damping factor

    // For some reason NLEQ modifies the tolerance value, use a local copy instead
    double tmpTol = relativeTolerance;

    // set up the thread local variables, only this thread
    // access them.
    if (*threadModel)
    {
        throw(Exception("thread local storage model is set, this should never occur here."));
    }

    *threadModel = model;

    vector<double> amounts(n);
    model->getFloatingSpeciesAmounts(amounts.size(), 0, &amounts[0]);

    NLEQ1(     &n,
            &ModelFunction,
            NULL,
            &amounts[0],
            XScal,
            &tmpTol,
               iopt,
            &ierr,
            &LIWK,
               IWK,
            &LWRK,
            RWK);

    // done, clear it.
    *threadModel = 0;

    if (ierr == 2) // retry
    {
        for (int i = 0; i < nOpts; i++)
        {
            iopt[i] = 0;
        }

        iopt[31 - 1] = 3; // Set for Highly nonlinear problem
        iopt[0] = 1; // Try again but tell NLEQ not to reinitialize
        tmpTol = relativeTolerance;

    }

    if(ierr > 0 )
    {
        string err = ErrorForStatus(ierr);
        Log(lError)<<"Error :"<<err;
        throw NLEQException(err);
    }

    return computeSumsOfSquares();
}

void ModelFunction(int* nx, double* y, double* fval, int* pErr)
{
    ExecutableModel* model = *threadModel;
    if (model == NULL)
    {
        return;
    }

    try
    {
        int n = model->getNumIndependentSpecies();
        model->setFloatingSpeciesAmounts(n, 0, y);

        // eval the model in its current state
        model->evalModel(0.0, 0, 0);

        model->getFloatingSpeciesAmountRates(n, 0, fval);

        pErr = 0;
    }
    catch (const Exception& ex)
    {
        throw(ex);    //catch at a higher level
    }
}

void NLEQInterface::setScalingFactors(const vector<double>& sx)
{
    for (int i = 0; i < n; i++)
    {
        XScal[i] = sx[i];
    }
}

int NLEQInterface::getNumberOfNewtonIterations()
{
    return IWK[0];
}

int NLEQInterface::getNumberOfCorrectorSteps()
{
    return IWK[2];
}

int NLEQInterface::getNumberOfModelEvaluations()
{
    return IWK[3];
}

int NLEQInterface::getNumberOfJacobianEvaluations()
{
    return IWK[4];
}

int NLEQInterface::getNumberOfModelEvaluationsForJacobian()
{
    return IWK[7];
}

string ErrorForStatus(const int& error)
{
    switch (error)
    {
    case 1:     return ("Jacobian matrix singular in NLEQ");
    case 2:     return ("Maximum iterations exceeded");
    case 3:     return ("Damping factor has became to small to continue");
    case 4:     return ("Warning: Superlinear or quadratic convergence slowed down near the solution");
    case 5:     return ("Warning: Error Tolerance reached but solution is suspect");
    case 10:    return ("Integer or real workspace too small in NLEQ");
    case 20:    return ("Bad input to size of model parameter");
    case 21:    return ("Nonpositive value for RTOL supplied to NLEQ");
    case 22:    return ("Negative scaling value via vector XSCAL supplied");
    case 30:    return ("One or more fields specified in IOPT are invalid (NLEQ)");
    case 80:    return ("Error signalled by linear solver routine N1FACT, in NLEQ");
    case 81:    return ("Error signalled by linear solver routine N1SOLV, in NLEQ");
    case 82:    return ("Possible negative concentrations in solution (NLEQ)");
    case 83:    return ("Error signalled by user routine JAC in NLEQ");
    default:    return (format("Unknown error in NLEQ, errCode = {0}", error));
    }
}

double NLEQInterface::computeSumsOfSquares()
{
    // Compute the sums of squares and return value to caller
    model->evalModel(0.0, 0, 0);
    double sum = 0;
    vector<double> rates(n);
    model->getFloatingSpeciesAmountRates(rates.size(), 0, &rates[0]);

    for (int i = 0; i < n; i++)
    {
        sum = sum + pow(rates[i], 2.0);
    }
    return sqrt(sum);
}

_xmlNode* NLEQInterface::createConfigNode()
{
    // maxIterationsParam("MaxIterations", maxIterations, "Maximum number of newton iterations"),
    // relativeToleranceParam("relativeTolerance", relativeTolerance, "Relative precision of solution components")
    // mCapability("SteadyState", funcName, description)
    //

    _xmlNode *caps = Configurable::createCapabilityNode("SteadyState", "NLEQ2", "NLEQ2 Steady State Solver");
    Configurable::addChild(caps, Configurable::createParameterNode("MaxIterations", "Maximum number of newton iterations", maxIterations));
    Configurable::addChild(caps, Configurable::createParameterNode("relativeTolerance", "Relative precision of solution components", relativeTolerance));

    return caps;
}

void NLEQInterface::loadConfig(const _xmlDoc* doc)
{
}

}    //end of namespace


