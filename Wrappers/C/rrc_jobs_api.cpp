#pragma hdrstop
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#include "rrRoadRunner.h"
#include "rrLoadModel.h"
#include "rrSimulate.h"
#include "rrException.h"
#include "rrLoadModelThread.h"
#include "rrSimulateThread.h"
#include "rrc_utilities.h" 		// Need to include this before the support header..
#include "rrc_support.h"   		//Support functions, not exposed as api functions and or data
#include "rrc_jobs_api.h"
//---------------------------------------------------------------------------

namespace rrc
{
using namespace std;
using namespace rr;
using namespace rrc;


RRJobHandle rrCallConv loadSBMLFromFileJob(RRHandle rrHandle, const char* fileName)
{
	try
    {
        //Check if file exists first
        if(!FileExists(fileName))
        {
            stringstream msg;
            msg<<"The file "<<fileName<<" do not exist";
            setError(msg.str());
            return NULL;
        }

        RoadRunner* rr = castFrom(rrHandle);
        LoadModelThread* loadThread = new LoadModelThread(fileName);

        if(!loadThread)
        {
            setError("Failed to create a LoadModel Thread");
        }
        loadThread->addJob(rr);
        loadThread->start();
        return loadThread;
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
	    return NULL;
    }
}

RRJobHandle rrCallConv loadSBMLFromFileJobEx(RRHandle rrHandle, const char* fileName, bool recompile)
{
	try
    {
        //Check if file exists first
        if(!FileExists(fileName))
        {
            stringstream msg;
            msg<<"The file "<<fileName<<" do not exist";
            setError(msg.str());
            return NULL;
        }

        RoadRunner* rr = castFrom(rrHandle);
        LoadModelThread* loadThread = new LoadModelThread(fileName, recompile);

        if(!loadThread)
        {
            setError("Failed to create a LoadModel Thread");
        }

        loadThread->addJob(rr);
        loadThread->start();
        return loadThread;
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
	    return NULL;
    }
}

bool rrCallConv freeJob(RRJobHandle aJob)
{
	try
    {
        LoadModelThread* theJob = (LoadModelThread*) aJob;
        if(theJob)
        {
    	   delete theJob;
        }
        return true;
    }
    CATCH_BOOL_MACRO
}

RRJobsHandle rrCallConv loadSBMLFromFileJobs(RRInstanceListHandle _handles, const char* fileName, int nrOfThreads)
{
	try
    {
        //Check if file exists first
        if(!FileExists(fileName))
        {
            stringstream msg;
            msg<<"The file "<<fileName<<" do not exist";
            setError(msg.str());
            return NULL;
        }

        RoadRunnerList *rrs = getRRList(_handles);
        LoadModel* tp = new LoadModel(*rrs, fileName, nrOfThreads);

        if(!tp)
        {
            setError("Failed to create a LoadModel Thread Pool");
        }
        return tp;
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
	    return NULL;
    }
}

bool rrCallConv waitForJob(RRJobHandle handle)
{
	try
    {
        RoadRunnerThread* aThread = (RoadRunnerThread*) handle;
        if(aThread)
        {
			aThread->waitForFinish();
            return true;
        }
		return false;
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
	    return false;
    }
}

bool rrCallConv waitForJobs(RRJobsHandle handle)
{
	try
    {
        ThreadPool* aTP = (ThreadPool*) handle;
        if(aTP)
        {
			aTP->waitForFinish();
            return true;
        }
		return false;
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
	    return false;
    }
}

bool rrCallConv isJobFinished(RRJobHandle handle)
{
	try
    {
        RoadRunnerThread* aT = (RoadRunnerThread*) handle;
        if(aT)
        {
			return ! aT->isActive();
        }
		return false;
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
	    return false;
    }
}

bool rrCallConv areJobsFinished(RRJobsHandle handle)
{
	try
    {
        ThreadPool* aTP = (ThreadPool*) handle;
        if(aTP)
        {
			return ! aTP->isWorking();
        }
		return false;
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
	    return false;
    }
}

int rrCallConv getNumberOfRemainingJobs(RRJobHandle handle)
{
	try
    {
        ThreadPool* aTP = (ThreadPool*) handle;
        if(aTP)
        {
            return aTP->getNumberOfRemainingJobs();
        }
    	return -1;
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
	    return -1;
    }
}

RRJobHandle rrCallConv simulateJob(RRHandle rrHandle)
{
	try
    {
        RoadRunner *rr = castFrom(rrHandle);
        SimulateThread *t = new SimulateThread(NULL, false);

        if(!t)
        {
            setError("Failed to create a Simulate Thread Pool");
        }
        t->addJob(rr);
        t->start();
        return t;
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
		return NULL;
    }
}

RRJobHandle rrCallConv simulateJobEx(	RRHandle rrHandle,
										double timeStart,
                                        double timeEnd,
                                        int numberOfPoints,
                                        jobStartedCB f1,
                                        jobFinishedCB f2,
                                        void* userData)
{
	try
    {
        RoadRunner *rr = castFrom(rrHandle);
        SimulateThread *t = new SimulateThread(NULL, timeStart, timeEnd, numberOfPoints, f1, f2, userData, false);

        if(!t)
        {
            setError("Failed to create a Simulate Thread Pool");
        }
        t->addJob(rr);
        t->start();
        return t;
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
		return NULL;
    }
}
RRJobHandle rrCallConv simulateJobs(RRInstanceListHandle _handles, int nrOfThreads)
{
	try
    {
        RoadRunnerList *rrs = getRRList(_handles);
        Simulate* tp = new Simulate(*rrs, nrOfThreads);

        if(!tp)
        {
            setError("Failed to create a Simulate Thread Pool");
        }
        return tp;
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
		return NULL;
    }
}

}
