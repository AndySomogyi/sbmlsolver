#pragma hdrstop
#include "rrRoadRunnerData.h"
#include "rrLogger.h"
#include "full_space_fit_thread.h"
#include "rrNoise.h"
#include "full_space_minimization.h"
#include "../../Wrappers/C/rrc_api.h"
#include "../../Wrappers/C/rrc_support.h"

//---------------------------------------------------------------------------

namespace fullSpaceFit
{
double getRSquare(RoadRunnerData& data1, RoadRunnerData& data2);

using namespace rr;
FullSpaceFitThread::FullSpaceFitThread(FullSpaceMinimization& host)
:
threadEnterCB(NULL),
threadExitCB(NULL),
mUserData(NULL),
mTheHost(host),
mResult(mTheHost.mResult)
{}

void FullSpaceFitThread::assignCallBacks(ThreadCB fn1, ThreadCB fn2, void* userData)
{
	threadEnterCB 	= fn1;
	threadExitCB  	= fn2;
    mUserData 		= userData;
}

void FullSpaceFitThread::start(RoadRunnerData* inputData)
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

    RoadRunnerData& inputData = *(mInputData);

	//Allocate
    int handleCount = mTheHost.mStepsPerDimension.getValue();
    int threadCount = mTheHost.mNumberOfThreads.getValue();

    if(!handleCount || !threadCount)
    {
        Log(lError)<<"No handles or no threads.. error!";
        return;
    }
    RRInstanceListHandle rrs = createRRInstances(handleCount);

	if(!setTempFolder(rrs->Handle[0], mTheHost.getTempFolder().c_str()))
    {
        Log(lError)<<"Failed setting up temporary folder";
    	return;
    }

	//loadSBML models in threads
    RRJobsHandle jobsHandle = loadSBMLJobsEx(rrs, mTheHost.getSBML().c_str(), threadCount, false);
    waitForJobs(jobsHandle);
    freeJobs(jobsHandle);

	//Prepare to run simulations
    string paraToFit = mTheHost.getParameterToFit().getValueAsString();
	double modelValue;
    if(!getValue(rrs->Handle[0], paraToFit.c_str(), &modelValue))
    {
		//Cleanup and exit...
        return;
    }

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

    double range = (mTheHost.mParameterSweepRange.getValue()/100.0) / 2.0;
	double parValue = range * modelValue;  //Start of 'sweep'
    double increment = 2.0*(modelValue - parValue) / handleCount;

    vector<double> paraValues;
	//Setup roadrunners for simulations
    for(int i = 0; i < handleCount; i++)
    {
		paraValues.push_back(parValue);
        setValue(rrs->Handle[i], paraToFit.c_str(), parValue);
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

    vector< pair<double, double> > RSquares;
    //Calculate chi squares for exp. data and simulated data
    for(int i = 0; i < handleCount; i++)
    {
    	RoadRunner *rri 		= (RoadRunner*) rrs->Handle[i];
		RoadRunnerData simData 	= rri->getSimulationResult();

        //Make sure data dimensions agree..
        if(simData.dimension() != inputData.dimension())
        {
        	Log(lError)<<"Bad data dimensions..";
        	continue;
        }

        double R2 = getRSquare(simData, inputData);
        pair<double, double> aVal(paraValues[i], R2);
		RSquares.push_back(aVal);
        Log(lInfo)<<"R Square, ParaValue "<<aVal.second <<":"<<aVal.first;
    }

    //Find minimum
    double min = 1e9;
    for(int i = 0; i < RSquares.size(); i++)
    {
    	double val = RSquares[i].second;
    	if(val < min)
        {
        	min = val;
        }
    }


    Parameter<double> para(mTheHost.getParameterToFit().getValue(), 0, "");
    para.setValue(min);
//    mResult.addParameter();

	if(threadExitCB)
    {
		threadExitCB(mUserData);
    }

    freeRRInstances(rrs);
}

double getRSquare(RoadRunnerData& observedData, RoadRunnerData& modelData)
{
	vector<double> rSquares;

    for(int col = 1; col < observedData.cSize(); col++)   //Col 1 is time
    {
		rSquares.push_back(0);
		for(int row = 0; row < observedData.rSize(); row++)
    	{
        	rSquares[col - 1] +=  pow((observedData(row, col) - modelData(row, col)), 2);
        }
    }

    double r2 = 0;
	for(int i = 0; i < rSquares.size(); i++)
    {
    	r2 += rSquares[i];
    }
    return r2;
}

}
