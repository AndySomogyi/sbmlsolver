#include <iostream>
#include "rrRoadRunner.h"
#include "rrRoadRunnerThread.h"
#include "rrLogger.h"
#include "rrUtils.h"
#include "rrException.h"
using namespace std;
using namespace rr;

class LoadSBML : public RoadRunnerThread
{
	protected:
		string			mModelFileName;

	public:
    					LoadSBML(RoadRunner* rr, const string& modelFile)
                        :
                        RoadRunnerThread(rr),
                        mModelFileName(modelFile){}

    	void 			worker()
    	                {
                            if(mRR)
                            {
                            	mRR->loadSBMLFromFile(mModelFileName);
                            }
                            else
                            {
                            	Log(lError)<<"RoadRunner Handle is NULL";
                            }
    	                }
};

class Simulate : public RoadRunnerThread
{
	public:
    					Simulate(RoadRunner* rr) :
                        RoadRunnerThread(rr)
						{}

    	void 			worker()
    	                {
                            mIsDone = false;
                            if(mRR)
                            {
                            	if(!mRR->simulate2())
                                {
	                            	Log(lError)<<"Simulation failure";
                                }
                            }
                            else
                            {
                            	Log(lError)<<"RoadRunner Handle is NULL";
                            }
                            mIsDone = true;
    	                }
};

int main(int argc, char** argv)
{
	try
    {
    string logFile = "RoadRunner.log";
    gLog.Init("test", lDebug);
	gLog.SetCutOffLogLevel(lInfo);
	LogOutput::mLogToConsole = true;

	//Create many roadrunners
    const int nrOfRRInstances = 120;
	vector<RoadRunner*> rrInstance;

    for(int i = 0; i < nrOfRRInstances; i++)
    {
    	rrInstance.push_back(new RoadRunner);
        rrInstance[i]->setTempFileFolder("r:\\rrThreads");
    }

    //Threads ..
    int threadCount = 16;
    vector<LoadSBML*> loadSBML;
   	vector<Simulate*> simulate;

	//Create threads to load and simulate models
    for(int i = 0; i < threadCount; i++)
    {
    	loadSBML.push_back(new LoadSBML(NULL, "..\\models\\test_1.xml"));
        simulate.push_back(new Simulate(NULL));
    }

    //Run threadCount threads
    int instance = 0;
    do
    {
        for(int j = 0; j < threadCount; j++)
        {
            if(instance < nrOfRRInstances)
            {
                loadSBML[j]->setRRInstance(rrInstance[instance]);
                loadSBML[j]->start();
                instance++;
            }
        }

        //Wait for the threads..
        for(int j = 0; j < threadCount; j++)
        {
            loadSBML[j]->join();
        }

    }
    while(instance < nrOfRRInstances);

    //Simulate
    Log(lInfo)<<" ---------- SIMULATING -------------";
    instance = 0;
    do
    {
     	for(int j = 0; j < threadCount; j++)
        {
        	if(instance  < nrOfRRInstances)
            {
	            simulate[j]->setRRInstance(rrInstance[instance]);
                instance++;
            	double val = simulate[j]->getRRInstance()->getValue("k1");
	            simulate[j]->getRRInstance()->setValue("k1", val/(2.5*( j + instance + 1)));
                simulate[j]->getRRInstance()->setNumPoints(500);
                simulate[j]->getRRInstance()->setTimeEnd(150);
                simulate[j]->getRRInstance()->setTimeCourseSelectionList("S1");
        		simulate[j]->start();
            }
        }

     	for(int j = 0; j < threadCount; j++)
        {
        	simulate[j]->join();
        }
    }
    while(instance < nrOfRRInstances);

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
