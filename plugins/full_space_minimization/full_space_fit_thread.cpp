#pragma hdrstop
#include "rrSimulationData.h"
#include "rrLogger.h"
#include "full_space_fit_thread.h"
#include "rrNoise.h"
#include "full_space_minimization.h"
#include "../../Wrappers/C/rrc_api.h"
#include "../../Wrappers/C/rrc_support.h"

//---------------------------------------------------------------------------

namespace fullSpaceFit
{
double getChiSquare(SimulationData& data1, SimulationData& data2);

using namespace rr;
FullSpaceFitThread::FullSpaceFitThread(FullSpaceMinimization& host)
:
threadEnterCB(NULL),
threadExitCB(NULL),
mUserData(NULL),
mTheHost(host)
{}

void FullSpaceFitThread::assignCallBacks(ThreadCB fn1, ThreadCB fn2, void* userData)
{
	threadEnterCB 	= fn1;
	threadExitCB  	= fn2;
    mUserData 		= userData;
}

void FullSpaceFitThread::start(SimulationData* inputData)
{
	mInputData = inputData;

    if(mThread.isRunning())
    {
    	Log(lError)<<"Tried to start a working thread!";
        return;
    }

	mThread.start(*this);	//Will spawn the run function below in a thread
}

void FullSpaceFitThread::run()
{
	if(threadEnterCB)
    {
		threadEnterCB(mUserData);	//Tell anyone who wants to know
    }

    SimulationData& inputData = *(mInputData);

	//Allocate
    int handleCount = mTheHost.mStepsPerDimension.getValue();
    int threadCount = mTheHost.mNumberOfThreads.getValue();

    RRInstanceListHandle rrs = createRRInstances(handleCount);

	if(!setTempFolder(rrs->Handle[0], mTheHost.getTempFolder().c_str()))
    {
        Log(lError)<<"Failed setting up temporary folder";
    	return;
    }

	//loadSBML models in threads
    RRJobsHandle jobsHandle = loadSBMLJobsEx(rrs, mTheHost.getSBML().c_str(), threadCount, false);

    waitForJobs(jobsHandle);
//    while(!areJobsFinished(jobsHandle))
//    {
//    	Log(lInfo)<<"Loading models... ";
//        sleep(10);
//    };

    freeJobs(jobsHandle);
    vector<string> parasToFit = mTheHost.getParametersToFit();

	double modelValue;
    if(!getValue(rrs->Handle[0], parasToFit[0].c_str(), &modelValue))
    {
		//Cleanup and exit...
        return;
    }

    double range = (mTheHost.mParameterSweepRange.getValue()/100.0) / 2.0;
	double parValue = range * modelValue;  //Start of 'sweep'
    double increment = 2.0*(modelValue - parValue) / handleCount;

	//Get the species from input data
    string selList;
    for(int i = 1; i < inputData.cSize(); i++) //First one is time
    {
		selList += inputData.getColumnName(i);
        if(i < inputData.cSize())
        {
        	selList += ",";
        }
    }

    selList = "time," + selList;
	//Setup roadrunners for simulations
    for(int i = 0; i < handleCount; i++)
    {
        setValue(rrs->Handle[i], "k1", parValue);
        setTimeCourseSelectionList(rrs->Handle[i], selList.c_str());
        parValue  += increment;
    }

    int nrTimePoints = inputData.rSize();
    double startTime = inputData(0,0);
    double endTime 	 = inputData(inputData.rSize() - 1,0);

    //Simulate them using a pool of threads..
    jobsHandle = simulateJobsEx(rrs, threadCount, startTime, endTime, nrTimePoints, NULL, NULL, NULL);
    while(!areJobsFinished(jobsHandle))
    {
    	Log(lDebug)<<"Simulating.. ";
    };

	freeJobs(jobsHandle);

    vector<double> chiSquares;
    //Calculate chi squares for exp. data and simulated data
    for(int i = 0; i < handleCount; i++)
    {
    	RoadRunner *rri 		= (RoadRunner*) rrs->Handle[i];
		SimulationData simData 	= rri->getSimulationResult();

        //Make sure data dimensions agree..
        if(simData.dimension() != inputData.dimension())
        {
        	Log(lError)<<"Bad data dimensions..";
        	continue;
        }

        double chi = getChiSquare(simData, inputData);
		chiSquares.push_back(chi);
    }

    inputData.reSize(chiSquares.size(), 2);

    parValue = range*modelValue;
    for(int i = 0; i < chiSquares.size(); i++)
    {
        inputData(i, 0) = parValue;
    	inputData(i, 1) = chiSquares[i];
        parValue  += increment;
    }

	if(threadExitCB)
    {
		threadExitCB(mUserData);
    }
    freeRRInstances(rrs);
}

double getChiSquare(SimulationData& data1, SimulationData& data2)
{
	vector<double> chis;

    for(int col = 1; col < data1.cSize(); col++)   //Col 1 is time
    {
		chis.push_back(0);
		for(int row = 0; row < data1.rSize(); row++)
    	{
        	chis[col - 1] +=  pow((data1(row, col) - data2(row, col)), 2);
        }
    }

    double chi = 0;
	for(int i = 0; i < chis.size(); i++)
    {
    	chi += chis[i];
    }
    return chi;
}

}
