#ifdef USE_PCH
#include "rr_pch.h"
#endif
#pragma hdrstop
#include <iomanip>
#include <math.h>
#include <map>
#include <algorithm>
#include "nvector/nvector_serial.h"
#include "cvode/cvode_dense.h"
#include "rrRoadRunner.h"
#include "rrExecutableModel.h"
#include "rrException.h"
#include "rrLogger.h"
#include "rrStringUtils.h"
#include "rrException.h"
#include "rrCVODEInterface.h"
#include "rrUtils.h"
#include "rrEvent.h"
//---------------------------------------------------------------------------

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






CvodeInterface::CvodeInterface(RoadRunner* rr, ExecutableModel *aModel, const double& _absTol, const double& _relTol)
:
mDefaultReltol(_relTol),
mDefaultAbsTol(_absTol),
mDefaultMaxNumSteps(10000),
mAmounts(NULL),
mAbstolArray(NULL),
mLogFile("cvodeLogFile"),
mCVODE_Memory(NULL),
mDefaultMaxAdamsOrder(12),
mDefaultMaxBDFOrder(5),
mLastTimeValue(0),
mLastEvent(0),
mOneStepCount(0),
mFollowEvents(true),
mRR(rr),
mMaxAdamsOrder(mDefaultMaxAdamsOrder),
mMaxBDFOrder(mDefaultMaxBDFOrder),
mInitStep(0.0),
mMinStep(0.0),
mMaxStep(0.0),
mMaxNumSteps(mDefaultMaxNumSteps),
mRelTol(_relTol),
mAbsTol(_absTol),
paramBDFOrder("BDFOrder", mMaxBDFOrder, "Maximum order for BDF Method"),
paramAdamsOrder("AdamsOrder", mMaxAdamsOrder, "Maximum order for Adams Method"),
paramRTol("rtol", mRelTol, "Relative Tolerance"),
paramATol("atol", mAbsTol, "Absolute Tolerance"),
paramMaxSteps("maxsteps", mMaxNumSteps, "Maximum number of internal stepsc"),
paramInitSteps("initstep", mInitStep, "the initial step size"),
paramMinStep("minstep", mMinStep, "specifies a lower bound on the magnitude of the step size."),
paramMaxStep("maxstep", mMaxStep, "specifies an upper bound on the    magnitude of the step size."),
mCVODECapability("Integration", "CVODE", "CVODE Integrator")
{
    //Setup capability
    mCVODECapability.addParameter(&paramBDFOrder);
    mCVODECapability.addParameter(&paramAdamsOrder);
    mCVODECapability.addParameter(&paramRTol);
    mCVODECapability.addParameter(&paramATol);
    mCVODECapability.addParameter(&paramMaxSteps);
    mCVODECapability.addParameter(&paramInitSteps);
    mCVODECapability.addParameter(&paramMinStep);
    mCVODECapability.addParameter(&paramMaxStep);

    if(rr)
    {
        mTempPathstring = rr->getTempFolder();
    }

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

    if(mAmounts)
    {
        N_VDestroy_Serial(mAmounts);
    }

    if(mAbstolArray)
    {
        N_VDestroy_Serial(mAbstolArray);
    }
}

void CvodeInterface::setTolerances(const double& aTol, const double& rTol)
{
    mAbsTol = aTol;
    mRelTol = rTol;
}

ExecutableModel*    CvodeInterface::getModel()
{
    return mTheModel;
}

Capability&    CvodeInterface::getCapability()
{
    return mCVODECapability;
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
        Log(lError)<<"Problem in setting CVODE User data";
    }

    int result =  CVodeInit(mCVODE_Memory, InternalFunctionCall, t0, mAmounts);

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

    int result = CVodeReInit(mCVODE_Memory,  t0, mAmounts);

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
            if (!haveVariables() && mTheModel->getNumEvents() == 0)
            {
                mTheModel->convertToAmounts();
                vector<double> args = buildEvalArgument();
                mTheModel->evalModel(tout, &args[0]);
                return tout;
            }

            if (mLastTimeValue > timeStart)
            {
                reStart(timeStart, mTheModel);
            }

            double nextTargetEndTime = tout;
            if (mAssignmentTimes.size() > 0 && mAssignmentTimes[0] < nextTargetEndTime)
            {
                nextTargetEndTime = mAssignmentTimes[0];
                mAssignmentTimes.erase(mAssignmentTimes.begin());
            }

            int nResult = CVode(mCVODE_Memory, nextTargetEndTime,  mAmounts, &timeEnd, CV_NORMAL);

            if (nResult == CV_ROOT_RETURN && mFollowEvents)
            {
                Log(lDebug1)<<("---------------------------------------------------");
                Log(lDebug1)<<"--- E V E N T      ( " << mOneStepCount << " ) ";
                Log(lDebug1)<<("---------------------------------------------------");

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
                    handleRootsFound(timeEnd, tout);
                    reStart(timeEnd, mTheModel);
                    mLastEvent = timeEnd;
                }
            }
            else if (nResult == CV_SUCCESS || !mFollowEvents)
            {
                //mTheModel->resetEvents();
                mTheModel->setTime(tout);
                assignResultsToModel();
            }
            else
            {
                handleCVODEError(nResult);
            }

            mLastTimeValue = timeEnd;

            try
            {
                mTheModel->testConstraints();
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
        Log(lError)<<"Problem in OneStep: "<<ex.getMessage()<<endl;
        initializeCVODEInterface(mTheModel);    //tk says ??? tk
        throw;
    }
}

void ModelFcn(int n, double time, double* y, double* ydot, void* userData)
{
    CvodeInterface* cvInstance = (CvodeInterface*) userData;
    if(!cvInstance)
    {
        Log(lError)<<"Problem in CVode Model Function!";
        return;
    }

    ExecutableModel *model = cvInstance->getModel();

    model->pushState();

    int size = model->getModelData().numFloatingSpecies + model->getModelData().numRateRules;
    vector<double> dCVodeArgument(size);

    for(int i = 0; i < min((int) dCVodeArgument.size(), n); i++)
    {
        dCVodeArgument[i] = y[i];
    }

    model->evalModel(time, &dCVodeArgument[0]);

    copyCArrayToStdVector(model->getModelData().rateRules,    dCVodeArgument, (model->getModelData().numRateRules));

    for(u_int i = 0 ; i < (model->getModelData().numFloatingSpecies); i++)
    {
        dCVodeArgument.push_back(model->getModelData().floatingSpeciesConcentrationRates[i]);
    }


    for (int i = 0; i < min((int) dCVodeArgument.size(), n); i++)
    {
        ydot[i]= dCVodeArgument[i];
    }

    cvInstance->mCount++;
    //oldState.AssignToModel(*model);
    model->popState();
}

void EventFcn(double time, double* y, double* gdot, void* userData)
{
    CvodeInterface* cvInstance = (CvodeInterface*) userData;
    if(!cvInstance)
    {
        Log(lError)<<"Problem in CVode Model Function";
        return;
    }

    ExecutableModel *model = cvInstance->getModel();

    //ModelState* oldState = new ModelState(*model);
    model->pushState();

    vector<double> args = cvInstance->buildEvalArgument();
    model->evalModel(time, &args[0]);
    cvInstance->assignResultsToModel();

    args = cvInstance->buildEvalArgument();
    model->evalEvents(time, args);

    for(int i = 0; i < model->getNumEvents(); i++)
    {
        gdot[i] = model->getModelData().eventTests[i];
    }

    cvInstance->mRootCount++;
    //oldState->AssignToModel(*model);
    //delete oldState;
    model->popState();
}

bool CvodeInterface::haveVariables()
{
    return (mNumAdditionalRules + mNumIndependentVariables > 0) ? true : false;
}

void CvodeInterface::initializeCVODEInterface(ExecutableModel *oModel)
{
    if(!oModel)
    {
        throw CVODEException("Fatal Error while initializing CVODE");
    }

    try
    {
        mTheModel = oModel;
        mNumIndependentVariables = oModel->getNumIndependentSpecies();
        mNumAdditionalRules = (oModel->getModelData().numRateRules);

        if (mNumAdditionalRules + mNumIndependentVariables > 0)
        {
            int allocatedMemory = mNumIndependentVariables + mNumAdditionalRules;
            mAmounts =     N_VNew_Serial(allocatedMemory);
            mAbstolArray = N_VNew_Serial(allocatedMemory);
            for (int i = 0; i < allocatedMemory; i++)
            {
                SetVector((N_Vector) mAbstolArray, i, mDefaultAbsTol);
            }

            assignNewVector(oModel, true);

            mCVODE_Memory = (void*) CVodeCreate(CV_BDF, CV_NEWTON);
            //SetMaxOrder(mCVODE_Memory, MaxBDFOrder);
            if(mCVODE_Memory)
            {
                CVodeSetMaxOrd(        mCVODE_Memory, mMaxBDFOrder);
                CVodeSetInitStep(    mCVODE_Memory, mInitStep);
                CVodeSetMinStep(    mCVODE_Memory, mMinStep);
                CVodeSetMaxStep(    mCVODE_Memory, mMaxStep);
                CVodeSetMaxNumSteps(mCVODE_Memory, mMaxNumSteps);
            }

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

               errCode = CVDense(mCVODE_Memory, allocatedMemory); // int = size of systems


            if (errCode < 0)
            {
                handleCVODEError(errCode);
            }

            oModel->resetEvents();
        }
        else if (mTheModel->getNumEvents() > 0)
        {
            int allocated = 1;
            mAmounts         = N_VNew_Serial(allocated);
            mAbstolArray     = N_VNew_Serial(allocated);
            SetVector(mAmounts, 0, 10);
            SetVector(mAbstolArray, 0, mDefaultAbsTol);

            mCVODE_Memory = (void*) CVodeCreate(CV_BDF, CV_NEWTON);
            CVodeSetMaxOrd(mCVODE_Memory, mMaxBDFOrder);
            CVodeSetMaxNumSteps(mCVODE_Memory, mMaxNumSteps);

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
        Log(lError)<<"Fatal Error while initializing CVODE: " << ex.getMessage();
        throw CVODEException("Fatal Error while initializing CVODE");
    }
}

void CvodeInterface::assignPendingEvents(const double& timeEnd, const double& tout)
{
    for (int i = (int) mAssignments.size() - 1; i >= 0; i--)
    {
        if (timeEnd >= mAssignments[i].GetTime())
        {
            mTheModel->setTime(tout);
            assignResultsToModel();
            mTheModel->convertToConcentrations();
            mTheModel->updateDependentSpeciesValues(mTheModel->getModelData().floatingSpeciesConcentrations);
            mAssignments[i].AssignToModel();

            if (mRR && !mRR->mConservedTotalChanged)
            {
                 mTheModel->computeConservedTotals();
            }

            mTheModel->convertToAmounts();
            vector<double> args = buildEvalArgument();
            mTheModel->evalModel(timeEnd, &args[0]);
            reStart(timeEnd, mTheModel);
            mAssignments.erase(mAssignments.begin() + i);
        }
    }
}

vector<int> CvodeInterface::retestEvents(const double& timeEnd, const vector<int>& handledEvents, vector<int>& removeEvents)
{
    return retestEvents(timeEnd, handledEvents, false, removeEvents);
}

vector<int> CvodeInterface::retestEvents(const double& timeEnd, vector<int>& handledEvents, const bool& assignOldState)
{
    vector<int> removeEvents;
    return retestEvents(timeEnd, handledEvents, assignOldState, removeEvents);
}

vector<int> CvodeInterface::retestEvents(const double& timeEnd, const vector<int>& handledEvents, const bool& assignOldState, vector<int>& removeEvents)
{
    vector<int> result;
//    vector<int> removeEvents;// = new vector<int>();    //Todo: this code was like this originally.. which removeEvents to use???

    if (mRR && !mRR->mConservedTotalChanged)
    {
        mTheModel->computeConservedTotals();
    }

    mTheModel->convertToAmounts();
    vector<double> args = buildEvalArgument();
    mTheModel->evalModel(timeEnd, &args[0]);

    // copy original evenStatusArray
    vector<bool> eventStatusArray(mTheModel->getModelData().eventStatusArray,
            mTheModel->getModelData().eventStatusArray +
            mTheModel->getModelData().numEvents / sizeof(bool));

    mTheModel->pushState();

    args = buildEvalArgument();
    mTheModel->evalEvents(timeEnd, args);

    for (int i = 0; i < mTheModel->getNumEvents(); i++)
    {
        bool containsI = (std::find(handledEvents.begin(), handledEvents.end(), i) != handledEvents.end()) ? true : false;
        if (mTheModel->getModelData().eventStatusArray[i] == true && eventStatusArray[i] == false && !containsI)
        {
            result.push_back(i);
        }

        if (mTheModel->getModelData().eventStatusArray[i] == false && eventStatusArray[i] == true && !mTheModel->getModelData().eventPersistentType[i])
        {
            removeEvents.push_back(i);
        }
    }

    mTheModel->popState(assignOldState ? 0 : ExecutableModel::PopDiscard);

    return result;
}

void CvodeInterface::handleRootsFound(double &timeEnd, const double& tout)
{
    vector<int> rootsFound(mTheModel->getNumEvents());

    // Create some space for the CVGetRootInfo call
    int* _rootsFound = new int[mTheModel->getNumEvents()];
    CVodeGetRootInfo(mCVODE_Memory, _rootsFound);
    copyCArrayToStdVector(_rootsFound, rootsFound, mTheModel->getNumEvents());
    delete [] _rootsFound;
    handleRootsForTime(timeEnd, rootsFound);
}

void CvodeInterface::testRootsAtInitialTime()
{
    vector<int> dummy;
    vector<int> events = retestEvents(0, dummy, true); //Todo: dummy is passed but is not used..?

    if (events.size() > 0)
    {
        vector<int> rootsFound(mTheModel->getNumEvents());//         = new int[mTheModel->getNumEvents];
        vector<int>::iterator iter;
        for(iter = rootsFound.begin(); iter != rootsFound.end(); iter ++)
        {
            (*iter) = 1;
        }
        handleRootsForTime(0, rootsFound);
    }
}

void CvodeInterface::removePendingAssignmentForIndex(const int& eventIndex)
{
    for (int j = (int) mAssignments.size() - 1; j >= 0; j--)
    {
        if (mAssignments[j].GetIndex() == eventIndex)
        {
            mAssignments.erase(mAssignments.begin() + j);
        }
    }
}

void CvodeInterface::sortEventsByPriority(vector<rr::Event>& firedEvents)
{
    if ((firedEvents.size() > 1))
    {
        Log(lDebug3)<<"Sorting event priorities";
        for(int i = 0; i < firedEvents.size(); i++)
        {
            firedEvents[i].SetPriority(mTheModel->getModelData().eventPriorities[firedEvents[i].GetID()]);
            Log(lDebug3)<<firedEvents[i];
        }
        sort(firedEvents.begin(), firedEvents.end(), SortByPriority());

        Log(lDebug3)<<"After sorting event priorities";
        for(int i = 0; i < firedEvents.size(); i++)
        {
            Log(lDebug3)<<firedEvents[i];
        }
    }
}

void CvodeInterface::sortEventsByPriority(vector<int>& firedEvents)
{
    if (firedEvents.size() > 1)
    {
        mTheModel->computeEventPriorites();
        vector<rr::Event> dummy;
        for(int i = 0; i < firedEvents.size(); i++)
        {
            dummy.push_back(firedEvents[i]);
        }

        Log(lDebug3)<<"Sorting event priorities";
        for(int i = 0; i < firedEvents.size(); i++)
        {
            dummy[i].SetPriority(mTheModel->getModelData().eventPriorities[dummy[i].GetID()]);
            Log(lDebug3)<<dummy[i];
        }
        sort(dummy.begin(), dummy.end(), SortByPriority());

        for(int i = 0; i < firedEvents.size(); i++)
        {
            firedEvents[i] = dummy[i].GetID();
        }

        Log(lDebug3)<<"After sorting event priorities";
        for(int i = 0; i < firedEvents.size(); i++)
        {
            Log(lDebug3)<<firedEvents[i];
        }
    }
}

void CvodeInterface::handleRootsForTime(const double& timeEnd, vector<int>& rootsFound)
{
    assignResultsToModel();
    mTheModel->convertToConcentrations();
    mTheModel->updateDependentSpeciesValues(mTheModel->getModelData().floatingSpeciesConcentrations);
    vector<double> args = buildEvalArgument();
    mTheModel->evalEvents(timeEnd, args);

    vector<int> firedEvents;
    map<int, double* > preComputedAssignments;

    for (int i = 0; i < mTheModel->getNumEvents(); i++)
    {
        // We only fire an event if we transition from false to true
        if (rootsFound[i] == 1)
        {
            if (mTheModel->getModelData().eventStatusArray[i])
            {
                firedEvents.push_back(i);
                if (mTheModel->getModelData().eventType[i])
                {
                    preComputedAssignments[i] = mTheModel->getModelData().computeEventAssignments[i](&(mTheModel->getModelData()));
                }
            }
        }
        else
        {
            // if the trigger condition is not supposed to be persistent, remove the event from the firedEvents list;
            if (!mTheModel->getModelData().eventPersistentType[i])
            {
                removePendingAssignmentForIndex(i);
            }
        }
    }

    vector<int> handled;
    while (firedEvents.size() > 0)
    {
        sortEventsByPriority(firedEvents);
        // Call event assignment if the eventstatus flag for the particular event is false
        for (u_int i = 0; i < firedEvents.size(); i++)
        {
            int currentEvent = firedEvents[i];//.GetID();

            // We only fire an event if we transition from false to true
            mTheModel->getModelData().previousEventStatusArray[currentEvent] = mTheModel->getModelData().eventStatusArray[currentEvent];
            double eventDelay = mTheModel->getModelData().eventDelays[currentEvent](&(mTheModel->getModelData()));
            if (eventDelay == 0)
            {
                if (mTheModel->getModelData().eventType[currentEvent] && preComputedAssignments.count(currentEvent) > 0)
                {
                    mTheModel->getModelData().performEventAssignments[currentEvent](&(mTheModel->getModelData()), preComputedAssignments[currentEvent]);
                }
                else
                {
                    mTheModel->getModelData().eventAssignments[currentEvent]();
                }

                handled.push_back(currentEvent);
                vector<int> removeEvents;
                vector<int> additionalEvents = retestEvents(timeEnd, handled, removeEvents);

                std::copy (additionalEvents.begin(), additionalEvents.end(), firedEvents.end());

                for (int j = 0; j < additionalEvents.size(); j++)
                {
                    int newEvent = additionalEvents[j];
                    if (mTheModel->getModelData().eventType[newEvent])
                    {
                        preComputedAssignments[newEvent] = mTheModel->getModelData().computeEventAssignments[newEvent](&(mTheModel->getModelData()));
                    }
                }

                mTheModel->getModelData().eventStatusArray[currentEvent] = false;
                Log(lDebug3)<<"Fired Event with ID:"<<currentEvent;
                firedEvents.erase(firedEvents.begin() + i);

                for (int i = 0; i < removeEvents.size(); i++)
                {
                    int item = removeEvents[i];
                    if (find(firedEvents.begin(), firedEvents.end(), item) != firedEvents.end())
                    {
                        firedEvents.erase(find(firedEvents.begin(), firedEvents.end(), item));
                        removePendingAssignmentForIndex(item);
                    }
                }

                break;
            }
            else
            {
                if (find(mAssignmentTimes.begin(), mAssignmentTimes.end(), timeEnd + eventDelay) == mAssignmentTimes.end())
                {
                    mAssignmentTimes.push_back(timeEnd + eventDelay);
                }

                PendingAssignment *pending = new PendingAssignment( &(mTheModel->getModelData()),
                                                                    timeEnd + eventDelay,
                                                                    mTheModel->getModelData().computeEventAssignments[currentEvent],
                                                                    mTheModel->getModelData().performEventAssignments[currentEvent],
                                                                    mTheModel->getModelData().eventType[currentEvent],
                                                                    currentEvent);

                if (mTheModel->getModelData().eventType[currentEvent] && preComputedAssignments.count(currentEvent) == 1)
                {
                    pending->ComputedValues = preComputedAssignments[currentEvent];
                }

                mAssignments.push_back(*pending);
                mTheModel->getModelData().eventStatusArray[currentEvent] = false;
                firedEvents.erase(firedEvents.begin() + i);
                break;
            }
        }
    }

    if (mRR && !mRR->mConservedTotalChanged)
    {
        mTheModel->computeConservedTotals();
    }
    mTheModel->convertToAmounts();


    args = buildEvalArgument();
    mTheModel->evalModel(timeEnd, &args[0]);

    vector<double> dCurrentValues(mTheModel->getModelData().numRateRules, 0);
    mTheModel->getRateRuleValues(&dCurrentValues[0]);

    for (int k = 0; k < mNumAdditionalRules; k++)
    {
        SetVector((N_Vector) mAmounts, k, dCurrentValues[k]);
    }

    for (int k = 0; k < mNumIndependentVariables; k++)
    {
        SetVector((N_Vector) mAmounts, k + mNumAdditionalRules, mTheModel->getModelData().floatingSpeciesAmounts[k]);
    }

    reInit(timeEnd);//, mAmounts, mRelTol, mAbstolArray);
    sort(mAssignmentTimes.begin(), mAssignmentTimes.end());
}

void CvodeInterface::assignResultsToModel()
{
    mTheModel->updateDependentSpeciesValues(mTheModel->getModelData().floatingSpeciesConcentrations);
    vector<double> dTemp(mNumAdditionalRules);

    for (int i = 0; i < mNumAdditionalRules; i++)
    {
        dTemp[i] = GetVector((_generic_N_Vector*) mAmounts, i);
    }

    for (int i = 0; i < mNumIndependentVariables; i++)
    {
        double val = GetVector((_generic_N_Vector*) mAmounts, i + mNumAdditionalRules);
        mTheModel->getModelData().floatingSpeciesAmounts[i] = (val);
        Log(lDebug5)<<"Amount "<<setprecision(16)<<val;
    }

    vector<double> args = buildEvalArgument();
    mTheModel->computeRules(args);
    mTheModel->setRateRuleValues(&dTemp[0]);

    mTheModel->computeAllRatesOfChange();
}

void CvodeInterface::assignNewVector(ExecutableModel *model)
{
    assignNewVector(model, false);
}

// Restart the simulation using a different initial condition
void CvodeInterface::assignNewVector(ExecutableModel *oModel, bool bAssignNewTolerances)
{
    vector<double> dTemp(mTheModel->getModelData().numRateRules, 0);
    mTheModel->getRateRuleValues(&dTemp[0]);

    double dMin = mAbsTol;

    for (int i = 0; i < mNumAdditionalRules; i++)
    {
        if (dTemp[i] > 0 && dTemp[i]/1000. < dMin)
        {
            dMin = dTemp[i]/1000.0;
        }
    }

    for (int i = 0; i < mNumIndependentVariables; i++)
    {
        if (oModel->getAmounts(i) > 0 && oModel->getAmounts(i)/1000.0 < dMin)    //Todo: was calling oModel->amounts[i]  is this in fact GetAmountsForSpeciesNr(i) ??
        {
            dMin = oModel->getModelData().floatingSpeciesAmounts[i]/1000.0;
        }
    }

    for (int i = 0; i < mNumAdditionalRules; i++)
    {
        if (bAssignNewTolerances)
        {
            setAbsTolerance(i, dMin);
        }
        SetVector(mAmounts, i, dTemp[i]);
    }

    for (int i = 0; i < mNumIndependentVariables; i++)
    {
        if (bAssignNewTolerances)
        {
            setAbsTolerance(i + mNumAdditionalRules, dMin);
        }
        SetVector(mAmounts, i + mNumAdditionalRules, oModel->getAmounts(i));
    }

    if (!haveVariables() && mTheModel->getNumEvents() > 0)
    {
        if (bAssignNewTolerances)
        {
            setAbsTolerance(0, dMin);
        }
        SetVector(mAmounts, 0, 1.0);
    }

    if (bAssignNewTolerances)
    {
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
        CVodeSetInitStep(mCVODE_Memory, mInitStep);
        CVodeSetMinStep(mCVODE_Memory, mMinStep);
        CVodeSetMaxStep(mCVODE_Memory, mMaxStep);
        reInit(timeStart);
    }
}

vector<double> CvodeInterface::buildEvalArgument()
{
    vector<double> dResult;
    dResult.resize(mTheModel->getModelData().numFloatingSpecies + mTheModel->getModelData().numRateRules);

    vector<double> dCurrentValues(mTheModel->getModelData().numRateRules, 0);
    mTheModel->getRateRuleValues(&dCurrentValues[0]);


    for(int i = 0; i < dCurrentValues.size(); i++)
    {
        dResult[i] = dCurrentValues[i];
    }

    for(int i = 0; i < mTheModel->getModelData().numFloatingSpecies; i++)
    {
        dResult[i + mTheModel->getModelData().numRateRules] = mTheModel->getModelData().floatingSpeciesAmounts[i];
    }

    Log(lDebug4)<<"Size of dResult in BuildEvalArgument: "<<dResult.size();
    return dResult;
}

void CvodeInterface::handleCVODEError(const int& errCode)
{
    if (errCode < 0)
    {
         string tempFolder;

        if(mRR)
        {
            tempFolder = mRR->getTempFolder();
        }
        else
        {
            tempFolder = ".";
        }

        string msg = "";
        string errorFile = joinPath(tempFolder, mLogFile) + toString(mErrorFileCounter) + ".txt";

        // and open a new file handle
        mErrorFileCounter++;
//        throw CvodeException("Error in RunCVode: " + errorCodes[-errCode].msg + msg);
        Log(lError)<<"**************** Error in RunCVode: "<<errCode<<msg<<" ****************************"<<endl;
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

}


