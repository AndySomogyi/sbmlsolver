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
//---------------------------------------------------------------------------

namespace rr
{
// private (file scope only) variables.
// thread local pointer.
#if defined(__unix__) || defined(__APPLE__)
static __thread ExecutableModel *gModel = 0;
#else
static __declspec(thread) ExecutableModel *gModel = 0;
#endif

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
    relativeTolerance(defaultTolerance),
    maxIterationsParam("MaxIterations", maxIterations, "Maximum number of newton iterations"),
    relativeToleranceParam("relativeTolerance", relativeTolerance, "Relative precision of solution components")
{
    model = _model;

    mCapability.addParameter(&maxIterationsParam);
    mCapability.addParameter(&relativeToleranceParam);

    if(model)
    {
        setup();
    }
}

NLEQInterface::~NLEQInterface()
{
    delete IWK;
    delete RWK;
    delete XScal;
    delete iopt;
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

Capability&    NLEQInterface::getCapability()
{
    return mCapability;
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
    if (gModel)
    {
        throw(Exception("gModel is set, this should never occur here."));
    }

    gModel = model;

    NLEQ1(     &n,
            &ModelFunction,
            NULL,
               model->getModelData().floatingSpeciesAmounts,
            XScal,
            &tmpTol,
               iopt,
            &ierr,
            &LIWK,
               IWK,
            &LWRK,
            RWK);

    // done, clear it.
    gModel = 0;

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
    ExecutableModel* model = gModel;
    if (model == NULL)
    {
        return;
    }

    try
    {
        long n = model->getNumIndependentSpecies();
        for(long i = 0; i < n; i++)
        {
            model->getModelData().floatingSpeciesAmounts[i] = y[i];
        }

        int size = model->getModelData().numFloatingSpecies + model->getModelData().numRateRules;
        vector<double> dTemp;
        dTemp.resize(size);

        for(int i = 0; i < model->getModelData().numRateRules; i++)
        {
            dTemp[i] = model->getModelData().rateRules[i];
        }

        for(int i = 0; i < model->getModelData().numFloatingSpecies; i++)
        {
            dTemp[model->getModelData().numRateRules + i] = model->getModelData().floatingSpeciesAmounts[i];
        }

        model->evalModel(0.0, &dTemp[0]);

        for(int i = 0; i < n; i++)
        {
            fval[i] = model->getModelData().floatingSpeciesConcentrationRates[i];
        }

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
    vector<double> dTemp;// = new double[model->getModelData().amounts.Length + model->getModelData().rateRules.Length];
    //    dTemp.resize(model->getModelData().amounts.size() + model->getModelData().rateRules.size());

    //    dTemp = model->getModelData().rateRules;//model->getModelData().rateRules.CopyTo(dTemp, 0);
    copyCArrayToStdVector(model->getModelData().rateRules,   dTemp, (model->getModelData().numRateRules));//model->mData.rateRules.CopyTo(dTemp, 0);
    //model->getModelData().amounts.CopyTo(dTemp, model->getModelData().rateRules.Length);
    //    for(int i = 0; i < model->getModelData().amounts.size(); i++)
    for(int i = 0; i < model->getNumIndependentSpecies(); i++)
    {
        dTemp.push_back(model->getModelData().floatingSpeciesAmounts[i]);
    }

    model->evalModel(0.0, &dTemp[0]);
    double sum = 0;
    for (int i = 0; i < n; i++)
    {
        sum = sum + pow(model->getModelData().floatingSpeciesConcentrationRates[i], 2.0);
    }
    return sqrt(sum);
}

}//end of namespace
