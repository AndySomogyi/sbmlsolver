#include <iostream>
#include "rrRoadRunner.h"
#include "rrLogger.h"
#include "rrUtils.h"
#include "rrException.h"
#include "rrThreadPool.h"

int main(int argc, char** argv)
{
	try
    {
    string logFile = "RoadRunner.log";
    gLog.Init("test", lInfo);
	gLog.SetCutOffLogLevel(lInfo);
	LogOutput::mLogToConsole = true;

	//Create some roadrunners
    const int nrOfRRInstances = 50;
	vector<RoadRunner*> rrInstances;

    for(int i = 0; i < nrOfRRInstances; i++)
    {
    	rrInstances.push_back(new RoadRunner);
        rrInstances[i]->setTempFileFolder("r:\\rrThreads");
    }

    const string model = "r:\\models\\test_1.xml";

	//Threads ..
    int threadCount = 16;
    LoadSBMLThreadPool loadSBML(threadCount, model, rrInstances);

    loadSBML.waitForAll();

    //Simulate
    Log(lInfo)<<" ---------- SIMULATING -------------";

    //Setup instances with different variables
    for(int i = 0; i < nrOfRRInstances; i++)
    {
    	double val = rrInstances[i]->getValue("k1");
	    rrInstances[i]->setValue("k1", val/(2.5*(i + 1)));
        rrInstances[i]->setNumPoints(50);
        rrInstances[i]->setTimeEnd(15);
        rrInstances[i]->setTimeCourseSelectionList("S1");
    }

    //Simulate them using a pool of threads..
    SimulateSBMLThreadPool simulateSBML(threadCount, rrInstances);

    //Wait for all jobs
    simulateSBML.waitForAll();


    //Write data to a file
    SimulationData allData;
    for(int i = nrOfRRInstances -1 ; i >-1 ; i--)
    {
		RoadRunner* rr = rrInstances[i];
        SimulationData data = rr->getSimulationResult();
        allData.append(data);
    }


    allData.writeTo("r:\\allData.dat");
    }
    catch(const Exception& ex)
    {
    	Log(lError)<<"There was a  problem: "<<ex.getMessage();
    }

    return 0;
}

#pragma comment(lib, "roadrunner.lib")
#pragma comment(lib, "poco_foundation-static.lib")
