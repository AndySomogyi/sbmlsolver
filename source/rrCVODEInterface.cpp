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
#include "rrModelFromC.h"
#include "rrException.h"
#include "rrModelState.h"
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

void ModelFcn(int n, 	double time, double* y, double* ydot, void* userData);
void EventFcn(			double time, double* y, double* gdot, void* userData);

// N_Vector is a point to an N_Vector structure
RR_DECLSPEC void        Cvode_SetVector (N_Vector v, int Index, double Value);
RR_DECLSPEC double      Cvode_GetVector (N_Vector v, int Index);

//helper function for call back functions..
vector<double> BuildEvalArgument(ModelFromC* model);

//Static stuff...
//int         CvodeInterface::mCount = 0;
//int         CvodeInterface::mRootCount = 0;
//ModelFromC* CvodeInterface::model = NULL;

CvodeInterface::CvodeInterface(RoadRunner* rr, ModelFromC *aModel, const double& _absTol, const double& _relTol)
:
defaultReltol(_relTol),
defaultAbsTol(_absTol),
defaultMaxNumSteps(10000),
mAmounts(NULL),
cvodeLogFile("cvodeLogFile"),
followEvents(true),
defaultMaxAdamsOrder(12),
defaultMaxBDFOrder(5),
MaxAdamsOrder(defaultMaxAdamsOrder),
MaxBDFOrder(defaultMaxBDFOrder),
InitStep(0.0),
MinStep(0.0),
MaxStep(0.0),
MaxNumSteps(defaultMaxNumSteps),
relTol(_relTol),
absTol(_absTol),
abstolArray(NULL),
mRR(rr),
mCVODE_Memory(NULL),
mOneStepCount(0),
lastTimeValue(0),
lastEvent(0)
{
	if(rr)
	{
		tempPathstring = rr->getTempFileFolder();
	}

    InitializeCVODEInterface(aModel);
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

    if(abstolArray)
    {
    	N_VDestroy_Serial(abstolArray);
    }
}

// CallBack is the host application function that computes the dy/dt terms
int CvodeInterface::AllocateCvodeMem (int n)//, TModelCallBack callBack, double t0, N_Vector y, double reltol, N_Vector abstol/*, long int iopt[], double ropt[]*/)
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
    return CVodeSVtolerances(mCVODE_Memory, relTol, abstolArray);
}

int CvodeInterface::RootInit (int numRoots)//, TRootCallBack callBack, void *gdata)
{
    if (mCVODE_Memory == NULL)
    {
         return CV_SUCCESS;
    }

    return CVodeRootInit (mCVODE_Memory, numRoots, InternalRootCall);
}

// Initialize cvode with a new set of initial conditions
//int CvodeInterface::CVReInit (void *cvode_mem, double t0, N_Vector y0, double reltol, N_Vector abstol)
int CvodeInterface::ReInit (double t0)
{
    int result;

    if (mCVODE_Memory == NULL)
    {
        return CV_SUCCESS;
    }

    result = CVodeReInit(mCVODE_Memory,  t0, mAmounts);

    if (result != CV_SUCCESS)
    {
        return result;
    }

    result = CVodeSVtolerances(mCVODE_Memory, relTol, abstolArray);
    return result;
}

double CvodeInterface::OneStep(double timeStart, double hstep)
{
    Log(lDebug3)<<"---------------------------------------------------";
    Log(lDebug3)<<"--- O N E     S T E P      ( "<<mOneStepCount<< " ) ";
    Log(lDebug3)<<"---------------------------------------------------";

    mOneStepCount++;
    mCount = 0;

    double timeEnd = 0.0;
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
            if (!HaveVariables() && mTheModel->getNumEvents() == 0)
            {
                mTheModel->convertToAmounts();
                vector<double> args = BuildEvalArgument();
                mTheModel->evalModel(tout, args);
                return tout;
            }

            if (lastTimeValue > timeStart)
            {
                reStart(timeStart, mTheModel);
            }

            double nextTargetEndTime = tout;
            if (assignmentTimes.size() > 0 && assignmentTimes[0] < nextTargetEndTime)
            {
                nextTargetEndTime = assignmentTimes[0];
                assignmentTimes.erase(assignmentTimes.begin());
            }

            int nResult = CVode(mCVODE_Memory, nextTargetEndTime,  mAmounts, &timeEnd, CV_NORMAL);

            if (nResult == CV_ROOT_RETURN && followEvents)
            {
                Log(lDebug1)<<("---------------------------------------------------");
                Log(lDebug1)<<"--- E V E N T      ( " << mOneStepCount << " ) ";
                Log(lDebug1)<<("---------------------------------------------------");

                bool tooCloseToStart = fabs(timeEnd - lastEvent) > relTol;

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
                    HandleRootsFound(timeEnd, tout);
                    reStart(timeEnd, mTheModel);
                    lastEvent = timeEnd;
                }
            }
            else if (nResult == CV_SUCCESS || !followEvents)
            {
                //mTheModel->resetEvents();
                mTheModel->setTime(tout);
                AssignResultsToModel();
            }
            else
            {
                HandleCVODEError(nResult);
            }

            lastTimeValue = timeEnd;

            try
            {
                mTheModel->testConstraints();
            }
            catch (Exception e)
            {
                mTheModel->mWarnings.push_back("Constraint Violated at time = " + ToString(timeEnd) + "\n" + e.Message());
            }

            AssignPendingEvents(timeEnd, tout);

            if (tout - timeEnd > 1E-16)
            {
                timeStart = timeEnd;
            }
            Log(lDebug3)<<"tout: "<<tout<<tab<<"timeEnd: "<<timeEnd;
        }
        return (timeEnd);
    }
    catch (Exception)
    {
        InitializeCVODEInterface(mTheModel);
        throw;
    }
}

void ModelFcn(int n, double time, double* y, double* ydot, void* userData)
{
	CvodeInterface* cvInstance = (CvodeInterface*) userData;//CvodeInterface::mTheModel;
    if(!cvInstance)
    {
    	Log(lError)<<"Problem in CVode Model Function";
    	return;
    }

    ModelFromC *model = cvInstance->getModel();
    ModelState oldState(*model);

    int size = *model->amountsSize + (*model->rateRulesSize);
    vector<double> dCVodeArgument(size);

    for(int i = 0; i < min((int) dCVodeArgument.size(), n); i++)
    {
        dCVodeArgument[i] = y[i];
    }

//    stringstream msg;
//    msg<<left<<setw(20)<<"Count = "<<(CvodeInterface::mCount)<<"\t";

    //for (u_int i = 0; i < dCVodeArgument.size(); i++)
    //{
        //msg<<left<<setw(20)<<setprecision(4)<<dCVodeArgument[i];
    //}

    model->evalModel(time, dCVodeArgument);

    CopyCArrayToStdVector(model->rateRules,    dCVodeArgument, (*model->rateRulesSize));

    for(u_int i = 0 ; i < (*model->dydtSize); i++)
    {
        dCVodeArgument.push_back(model->dydt[i]);
    }

    //msg<<"\tcount = "<<CvodeInterface::mCount << "\t" ;
    //for (u_int i = 0; i < dCVodeArgument.size(); i++)
    //{
    //	msg<<setw(20)<<left<<setprecision(4)<<dCVodeArgument[i];
    //}

    //Log(lDebug4)<<msg.str();

    for (int i = 0; i < min((int) dCVodeArgument.size(), n); i++)
    {
        ydot[i]= dCVodeArgument[i];
    }

    cvInstance->mCount++;
    oldState.AssignToModel(*model);
}

////        void CvodeInterface::EventFcn(double time, IntPtr y, IntPtr gdot, IntPtr fdata)
void EventFcn(double time, double* y, double* gdot, void* userData)
{
//    ModelFromC *model = CvodeInterface::model;
	CvodeInterface* cvInstance = (CvodeInterface*) userData;
    if(!cvInstance)
    {
    	Log(lError)<<"Problem in CVode Model Function";
    	return;
    }

    ModelFromC *model = cvInstance->getModel();

    ModelState* oldState = new ModelState(*model);

    vector<double> args = BuildEvalArgument(model);
    model->evalModel(time, args);
    model->mCvodeInterface->AssignResultsToModel(); //AssignResultsToModel();

    args = BuildEvalArgument(model);
    model->evalEvents(time, args);

//    Marshal.Copy(model.eventTests, 0, gdot, model.getNumEvents());

    for(int i = 0; i < model->getNumEvents(); i++)
    {
        gdot[i] = model->eventTests[i];
    }

    Log(lDebug3)<<"S1 Value"<<model->amounts[0];
    Log(lDebug3)<<"Rootfunction Out: t="<<time<<" ("<< cvInstance->mRootCount <<"): ";
    for (int i = 0; i < *model->eventTestsSize; i++)
    {
        Log(lDebug3)<<ToString(model->eventTests[i])<<" p="<<model->previousEventStatusArray[i]<<" c="<<ToString(model->eventStatusArray[i])<<", ";
    }
    cvInstance->mRootCount++;
    oldState->AssignToModel(*model);
    delete oldState;
}

bool CvodeInterface::HaveVariables()
{
    return (numAdditionalRules + numIndependentVariables > 0) ? true : false;
}

void CvodeInterface::InitializeCVODEInterface(ModelFromC *oModel)
{
    if(!oModel)
    {
        throw CVODEException("Fatal Error while initializing CVODE");
    }

    try
    {
        mTheModel = oModel;
        numIndependentVariables = oModel->getNumIndependentVariables();
        numAdditionalRules = (*oModel->rateRulesSize);

        if (numAdditionalRules + numIndependentVariables > 0)
        {
            int allocatedMemory = numIndependentVariables + numAdditionalRules;
            mAmounts =     N_VNew_Serial(allocatedMemory);
            abstolArray = N_VNew_Serial(allocatedMemory);
            for (int i = 0; i < allocatedMemory; i++)
            {
                Cvode_SetVector((N_Vector) abstolArray, i, defaultAbsTol);
            }

            AssignNewVector(oModel, true);

            mCVODE_Memory = (void*) CVodeCreate(CV_BDF, CV_NEWTON);
            //SetMaxOrder(mCVODE_Memory, MaxBDFOrder);
            if(mCVODE_Memory)
            {
                CVodeSetMaxOrd(mCVODE_Memory, MaxBDFOrder);
                CVodeSetInitStep(mCVODE_Memory, InitStep);

                //SetMinStep(mCVODE_Memory, MinStep);
                CVodeSetMinStep(mCVODE_Memory, MinStep);

                //SetMaxStep(mCVODE_Memory, MaxStep);
                CVodeSetMaxStep(mCVODE_Memory, MaxStep);

                //SetMaxNumSteps(mCVODE_Memory, MaxNumSteps);
                CVodeSetMaxNumSteps(mCVODE_Memory, MaxNumSteps);
			}

            errCode = AllocateCvodeMem(allocatedMemory);//, ModelFcn, (double) 0.0, (N_Vector) mAmounts, relTol, (N_Vector) abstolArray);

            if (errCode < 0)
            {
                HandleCVODEError(errCode);
            }

            if (oModel->getNumEvents() > 0)
            {
                errCode = RootInit(oModel->getNumEvents());//, EventFcn, gdata);
                Log(lDebug2)<<"CVRootInit executed.....";
            }

           	errCode = CVDense(mCVODE_Memory, allocatedMemory); // int = size of systems


            if (errCode < 0)
            {
                HandleCVODEError(errCode);
            }

            oModel->resetEvents();
        }
        else if (mTheModel->getNumEvents() > 0)
        {
            int allocated = 1;
            mAmounts =  N_VNew_Serial(allocated);
            abstolArray =  N_VNew_Serial(allocated);
            Cvode_SetVector( (N_Vector) mAmounts, 0, 10);
            Cvode_SetVector( (N_Vector) abstolArray, 0, defaultAbsTol);

            mCVODE_Memory = (void*) CVodeCreate(CV_BDF, CV_NEWTON);
            CVodeSetMaxOrd(mCVODE_Memory, MaxBDFOrder);
			CVodeSetMaxNumSteps(mCVODE_Memory, MaxNumSteps);

            errCode = AllocateCvodeMem(allocated);//, ModelFcn, 0.0, (N_Vector) mAmounts, relTol, (N_Vector) abstolArray);
            if (errCode < 0)
            {
                HandleCVODEError(errCode);
            }

            if (oModel->getNumEvents() > 0)
            {
                errCode = RootInit(oModel->getNumEvents());//, EventFcn, gdata);
                Log(lDebug2)<<"CVRootInit executed.....";
            }

            errCode = CVDense(mCVODE_Memory, allocated); // int = size of systems
            if (errCode < 0)
            {
                HandleCVODEError(errCode);
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

void CvodeInterface::AssignPendingEvents(const double& timeEnd, const double& tout)
{
    for (int i = (int) assignments.size() - 1; i >= 0; i--)
    {
        if (timeEnd >= assignments[i].GetTime())
        {
            mTheModel->setTime(tout);
            AssignResultsToModel();
            mTheModel->convertToConcentrations();
            mTheModel->updateDependentSpeciesValues(mTheModel->y);
            assignments[i].AssignToModel();

            if (mRR && !mRR->mConservedTotalChanged)
            {
                 mTheModel->computeConservedTotals();
            }
            mTheModel->convertToAmounts();
            vector<double> args = BuildEvalArgument();
            mTheModel->evalModel(timeEnd, args);
            reStart(timeEnd, mTheModel);
            assignments.erase(assignments.begin() + i);
        }
    }
}

vector<int> CvodeInterface::RetestEvents(const double& timeEnd, const vector<int>& handledEvents, vector<int>& removeEvents)
{
    return RetestEvents(timeEnd, handledEvents, false, removeEvents);
}

vector<int> CvodeInterface::RetestEvents(const double& timeEnd, vector<int>& handledEvents, const bool& assignOldState)
{
    vector<int> removeEvents;
    return RetestEvents(timeEnd, handledEvents, assignOldState, removeEvents);
}

vector<int> CvodeInterface::RetestEvents(const double& timeEnd, const vector<int>& handledEvents, const bool& assignOldState, vector<int>& removeEvents)
{
    vector<int> result;
//    vector<int> removeEvents;// = new vector<int>();    //Todo: this code was like this originally.. which removeEvents to use???

    if (mRR && !mRR->mConservedTotalChanged)
    {
        mTheModel->computeConservedTotals();
    }

    mTheModel->convertToAmounts();
    vector<double> args = BuildEvalArgument();
    mTheModel->evalModel(timeEnd, args);

    ModelState *oldState = new ModelState(*mTheModel);

    args = BuildEvalArgument();
    mTheModel->evalEvents(timeEnd, args);

    for (int i = 0; i < mTheModel->getNumEvents(); i++)
    {
        bool containsI = (std::find(handledEvents.begin(), handledEvents.end(), i) != handledEvents.end()) ? true : false;
        if (mTheModel->eventStatusArray[i] == true && oldState->mEventStatusArray[i] == false && !containsI)
        {
            result.push_back(i);
        }

        if (mTheModel->eventStatusArray[i] == false && oldState->mEventStatusArray[i] == true && !mTheModel->eventPersistentType[i])
        {
            removeEvents.push_back(i);
        }
    }

    if (assignOldState)
    {
        oldState->AssignToModel(*mTheModel);
    }

    delete oldState;
    return result;
}


void CvodeInterface::HandleRootsFound(double &timeEnd, const double& tout)
{
    vector<int> rootsFound(mTheModel->getNumEvents());

    // Create some space for the CVGetRootInfo call
    _rootsFound = new int[mTheModel->getNumEvents()];
    CVodeGetRootInfo(mCVODE_Memory, _rootsFound);
    CopyCArrayToStdVector(_rootsFound, rootsFound, mTheModel->getNumEvents());
    delete [] _rootsFound;
    HandleRootsForTime(timeEnd, rootsFound);
}

void CvodeInterface::TestRootsAtInitialTime()
{
    vector<int> dummy;
    vector<int> events = RetestEvents(0, dummy, true); //Todo: dummy is passed but is not used..?
    if (events.size() > 0)
    {
        vector<int> rootsFound(mTheModel->getNumEvents());//         = new int[mTheModel->getNumEvents];
        vector<int>::iterator iter;
        for(iter = rootsFound.begin(); iter != rootsFound.end(); iter ++) //int item in events)
        {
            (*iter) = 1;
        }
        HandleRootsForTime(0, rootsFound);
    }
}

void CvodeInterface::RemovePendingAssignmentForIndex(const int& eventIndex)
{
    for (int j = (int) assignments.size() - 1; j >= 0; j--)
    {
        if (assignments[j].GetIndex() == eventIndex)
        {
            assignments.erase(assignments.begin() + j);
        }
    }
}

void CvodeInterface::SortEventsByPriority(vector<rr::Event>& firedEvents)
{
    if ((firedEvents.size() > 1))
    {
		Log(lDebug3)<<"Sorting event priorities";
        for(int i = 0; i < firedEvents.size(); i++)
        {
        	firedEvents[i].SetPriority(mTheModel->eventPriorities[firedEvents[i].GetID()]);
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

void CvodeInterface::SortEventsByPriority(vector<int>& firedEvents)
{
    if ((firedEvents.size() > 1))
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
        	dummy[i].SetPriority(mTheModel->eventPriorities[dummy[i].GetID()]);
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

void CvodeInterface::HandleRootsForTime(const double& timeEnd, vector<int>& rootsFound)
{
    AssignResultsToModel();
    mTheModel->convertToConcentrations();
    mTheModel->updateDependentSpeciesValues(mTheModel->y);
    vector<double> args = BuildEvalArgument();
    mTheModel->evalEvents(timeEnd, args);

    vector<int> firedEvents;
    map<int, double* > preComputedAssignments;

	for (int i = 0; i < mTheModel->getNumEvents(); i++)
    {
        // We only fire an event if we transition from false to true
        if (rootsFound[i] == 1)
        {
            if (mTheModel->eventStatusArray[i])
            {
                firedEvents.push_back(i);
                if (mTheModel->eventType[i])
                {
                    preComputedAssignments[i] = mTheModel->computeEventAssignments[i]();
                }
            }
        }
        else
        {
            // if the trigger condition is not supposed to be persistent, remove the event from the firedEvents list;
            if (!mTheModel->eventPersistentType[i])
            {
                RemovePendingAssignmentForIndex(i);
            }
        }
    }

    vector<int> handled;
    while (firedEvents.size() > 0)
    {
        SortEventsByPriority(firedEvents);
        // Call event assignment if the eventstatus flag for the particular event is false
        for (u_int i = 0; i < firedEvents.size(); i++)
        {
            int currentEvent = firedEvents[i];//.GetID();

            // We only fire an event if we transition from false to true
            mTheModel->previousEventStatusArray[currentEvent] = mTheModel->eventStatusArray[currentEvent];
            double eventDelay = mTheModel->eventDelays[currentEvent]();
            if (eventDelay == 0)
            {
                if (mTheModel->eventType[currentEvent] && preComputedAssignments.count(currentEvent) > 0)
                {
                    mTheModel->performEventAssignments[currentEvent](preComputedAssignments[currentEvent]);
                }
                else
                {
                    mTheModel->eventAssignments[currentEvent]();
                }

                handled.push_back(currentEvent);
                vector<int> removeEvents;
                vector<int> additionalEvents = RetestEvents(timeEnd, handled, removeEvents);

                std::copy (additionalEvents.begin(), additionalEvents.end(), firedEvents.end());

                for (int j = 0; j < additionalEvents.size(); j++)
                {
                    int newEvent = additionalEvents[j];
                    if (mTheModel->eventType[newEvent])
                    {
                        preComputedAssignments[newEvent] = mTheModel->computeEventAssignments[newEvent]();
                    }
                }

                mTheModel->eventStatusArray[currentEvent] = false;
                Log(lDebug3)<<"Fired Event with ID:"<<currentEvent;
                firedEvents.erase(firedEvents.begin() + i);

                for (int i = 0; i < removeEvents.size(); i++)
                {
                    int item = removeEvents[i];
                    if (find(firedEvents.begin(), firedEvents.end(), item) != firedEvents.end())
                    {
                        firedEvents.erase(find(firedEvents.begin(), firedEvents.end(), item));
                        RemovePendingAssignmentForIndex(item);
                    }
                }

                break;
            }
            else
            {
                if (find(assignmentTimes.begin(), assignmentTimes.end(), timeEnd + eventDelay) == assignmentTimes.end())
                {
                    assignmentTimes.push_back(timeEnd + eventDelay);
                }

                PendingAssignment *pending = new PendingAssignment(  timeEnd + eventDelay,
                                                                    mTheModel->computeEventAssignments[currentEvent],
                                                                    mTheModel->performEventAssignments[currentEvent],
                                                                    mTheModel->eventType[currentEvent],
                                                                    currentEvent);

                if (mTheModel->eventType[currentEvent] && preComputedAssignments.count(currentEvent) == 1)
                {
                    pending->ComputedValues = preComputedAssignments[currentEvent];
                }

                assignments.push_back(*pending);
                mTheModel->eventStatusArray[currentEvent] = false;
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


    args = BuildEvalArgument();
    mTheModel->evalModel(timeEnd, args);

    vector<double> dCurrentValues = mTheModel->getCurrentValues();
    for (int k = 0; k < numAdditionalRules; k++)
    {
        Cvode_SetVector((N_Vector) mAmounts, k, dCurrentValues[k]);
    }

    for (int k = 0; k < numIndependentVariables; k++)
    {
        Cvode_SetVector((N_Vector) mAmounts, k + numAdditionalRules, mTheModel->amounts[k]);
    }

//    CVReInit(mCVODE_Memory, timeEnd, mAmounts, relTol, abstolArray);
    ReInit(timeEnd);//, mAmounts, relTol, abstolArray);
    sort(assignmentTimes.begin(), assignmentTimes.end());
}

void CvodeInterface::AssignResultsToModel()
{
    mTheModel->updateDependentSpeciesValues(mTheModel->y);
    vector<double> dTemp(numAdditionalRules);

    for (int i = 0; i < numAdditionalRules; i++)
    {
        dTemp[i] = Cvode_GetVector((_generic_N_Vector*) mAmounts, i);
        mTheModel->amounts[i] = dTemp[i];
    }

    for (int i = 0; i < numIndependentVariables; i++) //
    {
        double val = Cvode_GetVector((_generic_N_Vector*) mAmounts, i + numAdditionalRules);
        mTheModel->amounts[i] = (val);
        Log(lDebug5)<<"Amount "<<setprecision(16)<<val;
    }

    vector<double> args = BuildEvalArgument();
    mTheModel->computeRules(args);
    mTheModel->assignRates(dTemp);

    mTheModel->computeAllRatesOfChange();
}

// Restart the simulation using a different initial condition
void CvodeInterface::AssignNewVector(ModelFromC *oModel, bool bAssignNewTolerances)
{
    vector<double> dTemp = mTheModel->getCurrentValues();
    double dMin = absTol;

    for (int i = 0; i < numAdditionalRules; i++)
    {
        if (dTemp[i] > 0 && dTemp[i]/1000. < dMin)
        {
            dMin = dTemp[i]/1000.0;
        }
    }

    for (int i = 0; i < numIndependentVariables; i++)
    {
        if (oModel->getAmounts(i) > 0 && oModel->getAmounts(i)/1000.0 < dMin)    //Todo: was calling oModel->amounts[i]  is this in fact GetAmountsForSpeciesNr(i) ??
        {
            dMin = oModel->amounts[i]/1000.0;
        }
    }

    for (int i = 0; i < numAdditionalRules; i++)
    {
        if (bAssignNewTolerances)
        {
            setAbsTolerance(i, dMin);
        }
        Cvode_SetVector(mAmounts, i, dTemp[i]);
    }

    for (int i = 0; i < numIndependentVariables; i++)
    {
        if (bAssignNewTolerances)
        {
            setAbsTolerance(i + numAdditionalRules, dMin);
        }
        Cvode_SetVector(mAmounts, i + numAdditionalRules, oModel->getAmounts(i));
    }

    if (!HaveVariables() && mTheModel->getNumEvents() > 0)
    {
        if (bAssignNewTolerances)
        {
            setAbsTolerance(0, dMin);
        }
        Cvode_SetVector(mAmounts, 0, 1.0);
    }

    if (bAssignNewTolerances)
    {
        Log(lDebug1)<<"Set tolerance to: "<<setprecision(16)<< dMin;
    }
}

void CvodeInterface::AssignNewVector(ModelFromC *model)
{
    AssignNewVector(model, false);
}

void CvodeInterface::setAbsTolerance(int index, double dValue)
{
    double dTolerance = dValue;
    if (dValue > 0 && absTol > dValue)
    {
        dTolerance = dValue;
    }
    else
    {
        dTolerance = absTol;
    }

    Cvode_SetVector(abstolArray, index, dTolerance);
}

void CvodeInterface::reStart(double timeStart, ModelFromC* model)
{
    AssignNewVector(model);

    if(mCVODE_Memory)
    {
    	CVodeSetInitStep(mCVODE_Memory, InitStep);
    	CVodeSetMinStep(mCVODE_Memory, MinStep);
    	CVodeSetMaxStep(mCVODE_Memory, MaxStep);
		ReInit(timeStart);
    }
}

vector<double> BuildEvalArgument(ModelFromC* model)
{
    vector<double> dResult;
    dResult.resize(*model->amountsSize + *model->rateRulesSize);

    vector<double> dCurrentValues = model->getCurrentValues();
    for(int i = 0; i < dCurrentValues.size(); i++)
    {
        dResult[i] = dCurrentValues[i];
    }

    for(int i = 0; i < *model->amountsSize; i++)
    {
        dResult[i + *model->rateRulesSize] = model->amounts[i];
    }

    Log(lDebug4)<<"Size of dResult in BuildEvalArgument: "<<dResult.size();
    return dResult;
}

vector<double> CvodeInterface::BuildEvalArgument()
{
    vector<double> dResult;
    dResult.resize(*mTheModel->amountsSize + *mTheModel->rateRulesSize);

    vector<double> dCurrentValues = mTheModel->getCurrentValues();
    for(int i = 0; i < dCurrentValues.size(); i++)
    {
        dResult[i] = dCurrentValues[i];
    }

    for(int i = 0; i < *mTheModel->amountsSize; i++)
    {
        dResult[i + *mTheModel->rateRulesSize] = mTheModel->amounts[i];
    }

    Log(lDebug4)<<"Size of dResult in BuildEvalArgument: "<<dResult.size();
    return dResult;
}

void CvodeInterface::HandleCVODEError(int errCode)
{
    if (errCode < 0)
    {
        string msg = "";
        string errorFile = tempPathstring + cvodeLogFile + ToString(errorFileCounter) + ".txt";

        // and open a new file handle
        errorFileCounter++;
        //throw CvodeException("Error in RunCVode: " + errorCodes[-errCode].msg + msg);
        Log(lError)<<"**************** Error in RunCVode: "<<errCode<<msg<<" ****************************"<<endl;
    }
}

// Sets the value of an element in a N_Vector object
void Cvode_SetVector (N_Vector v, int Index, double Value)
{
    double *data = NV_DATA_S(v);
    data[Index] = Value;
}

double Cvode_GetVector (N_Vector v, int Index)
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

//int CVRootInit (void *cvode_mem, int numRoots, TRootCallBack callBack, void *gdata)
//{
//    if (cvode_mem == NULL)
//    {
//         return CV_SUCCESS;
//    }
//
////    gCallBackRoot = callBack;
//    return CVodeRootInit (cvode_mem, numRoots, InternalRootCall);
//}

//// Initialize cvode with a new set of initial conditions
//int CVReInit (void *cvode_mem, double t0, N_Vector y0, double reltol, N_Vector abstol)
//{
//    int result;
//
//    if (cvode_mem == NULL)
//    {
//        return CV_SUCCESS;
//    }
//
//    result = CVodeReInit(cvode_mem,  t0, y0);
//
//    if (result != CV_SUCCESS)
//    {
//        return result;
//    }
//
//    result = CVodeSVtolerances(cvode_mem, reltol, abstol);
//    return result;
//}

} //namespace rr


