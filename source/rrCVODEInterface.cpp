#pragma hdrstop
#include "rrCVODEInterface.h"
#include "rrExecutableModel.h"
#include "rrException.h"
#include "rrLogger.h"
#include "rrStringUtils.h"
#include "rrException.h"
#include "rrUtils.h"

#include <cvode/cvode.h>
#include <cvode/cvode_dense.h>
#include <nvector/nvector_serial.h>
#include <cstring>
#include <iomanip>
#include <math.h>
#include <map>
#include <algorithm>
#include <assert.h>
#include <Poco/Logger.h>


using namespace std;
namespace rr
{

int InternalFunctionCall(realtype t, N_Vector cv_y, N_Vector cv_ydot, void *f_data);
int InternalRootCall (realtype t, N_Vector y, realtype *gout, void *g_data);

void ModelFcn(int n,     double time, double* y, double* ydot, void* userData);
void EventFcn(            double time, double* y, double* gdot, void* userData);

// N_Vector is a point to an N_Vector structure
RR_DECLSPEC void        SetVector (N_Vector v, int Index, double Value);
RR_DECLSPEC double      GetVector (N_Vector v, int Index);

const int CvodeInterface::mDefaultMaxNumSteps = 10000;
const int CvodeInterface::mDefaultMaxAdamsOrder = 12;
const int CvodeInterface::mDefaultMaxBDFOrder = 5;

/**
 * Purpose
 * This function processes error and warning messages from CVODE and its
 * sub-modules.
 *
 * Arguments:
 * error_code: is the error code.
 * module: is the name of the CVODE module reporting the error.
 * function: is the name of the function in which the error occurred.
 * msg: is the error message.
 * eh_data: is a pointer to user data, the same as the eh_data parameter
 * passed to CVodeSetErrHandlerFn.
 * Return value: A CVErrHandlerFn function has no return value.
 *
 * Notes:
 * error_code is negative for errors and positive (CV_WARNING) for warnings.
 * If a function that returns a pointer to memory encounters an error,
 * it sets error_code to 0.
 */
static void cvodeErrHandler(int error_code, const char *module, const char *function,
        char *msg, void *eh_data);


/**
 * decode the cvode error code to a string
 */
static std::string cvodeDecodeError(int cvodeError, bool exInfo = true);

/**
 * macro to throw a (hopefully) usefull error message
 */
#define handleCVODEError(errCode) \
        { std::string _err_what = std::string("CVODE Error: ") + \
          cvodeDecodeError(errCode); \
          throw IntegratorException(_err_what, std::string(__FUNC__)); }

void CvodeInterface::setListener(IntegratorListenerPtr p)
{
    listener = p;
}

IntegratorListenerPtr CvodeInterface::getListener()
{
    return listener;
}

void* CvodeInterface::createCvode(const SimulateOptions* options)
{
    void* result = 0;

    if (options && (options->integratorFlags & SimulateOptions::STIFF))
    {
        Log(Logger::LOG_INFORMATION) << "using stiff integrator";
        result = (void*) CVodeCreate(CV_BDF, CV_NEWTON);
    }
    else
    {
        Log(Logger::LOG_INFORMATION) << "using non-stiff integrator";
        result = (void*) CVodeCreate(CV_ADAMS, CV_FUNCTIONAL);
    }

    assert(result && "could not create Cvode, CVodeCreate failed");

    CVodeSetErrHandlerFn(result, cvodeErrHandler, NULL);

    //SetMaxOrder(mCVODE_Memory, MaxBDFOrder);

    //CVodeSetMaxOrd(mCVODE_Memory, mMaxBDFOrder);
    //CVodeSetInitStep(mCVODE_Memory, mInitStep);
    //CVodeSetMinStep(mCVODE_Memory, mMinStep);
    //CVodeSetMaxStep(mCVODE_Memory, mMaxStep);
    CVodeSetMaxNumSteps(result, mDefaultMaxNumSteps);

    return result;
}



CvodeInterface::CvodeInterface(ExecutableModel *aModel, const SimulateOptions* options)
:
mStateVector(NULL),
mAbstolArray(NULL),
mCVODE_Memory(NULL),
mLastTimeValue(0),
mLastEvent(0),
mOneStepCount(0),
mFollowEvents(true),
mMaxAdamsOrder(mDefaultMaxAdamsOrder),
mMaxBDFOrder(mDefaultMaxBDFOrder),
options(*options)
{
    if(aModel)
    {
        initializeCVODEInterface(aModel);
    }

    // if we pass a 0, we re-load all the values from our options.
    // saves from copying it twice.
    setSimulateOptions(0);
}

CvodeInterface::~CvodeInterface()
{
    // cvode does not check for null values.
    if(mCVODE_Memory)
    {
        CVodeFree( &mCVODE_Memory);
    }

    if(mStateVector)
    {
        N_VDestroy_Serial(mStateVector);
    }

    if(mAbstolArray)
    {
        N_VDestroy_Serial(mAbstolArray);
    }
}

void CvodeInterface::setSimulateOptions(const SimulateOptions* o)
{
    if(o)
    {
        options = *o;
    }

    if (options.initialTimeStep > 0)
    {
        CVodeSetInitStep(mCVODE_Memory, options.initialTimeStep);
    }

    if (options.minimumTimeStep > 0)
    {
        CVodeSetMinStep(mCVODE_Memory, options.minimumTimeStep);
    }

    if (options.maximumTimeStep > 0)
    {
        CVodeSetMaxStep(mCVODE_Memory, options.maximumTimeStep);
    }

    if (options.maximumNumSteps > 0)
    {
        CVodeSetMaxNumSteps(mCVODE_Memory, options.maximumNumSteps);
    }

    // if mAbstolArray, also have mStateVector
    if (mAbstolArray)
    {
        assert(mStateVector && "mStateVector shold not be NULL");

        for (unsigned i = 0; i < NV_LENGTH_S(mAbstolArray); ++i)
        {
            NV_DATA_S(mAbstolArray)[i] = options.absolute;
        }

        updateAbsTolVector();
    }
}

int CvodeInterface::allocateCvodeMem()
{
    if (mCVODE_Memory == NULL)
    {
        return CV_SUCCESS;
    }

    double t0 = 0.0;
    if(CVodeSetUserData(mCVODE_Memory, (void*) this) != CV_SUCCESS)
    {
        Log(Logger::LOG_ERROR)<<"Problem in setting CVODE User data";
    }

    int result =  CVodeInit(mCVODE_Memory, InternalFunctionCall, t0, mStateVector);

    if (result != CV_SUCCESS)
    {
        return result;
    }
    return CVodeSVtolerances(mCVODE_Memory, options.relative, mAbstolArray);
}

int CvodeInterface::rootInit(int numRoots)
{
    if (mCVODE_Memory == NULL)
    {
         return CV_SUCCESS;
    }

    return CVodeRootInit (mCVODE_Memory, numRoots, InternalRootCall);
}

// Initialize cvode with a new set of initial conditions
int CvodeInterface::reInit(double t0)
{
    if (mCVODE_Memory == NULL)
    {
        return CV_SUCCESS;
    }

    int result = CVodeReInit(mCVODE_Memory,  t0, mStateVector);

    if (result != CV_SUCCESS)
    {
        return result;
    }

    return CVodeSVtolerances(mCVODE_Memory, options.relative, mAbstolArray);
}

double CvodeInterface::integrate(double timeStart, double hstep)
{
    Log(lDebug3)<<"---------------------------------------------------";
    Log(lDebug3)<<"--- O N E     S T E P      ( "<<mOneStepCount<< " ) ";
    Log(lDebug3)<<"---------------------------------------------------";

    mOneStepCount++;
    mCount = 0;

    double timeEnd = 0.0;
    double tout = timeStart + hstep;
    int strikes = 3;

    const int itask = options.integratorFlags & SimulateOptions::MULTI_STEP
            ? CV_ONE_STEP : CV_NORMAL;

    // get the original event status
    vector<unsigned char> eventStatus(mModel->getEventTriggers(0, 0, 0), false);


    // here we stop for a too small timestep ... this seems troublesome to me ...
    while (tout - timeEnd > 1E-16)
    {
        if (hstep < 1E-16)
        {
            return tout;
        }

        // here we bail in case we have no ODEs set up with CVODE ... though we should
        // still at least evaluate the model function
        if (!haveVariables() && mModel->getNumEvents() == 0)
        {
            mModel->convertToAmounts();
            mModel->getStateVectorRate(tout, 0, 0);
            return tout;
        }

        if (mLastTimeValue > timeStart)
        {
            reStart(timeStart);
        }

        double nextTargetEndTime = tout;

        if (mModel->getPendingEventSize() > 0 &&
                mModel->getNextPendingEventTime(false) < nextTargetEndTime)
        {
            nextTargetEndTime = mModel->getNextPendingEventTime(true);
        }

        // event status before time step
        mModel->getEventTriggers(eventStatus.size(), 0, &eventStatus[0]);

        // time step
        int nResult = CVode(mCVODE_Memory, nextTargetEndTime,  mStateVector, &timeEnd, itask);

        if (nResult == CV_ROOT_RETURN && mFollowEvents)
        {
            Log(Logger::LOG_DEBUG) << ("---------------------------------------------------");
            Log(Logger::LOG_DEBUG) << "--- E V E N T   ( " << mOneStepCount << ", time: " << timeEnd << " ) ";
            Log(Logger::LOG_DEBUG) << ("---------------------------------------------------");


            bool tooCloseToStart = fabs(timeEnd - mLastEvent) > options.relative;

            if(tooCloseToStart)
            {
                strikes =  3;
            }
            else
            {
                strikes--;
            }

            if (tooCloseToStart || strikes > 0)
            {
                // evaluate events
                handleRootsForTime(timeEnd, eventStatus);
                reStart(timeEnd);
                mLastEvent = timeEnd;

                if (listener)
                {
                    listener->onEvent(this, mModel, timeEnd);
                }
            }
        }
        else if (nResult == CV_SUCCESS || !mFollowEvents)
        {
            mModel->setTime(timeEnd);
            assignResultsToModel();

            if (listener)
            {
                listener->onTimeStep(this, mModel, timeEnd);
            }
        }
        else
        {
            handleCVODEError(nResult);
        }

        mLastTimeValue = timeEnd;

        try
        {
            mModel->testConstraints();
        }
        catch (const Exception& e)
        {
            Log(lWarning)<<"Constraint Violated at time = " + toString(timeEnd)<<": " + e.Message();
        }

        assignPendingEvents(timeEnd, tout);

        if (tout - timeEnd > 1E-16)
        {
            timeStart = timeEnd;
        }
        Log(lDebug3)<<"tout: "<<tout<<gTab<<"timeEnd: "<<timeEnd;
    }
    return timeEnd;

}

bool CvodeInterface::haveVariables()
{
    return (mStateVectorSize > 0) ? true : false;
}

void CvodeInterface::initializeCVODEInterface(ExecutableModel *oModel)
{
    if(!oModel)
    {
        throw CVODEException("Fatal Error while initializing CVODE");
    }

    try
    {
        mModel = oModel;
        mStateVectorSize = oModel->getStateVector(0);

        if (mStateVectorSize > 0)
        {
            mStateVector = N_VNew_Serial(mStateVectorSize);
            mAbstolArray = N_VNew_Serial(mStateVectorSize);
            for (int i = 0; i < mStateVectorSize; i++)
            {
                SetVector((N_Vector) mAbstolArray, i, options.absolute);
            }

            updateAbsTolVector();

            mCVODE_Memory = createCvode(&options);

            int errCode = allocateCvodeMem();

            if (errCode < 0)
            {
                handleCVODEError(errCode);
            }

            if (oModel->getNumEvents() > 0)
            {
                errCode = rootInit(oModel->getNumEvents());//, EventFcn, gdata);
                Log(lDebug2)<<"CVRootInit executed.....";
            }

               errCode = CVDense(mCVODE_Memory, mStateVectorSize); // int = size of systems


            if (errCode < 0)
            {
                handleCVODEError(errCode);
            }

            oModel->resetEvents();
        }
        else if (mModel->getNumEvents() > 0)
        {
            int allocated = 1;
            mStateVector = N_VNew_Serial(allocated);
            mAbstolArray = N_VNew_Serial(allocated);

            SetVector(mStateVector, 0, 0);
            SetVector(mAbstolArray, 0, options.absolute);

            mCVODE_Memory = createCvode(&options);

            int errCode = allocateCvodeMem();
            if (errCode < 0)
            {
                handleCVODEError(errCode);
            }

            if (oModel->getNumEvents() > 0)
            {
                errCode = rootInit(oModel->getNumEvents());
                Log(lDebug2)<<"CVRootInit executed.....";
            }

            errCode = CVDense(mCVODE_Memory, allocated);
            if (errCode < 0)
            {
                handleCVODEError(errCode);
            }

            oModel->resetEvents();
        }
    }
    catch (const Exception& ex)
    {
        Log(Logger::LOG_ERROR)<<"Fatal Error while initializing CVODE: " << ex.getMessage();
        throw CVODEException("Fatal Error while initializing CVODE");
    }
}

void CvodeInterface::assignPendingEvents(double timeEnd, double tout)
{
    double *stateVector = mStateVector ? NV_DATA_S(mStateVector) : 0;
    int handled = mModel->applyPendingEvents(stateVector, timeEnd, tout);
    if (handled > 0)
    {
        reStart(timeEnd);
    }
}

void CvodeInterface::testRootsAtInitialTime()
{
    vector<unsigned char> initialEventStatus(mModel->getEventTriggers(0, 0, 0), false);
    mModel->getEventTriggers(initialEventStatus.size(), 0, &initialEventStatus[0]);
    handleRootsForTime(0, initialEventStatus);
}


void CvodeInterface::handleRootsForTime(double timeEnd, vector<unsigned char> &previousEventStatus)
{
    double *stateVector = mStateVector ? NV_DATA_S(mStateVector) : 0;
    mModel->evalEvents(timeEnd, &previousEventStatus[0], stateVector, stateVector);
    reInit(timeEnd);
}

void CvodeInterface::assignResultsToModel()
{
    if (mStateVector)
    {
        mModel->setStateVector(NV_DATA_S(mStateVector));
    }
}



void CvodeInterface::updateAbsTolVector()
{
    if (mStateVector == 0 || mModel == 0)
    {
        return;
    }

    mModel->getStateVector(NV_DATA_S(mStateVector));

    double dMin = options.absolute;

    for (int i = 0; i < NV_LENGTH_S(mStateVector); ++i)
    {
        double tmp = NV_DATA_S(mStateVector)[i] / 1000.;
        if (tmp < dMin)
        {
            dMin = tmp;
        }
    }

    for (int i = 0; i < NV_LENGTH_S(mStateVector); ++i)
    {
        setAbsTolerance(i, dMin);
    }

    // TODO: events are bizarre, need to clean them up eventually
    if (!haveVariables() && mModel->getNumEvents() > 0)
    {
        setAbsTolerance(0, dMin);
        SetVector(mStateVector, 0, 1.0);
    }

    Log(Logger::LOG_TRACE) << "Set tolerance to: "<<setprecision(16)<< dMin;

}

void CvodeInterface::setAbsTolerance(int index, double dValue)
{
    double dTolerance = dValue;
    if (dValue > 0 && options.absolute > dValue)
    {
        dTolerance = dValue;
    }
    else
    {
        dTolerance = options.absolute;
    }

    SetVector(mAbstolArray, index, dTolerance);
}

void CvodeInterface::reStart(double timeStart)
{
    if (mStateVector && mCVODE_Memory)
    {
        mModel->getStateVector(NV_DATA_S(mStateVector));
    }

    if(mCVODE_Memory)
    {
        reInit(timeStart);
    }
}


// Sets the value of an element in a N_Vector object
void SetVector (N_Vector v, int Index, double Value)
{
    double *data = NV_DATA_S(v);
    data[Index] = Value;
}

double GetVector (N_Vector v, int Index)
{
    double *data = NV_DATA_S(v);
    return data[Index];
}

void ModelFcn(int n, double time, double* y, double* ydot, void* userData)
{
    CvodeInterface* cvInstance = (CvodeInterface*) userData;
    if(!cvInstance)
    {
        Log(Logger::LOG_ERROR)<<"Problem in CVode Model Function!";
        return;
    }

    ExecutableModel *model = cvInstance->mModel;

    model->getStateVectorRate(time, y, ydot);

    if (cvInstance->mStateVectorSize == 0 && cvInstance->mStateVector &&
            NV_LENGTH_S(cvInstance->mStateVector) == 1)
    {
        ydot[0] = 0.0;
    }

    Log(Logger::LOG_TRACE) << __FUNC__ << endl;
    Log(Logger::LOG_TRACE) << model << endl;

    cvInstance->mCount++;
}

void EventFcn(double time, double* y, double* gdot, void* userData)
{
    CvodeInterface* cvInstance = (CvodeInterface*) userData;
    if(!cvInstance)
    {
        Log(Logger::LOG_ERROR)<<"Problem in CVode Model Function";
        return;
    }

    ExecutableModel *model = cvInstance->mModel;

    model->evalEventRoots(time, y, gdot);

    cvInstance->mRootCount++;
}

// Cvode calls this to compute the dy/dts. This routine in turn calls the
// model function which is located in the host application.
int InternalFunctionCall(realtype t, N_Vector cv_y, N_Vector cv_ydot, void *f_data)
{
    // Calls the callBackModel here
    ModelFcn(NV_LENGTH_S(cv_y), t, NV_DATA_S (cv_y), NV_DATA_S(cv_ydot), f_data);
    return CV_SUCCESS;
}

//int (*CVRootFn)(realtype t, N_Vector y, realtype *gout, void *user_data)
// Cvode calls this to check for event changes
int InternalRootCall (realtype t, N_Vector y, realtype *gout, void *g_data)
{
    EventFcn(t, NV_DATA_S (y), gout, g_data);
    return CV_SUCCESS;
}



_xmlNode* CvodeInterface::createConfigNode()
{
    _xmlNode *cap = Configurable::createCapabilityNode("Integration", "CVODE",
            "CVODE Integrator");

    Configurable::addChild(cap, Configurable::createParameterNode(
            "BDFOrder", "Maximum order for BDF Method", mMaxBDFOrder));
    Configurable::addChild(cap, Configurable::createParameterNode(
            "AdamsOrder", "Maximum order for Adams Method", mMaxAdamsOrder));
    Configurable::addChild(cap, Configurable::createParameterNode(
            "rtol", "Relative Tolerance", options.relative));
    Configurable::addChild(cap, Configurable::createParameterNode(
            "atol", "Absolute Tolerance", options.absolute));
    Configurable::addChild(cap, Configurable::createParameterNode(
            "maxsteps", "Maximum number of internal stepsc", options.maximumNumSteps));
    Configurable::addChild(cap, Configurable::createParameterNode(
            "initstep", "the initial step size", options.initialTimeStep));
    Configurable::addChild(cap, Configurable::createParameterNode(
            "minstep", "specifies a lower bound on the magnitude of the step size.", options.minimumTimeStep));
    Configurable::addChild(cap, Configurable::createParameterNode(
            "maxstep", "specifies an upper bound on the magnitude of the step size.", options.maximumTimeStep));

    return cap;
}

void CvodeInterface::loadConfig(const _xmlDoc* doc)
{
    mMaxBDFOrder = Configurable::getParameterIntValue(doc, "Integration", "BDFOrder");
    mMaxAdamsOrder = Configurable::getParameterIntValue(doc, "Integration", "AdamsOrder");
    options.relative = Configurable::getParameterDoubleValue(doc, "Integration", "rtol");
    options.absolute = Configurable::getParameterDoubleValue(doc, "Integration", "atol");
    options.maximumNumSteps = Configurable::getParameterIntValue(doc, "Integration", "maxsteps");
    options.initialTimeStep = Configurable::getParameterDoubleValue(doc, "Integration", "initstep");
    options.minimumTimeStep = Configurable::getParameterDoubleValue(doc, "Integration", "minstep");
    options.maximumTimeStep = Configurable::getParameterDoubleValue(doc, "Integration", "maxstep");
}


static std::string cvodeDecodeError(int cvodeError, bool exInfo)
{
    std::string result;

    switch (cvodeError)
    {
    case CV_TOO_MUCH_WORK:
        result = "CV_TOO_MUCH_WORK";
        if (exInfo)
        {
            result += ": The solver took mxstep internal steps but "
                    "could not reach tout. The default value for "
                    "mxstep is MXSTEP_DEFAULT = 500.";
        }
        break;
    case CV_TOO_MUCH_ACC:
        result = "CV_TOO_MUCH_ACC";
        if (exInfo)
        {
            result += ": The solver could not satisfy the accuracy "
                    "demanded by the user for some internal step.";
        }
        break;
    case CV_ERR_FAILURE:
        result = "CV_ERR_FAILURE";
        if (exInfo)
        {
            result += ": Error test failures occurred too many times "
                    "(= MXNEF = 7) during one internal time step or"
                    "occurred with |h| = hmin.";
        }
        break;
    case CV_CONV_FAILURE:
        result = "CV_CONV_FAILURE";
        if (exInfo)
        {
            result += ": Convergence test failures occurred too many "
                    "times (= MXNCF = 10) during one internal time"
                    "step or occurred with |h| = hmin.";
        }
        break;
    case CV_LINIT_FAIL:
        result = "CV_LINIT_FAIL";
        if (exInfo)
        {
            result += ": The linear solver's initialization function "
                    "failed.";
        }
        break;
    case CV_LSETUP_FAIL:
        result = "CV_LSETUP_FAIL";
        if (exInfo)
        {
            result += ": The linear solver's setup routine failed in an "
                    "unrecoverable manner.";
        }
        break;
    case CV_LSOLVE_FAIL:
        result = "CV_LSOLVE_FAIL";
        if (exInfo)
        {
            result += ": The linear solver's solve routine failed in an "
                    "unrecoverable manner.";
        }
        break;
    case CV_RHSFUNC_FAIL:
        result = "CV_RHSFUNC_FAIL";
        break;
    case CV_FIRST_RHSFUNC_ERR:
        result = "CV_FIRST_RHSFUNC_ERR";
        break;
    case CV_REPTD_RHSFUNC_ERR:
        result = "CV_REPTD_RHSFUNC_ERR";
        break;
    case CV_UNREC_RHSFUNC_ERR:
        result = "CV_UNREC_RHSFUNC_ERR";
        break;
    case CV_RTFUNC_FAIL:
        result = "CV_RTFUNC_FAIL";
        break;
    case CV_MEM_FAIL:
        result = "CV_MEM_FAIL";
        break;
    case CV_MEM_NULL:
        result = "CV_MEM_NULL";
        if (exInfo)
        {
            result += ": The cvode_mem argument was NULL.";
        }
        break;
    case CV_ILL_INPUT:
        result = "CV_ILL_INPUT";
        if (exInfo)
        {
            result += ": One of the inputs to CVode is illegal. This "
                    "includes the situation when a component of the "
                    "error weight vectors becomes < 0 during "
                    "internal time-stepping.  It also includes the "
                    "situation where a root of one of the root "
                    "functions was found both at t0 and very near t0. "
                    "The ILL_INPUT flag will also be returned if the "
                    "linear solver routine CV--- (called by the user "
                    "after calling CVodeCreate) failed to set one of "
                    "the linear solver-related fields in cvode_mem or "
                    "if the linear solver's init routine failed. In "
                    "any case, the user should see the printed "
                    "error message for more details.";
        }
        break;
    case CV_NO_MALLOC:
        result = "CV_NO_MALLOC";
        if (exInfo)
        {
            result += ": indicating that cvode_mem has not been "
                    "allocated (i.e., CVodeInit has not been "
                    "called).";
        }
        break;
    case CV_BAD_K:
        result = "CV_BAD_K";
        if (exInfo)
        {
            result += ": k is not in the range 0, 1, ..., qu.";
        }
        break;
    case CV_BAD_T:
        result = "CV_BAD_T";
        if (exInfo)
        {
            result += ": t is not in the interval [tn-hu,tn].";
        }
        break;
    case CV_BAD_DKY:
        result = "CV_BAD_DKY";
        if (exInfo)
        {
            result += ": The dky argument was NULL.";
        }
        break;
    case CV_TOO_CLOSE:
        result = "CV_TOO_CLOSE:";
        break;
    default:
        result = "UNKNOWN_CODE";
        break;
    }
    return result;
}



/**
 * Purpose
 * This function processes error and warning messages from CVODE and its
 * sub-modules.
 *
 * Arguments:
 * error_code: is the error code.
 * module: is the name of the CVODE module reporting the error.
 * function: is the name of the function in which the error occurred.
 * msg: is the error message.
 * eh_data: is a pointer to user data, the same as the eh_data parameter
 * passed to CVodeSetErrHandlerFn.
 * Return value: A CVErrHandlerFn function has no return value.
 *
 * Notes:
 * error_code is negative for errors and positive (CV_WARNING) for warnings.
 * If a function that returns a pointer to memory encounters an error,
 * it sets error_code to 0.
 */
void cvodeErrHandler(int error_code, const char *module, const char *function,
        char *msg, void *eh_data)
{
    if (error_code < 0) {
        Log(Logger::LOG_ERROR) << "CVODE Error: " << cvodeDecodeError(error_code, false)
                               << ", Module: " << module << ", Function: " << function
                               << ", Message: " << msg;

    }
    else if(error_code == CV_WARNING) {
        Log(Logger::LOG_WARNING) << "CVODE Warning: "
                                 << ", Module: " << module << ", Function: " << function
                                 << ", Message: " << msg;
    }
}

}

