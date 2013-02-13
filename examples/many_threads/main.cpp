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
    					LoadSBML(RoadRunner* rr, const string& modelFile) :
                        RoadRunnerThread(rr),
                        mModelFileName(modelFile){}

    	                void worker()
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

    	                void worker()
    	                {
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
    	                }
};

int main(int argc, char** argv)
{
	try
    {
    string logFile = "RoadRunner.log";
    gLog.Init("test", lDebug);
	gLog.SetCutOffLogLevel(lInfo);

	//Create many roadrunners
    const int nrOfRRInstances = 120;
	vector<RoadRunner*> rrs;

    for(int i = 0; i < nrOfRRInstances; i++)
    {
    	rrs.push_back(new RoadRunner);
        rrs[i]->setTempFileFolder("r:\\rrThreads");

        //Give the instance a "name"
//        rrs[i]->setInstanceName("rrInstance" + ToString(i));
    }

	LogOutput::mLogToConsole = true;
    Log(lInfo)<<"Hello";

    vector<LoadSBML*> loadSBML;
   	vector<Simulate*> simulate;

	//Load the same model into different instances
    for(int i = 0; i < nrOfRRInstances; i++)
    {
    	loadSBML.push_back(new LoadSBML(rrs[i], "..\\models\\test_1.xml"));
        loadSBML[i]->setThreadName(ToString(i));
        simulate.push_back(new Simulate(rrs[i]));
    }

    int threadCount = 5;
    for(int i = 0; i < nrOfRRInstances; i+=threadCount)
    {
     	for(int j = 0; j < threadCount; j++)
        {
        	if((j + i) < nrOfRRInstances)
            {
        		loadSBML[i+j]->start();
            }
        }

     	for(int j = 0; j < threadCount; j++)
        {
        	if((j + i) < nrOfRRInstances)
            {
        		loadSBML[i+j]->join();
            }
        }
    }

    //Simulate
    for(int i = 0; i < nrOfRRInstances; i+=threadCount)
    {
     	for(int j = 0; j < threadCount; j++)
        {
        	if((j + i) < nrOfRRInstances)
            {
            	double val = simulate[i+j]->getRRInstance()->getValue("k1");
	            simulate[i+j]->getRRInstance()->setValue("k1", val/( j + i + 1));
                val = simulate[i+j]->getRRInstance()->getValue("k1");
                simulate[i+j]->getRRInstance()->setNumPoints(500);
                simulate[i+j]->getRRInstance()->setTimeEnd(150);
                simulate[i+j]->getRRInstance()->setTimeCourseSelectionList("S1");
        		simulate[i+j]->start();
            }
        }

     	for(int j = 0; j < threadCount; j++)
        {
        	if((j + i) < nrOfRRInstances)
            {
        		simulate[i+j]->join();
            }
        }
    }

    //Write data to a file
    SimulationData allData;
    for(int i = nrOfRRInstances -1 ; i >-1 ; i--)
    {
		RoadRunner* rr = rrs[i];
        SimulationData data = rr->getSimulationResult();
        allData.append(data);
    }

    allData.writeTo("r:\\allData.dat");
    }
    catch(const Exception& ex)
    {
    	Log(lError)<<"There was a  problem: "<<ex.getMessage();
    }

//    Pause(true);
    return 0;
}

#pragma comment(lib, "roadrunner.lib")
#pragma comment(lib, "poco_foundation-static.lib")
