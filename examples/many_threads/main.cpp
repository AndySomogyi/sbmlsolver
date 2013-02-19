#include <iostream>
#include "rrRoadRunner.h"
#include "rrLoadSBMLThread.h"
#include "rrLogger.h"
#include "rrUtils.h"
#include "rrException.h"
#include "rrThreadPool.h"

class LoadSBMLThreadPool : public ThreadPool
{
	private:

    public:
	    			LoadSBMLThreadPool(const int& nrThreads, const string& model) : ThreadPool()
                    {
                    	//create nrThreads that can load SBML models
                        for(int i = 0; i < nrThreads; i++)
                        {
                            mThreads.push_back(new LoadSBML(model));
                        }
                    }
};

class SimulateSBMLThreadPool : public ThreadPool
{
	private:

    public:
	    			SimulateSBMLThreadPool(const int& nrThreads, 	vector<RoadRunner*> &rrInstance) : ThreadPool()
                    {
                    	//create nrThreads that can load SBML models
                        for(int i = 0; i < nrThreads; i++)
                        {
                            mThreads.push_back(new Simulate());
                        }
                        for(int i = 0; i < rrInstance.size(); i++)
                        {
                        	addJob(rrInstance[i]);
                        }
                    }
};


int main(int argc, char** argv)
{
	try
    {
    string logFile = "RoadRunner.log";
    gLog.Init("test", lInfo);
	gLog.SetCutOffLogLevel(lInfo);
	LogOutput::mLogToConsole = true;

	//Create many roadrunners
    const int nrOfRRInstances = 4;
	vector<RoadRunner*> rrInstance;

    for(int i = 0; i < nrOfRRInstances; i++)
    {
    	rrInstance.push_back(new RoadRunner);
        rrInstance[i]->setTempFileFolder("r:\\rrThreads");
    }

    const string model = "r:\\models\\test_1.xml";
	//Threads ..

    int threadCount = 8;
    LoadSBMLThreadPool loadSBML(threadCount, model);

    //Populate threads with jobs. Observe the threads starts working as soon it gets a job..
    int instance = 0;
    do
    {
        for(int j = 0; j < threadCount; j++)
        {
            if(instance < nrOfRRInstances)
            {
                loadSBML.addJob(rrInstance[instance]);
                instance++;
            }
        }
    }
    while(instance < nrOfRRInstances);


    loadSBML.waitForAll();



    //Simulate
    Log(lInfo)<<" ---------- SIMULATING -------------";

    //Setup instances with different variables
    for(int i = 0; i < nrOfRRInstances; i++)
    {
    	cout<<i<<endl;
    	double val = rrInstance[i]->getValue("k1");
	    rrInstance[i]->setValue("k1", val/(2.5*(i + 1)));
        rrInstance[i]->setNumPoints(500);
        rrInstance[i]->setTimeEnd(150);
        rrInstance[i]->setTimeCourseSelectionList("S1");
    }

    //Simulate them using a pool of threads..
    SimulateSBMLThreadPool simulateSBML(threadCount, rrInstance);

    //Wait for all jobs
    simulateSBML.waitForAll();


    //Write data to a file
    SimulationData allData;
    for(int i = nrOfRRInstances -1 ; i >-1 ; i--)
    {
		RoadRunner* rr = rrInstance[i];
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
