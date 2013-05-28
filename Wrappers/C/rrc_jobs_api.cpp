#pragma hdrstop
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include "rrRoadRunner.h"
#include "rrLoadModel.h"
#include "rrLoadModelFromFile.h"
#include "rrSimulate.h"
#include "rrException.h"
#include "rrLoadModelThread.h"
#include "rrSimulateThread.h"
#include "rrc_utilities.h" 		// Need to include this before the support header..
#include "rrc_cpp_support.h" 		// Need to include this before the support header..
#include "rrc_jobs_api.h"
//---------------------------------------------------------------------------

namespace rrc
{
using namespace std;
using namespace rr;

RRJobHandle rrCallConv loadSBMLFromFileJob(RRHandle rrHandle, const char* fileName)
{
	try
    {
        //Check if file exists first
        if(!fileExists(fileName))
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
    catch_ptr_macro
}

RRJobHandle rrCallConv loadSBMLJob(RRHandle rrHandle, const char* sbml)
{
	try
    {
        RoadRunner* rr = castFrom(rrHandle);
        LoadModelThread* loadThread = new LoadModelThread();
        loadThread->setSBML(sbml);

        if(!loadThread)
        {
            setError("Failed to create a LoadModel Thread");
        }
        loadThread->addJob(rr);
        loadThread->start();
        return loadThread;
    }
    catch_ptr_macro
}

RRJobHandle rrCallConv loadSBMLFromFileJobEx(RRHandle rrHandle, const char* fileName, bool recompile)
{
	try
    {
        //Check if file exists first
        if(!fileExists(fileName))
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
    catch_ptr_macro
}

RRJobHandle rrCallConv loadSBMLJobEx(RRHandle rrHandle, const char* sbml, bool recompile)
{
	try
    {

        RoadRunner* rr = castFrom(rrHandle);
        LoadModelThread* loadThread = new LoadModelThread("", recompile);
        loadThread->setSBML(sbml);

        if(!loadThread)
        {
            setError("Failed to create a LoadModel Thread");
        }

        loadThread->addJob(rr);
        loadThread->start();
        return loadThread;
    }
    catch_ptr_macro
}

RRJobsHandle rrCallConv loadSBMLFromFileJobs(RRInstanceListHandle _handles, const char* fileName, int nrOfThreads)
{
	try
    {
        //Check if file exists first
        if(!fileExists(fileName))
        {
            stringstream msg;
            msg<<"The file "<<fileName<<" do not exist";
            setError(msg.str());
            return NULL;
        }

        RoadRunnerList *rrs = getRRList(_handles);
        LoadModelFromFile* tp = new LoadModelFromFile(*rrs, fileName, nrOfThreads);

        if(!tp)
        {
            setError("Failed to create a LoadModel Thread Pool");
        }
        return tp;
    }
    catch_ptr_macro
}

RRJobsHandle rrCallConv loadSBMLJobs(RRInstanceListHandle _handles, const char* sbml, int nrOfThreads)
{
	try
    {
        RoadRunnerList *rrs = getRRList(_handles);
        LoadModel* tp = new LoadModel(*rrs, sbml, nrOfThreads);


        if(!tp)
        {
            setError("Failed to create a LoadModel Thread Pool");
        }
        return tp;
    }
    catch_ptr_macro
}

RRJobsHandle rrCallConv loadSBMLJobsEx(RRInstanceListHandle _handles, const char* sbml, int nrOfThreads, bool reCompile)
{
	try
    {
        RoadRunnerList *rrs = getRRList(_handles);
        LoadModel* tp = new LoadModel(*rrs, sbml, nrOfThreads, reCompile);


        if(!tp)
        {
            setError("Failed to create a LoadModel Thread Pool");
        }
        return tp;
    }
    catch_ptr_macro
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
    catch_bool_macro
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
	catch_bool_macro
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
	catch_bool_macro
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
	catch_bool_macro
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
	catch_int_macro
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
	catch_ptr_macro
}

RRJobHandle rrCallConv simulateJobEx(	RRHandle rrHandle,
										double timeStart,
                                        double timeEnd,
                                        int numberOfPoints,
                                        callBackFunc f1,
                                        callBackFunc f2,
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
	catch_ptr_macro
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
	catch_ptr_macro
}

RRJobHandle rrCallConv simulateJobsEx(	RRInstanceListHandle _handles,
										int nrOfThreads,
                                        double timeStart,
                                        double timeEnd,
                                        int numPoints,
                                        callBackFunc fn1,
                                        callBackFunc fn2,
                                        void* userData)
{
	try
    {
        RoadRunnerList *rrs = getRRList(_handles);

        for(int i = 0; i < rrs->count(); i++)
        {
	        (*rrs)[i]->setTimeStart(timeStart);
	        (*rrs)[i]->setTimeEnd(timeEnd);
			(*rrs)[i]->setNumPoints(numPoints);
        }

	    Simulate* tp = new Simulate(*rrs, nrOfThreads);

        if(!tp)
        {
            setError("Failed to create a Simulate Thread Pool");
        }
        return tp;
    }
	catch_ptr_macro
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
    catch_bool_macro
}

bool rrCallConv freeJobs(RRJobsHandle handle)
{
	try
    {
        ThreadPool* jobs = (ThreadPool*) handle;
        if(jobs)
        {
    	   delete jobs;
        }
        return true;
    }
    catch_bool_macro
}

}
