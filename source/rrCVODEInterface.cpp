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
mDefaultReltol(options->relative),
mDefaultAbsTol(options->absolute),
mStateVector(NULL),
mAbstolArray(NULL),
mCVODE_Memory(NULL),
mLastTimeValue(0),
mLastEvent(0),
mOneStepCount(0),
mFollowEvents(true),
mMaxAdamsOrder(mDefaultMaxAdamsOrder),
mMaxBDFOrder(mDefaultMaxBDFOrder),
mInitStep(0.0),
mMinStep(0.0),
mMaxStep(0.0),
mMaxNumSteps(mDefaultMaxNumSteps),
mRelTol(options->relative),
mAbsTol(options->absolute),
options(options)
{
    if(aModel)
    {
        initializeCVODEInterface(aModel);
    }
}

CvodeInterface::~CvodeInterface()
{
    //CVode crashes if handed NULL vectorc... (: ! ........
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

int CvodeInterface::allocateCvodeMem ()
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
    return CVodeSVtolerances(mCVODE_Memory, mRelTol, mAbstolArray);
}

int CvodeInterface::rootInit (const int& numRoots)//, TRootCallBack callBack, void *gdata)
{
    if (mCVODE_Memory == NULL)
    {
         return CV_SUCCESS;
    }

    return CVodeRootInit (mCVODE_Memory, numRoots, InternalRootCall);
}

// Initialize cvode with a new set of initial conditions
//int CvodeInterface::CVReInit (void *cvode_mem, double t0, N_Vector y0, double reltol, N_Vector abstol)
int CvodeInterface::reInit (const double& t0)
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

    return CVodeSVtolerances(mCVODE_Memory, mRelTol, mAbstolArray);
}

double CvodeInterface::oneStep(const double& _timeStart, const double& hstep)
{
    Log(lDebug3)<<"---------------------------------------------------";
    Log(lDebug3)<<"--- O N E     S T E P      ( "<<mOneStepCount<< " ) ";
    Log(lDebug3)<<"---------------------------------------------------";

    mOneStepCount++;
    mCount = 0;

    double timeEnd = 0.0;
    double timeStart = _timeStart;
    double tout = timeStart + hstep;
    int strikes = 3;

    // get the original event status
    vector<unsigned char> eventStatus(mModel->getEventTriggers(0, 0, 0), false);

    try
    {
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
                reStart(timeStart, mModel);
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
            int nResult = CVode(mCVODE_Memory, nextTargetEndTime,  mStateVector, &timeEnd, CV_NORMAL);

            if (nResult == CV_ROOT_RETURN && mFollowEvents)
            {
                Log(Logger::LOG_DEBUG) << ("---------------------------------------------------");
                Log(Logger::LOG_DEBUG) << "--- E V E N T   ( " << mOneStepCount << ", time: " << timeEnd << " ) ";
                Log(Logger::LOG_DEBUG) << ("---------------------------------------------------");


                bool tooCloseToStart = fabs(timeEnd - mLastEvent) > mRelTol;

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
                    handleRootsForTime(timeEnd, eventStatus);
                    reStart(timeEnd, mModel);
                    mLastEvent = timeEnd;
                }
            }
            else if (nResult == CV_SUCCESS || !mFollowEvents)
            {
                mModel->setTime(tout);
                assignResultsToModel();
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
        return (timeEnd);
    }
    catch(const Exception& ex)
    {
        Log(Logger::LOG_ERROR)<<"Problem in OneStep: "<<ex.getMessage()<<endl;
        initializeCVODEInterface(mModel);    //tk says ??? tk
        throw;
    }
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
                SetVector((N_Vector) mAbstolArray, i, mDefaultAbsTol);
            }

            assignNewVector(oModel, true);

            mCVODE_Memory = createCvode(options);

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
            SetVector(mAbstolArray, 0, mDefaultAbsTol);

            mCVODE_Memory = createCvode(options);

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

void CvodeInterface::assignPendingEvents(const double& timeEnd, const double& tout)
{
    double *stateVector = mStateVector ? NV_DATA_S(mStateVector) : 0;
    int handled = mModel->applyPendingEvents(stateVector, timeEnd, tout);
    if (handled > 0)
    {
        reStart(timeEnd, mModel);
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

void CvodeInterface::assignNewVector(ExecutableModel *model)
{
    assignNewVector(model, false);
}

// Restart the simulation using a different initial condition
void CvodeInterface::assignNewVector(ExecutableModel *oModel,
        bool assignNewTolerances)
{
    if (mStateVector == 0)
    {
        if (oModel && oModel->getStateVector(0) != 0)
        {
            Log(lWarning) << "Attempting to assign non-zero state vector to "
                    "zero length state vector in " << __FUNC__;
        }
        return;
    }

    if (oModel->getStateVector(0) > NV_LENGTH_S(mStateVector))
    {
        stringstream msg;
        msg << "attempt to assign different length data to existing state vector, ";
        msg << "new data has " << oModel->getStateVector(0) << " elements and ";
        msg << "existing state vector has " << NV_LENGTH_S(mStateVector);

        poco_error(getLogger(), msg.str());

        throw CVODEException(msg.str());
    }

    oModel->getStateVector(NV_DATA_S(mStateVector));

    if (assignNewTolerances)
    {
        double dMin = mAbsTol;

        for (int i = 0; i < NV_LENGTH_S(mStateVector); ++i)
        {
            double tmp = NV_DATA_S(mStateVector)[i] / 1000.;
            if (tmp > 0 && tmp < dMin)
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

        Log(lDebug1)<<"Set tolerance to: "<<setprecision(16)<< dMin;
    }
}

void CvodeInterface::setAbsTolerance(int index, double dValue)
{
    double dTolerance = dValue;
    if (dValue > 0 && mAbsTol > dValue)
    {
        dTolerance = dValue;
    }
    else
    {
        dTolerance = mAbsTol;
    }

    SetVector(mAbstolArray, index, dTolerance);
}

void CvodeInterface::reStart(double timeStart, ExecutableModel* model)
{
    assignNewVector(model);

    if(mCVODE_Memory)
    {
        //CVodeSetInitStep(mCVODE_Memory, mInitStep);
        //CVodeSetMinStep(mCVODE_Memory, mMinStep);
        //CVodeSetMaxStep(mCVODE_Memory, mMaxStep);
        reInit(timeStart);
    }
}


void CvodeInterface::handleCVODEError(const int& errCode)
{
    if (errCode < 0)
    {
        Log(Logger::LOG_ERROR) << "**************** Error in RunCVode: "
                << errCode << " ****************************" << endl;
        throw(Exception("Error in CVODE...!"));
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

unsigned CvodeInterface::setTolerances(double relative, double absolute)
{
    mRelTol = relative;
    mAbsTol = absolute;

    if (mAbstolArray)
    {
        for (unsigned i = 0; i < NV_LENGTH_S(mAbstolArray); ++i)
        {
            NV_DATA_S(mAbstolArray)[i] = mAbsTol;
        }
    }
    return 0;
}

unsigned CvodeInterface::getTolerances(double* relative, double* absolute)
{
    *relative = mRelTol;
    *absolute = mAbsTol;
    return 0;
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
            "rtol", "Relative Tolerance", mRelTol));
    Configurable::addChild(cap, Configurable::createParameterNode(
            "atol", "Absolute Tolerance", mAbsTol));
    Configurable::addChild(cap, Configurable::createParameterNode(
            "maxsteps", "Maximum number of internal stepsc", mMaxNumSteps));
    Configurable::addChild(cap, Configurable::createParameterNode(
            "initstep", "the initial step size", mInitStep));
    Configurable::addChild(cap, Configurable::createParameterNode(
            "minstep", "specifies a lower bound on the magnitude of the step size.", mMinStep));
    Configurable::addChild(cap, Configurable::createParameterNode(
            "maxstep", "specifies an upper bound on the magnitude of the step size.", mMaxStep));

    return cap;
}

void CvodeInterface::loadConfig(const _xmlDoc* doc)
{
    mMaxBDFOrder = Configurable::getParameterIntValue(doc, "Integration", "BDFOrder");
    mMaxAdamsOrder = Configurable::getParameterIntValue(doc, "Integration", "AdamsOrder");
    mRelTol = Configurable::getParameterDoubleValue(doc, "Integration", "rtol");
    mAbsTol = Configurable::getParameterDoubleValue(doc, "Integration", "atol");
    mMaxNumSteps = Configurable::getParameterIntValue(doc, "Integration", "maxsteps");
    mInitStep = Configurable::getParameterDoubleValue(doc, "Integration", "initstep");
    mMinStep = Configurable::getParameterDoubleValue(doc, "Integration", "minstep");
    mMaxStep = Configurable::getParameterDoubleValue(doc, "Integration", "maxstep");
}


}


