#include <iostream>
#include "Poco/Mutex.h"
#include "Poco/ScopedLock.h"
#include "rrRoadRunner.h"
#include "rrRoadRunnerThread.h"
#include "rrLogger.h"
#include "rrUtils.h"
#include "rrException.h"

using namespace std;
using namespace rr;
using Poco::Mutex::ScopedLock;

class LoadSBML : public RoadRunnerThread
{
	protected:
		string			mModelFileName;

	public:
    					LoadSBML(const string& modelFile)
                        :
                        mModelFileName(modelFile)
                        {
                        	start();
                        }

    	void 			worker()
    	                {
	                        RoadRunner *rri = NULL;
							while(!mIsTimeToDie)
                            {
                                {
                                	ScopedLock lock(mJobsMutex);
                                	if(mJobs.size() == 0)
                                	{
                                    	Log(lDebug5)<<"Waiting for jobs in loadSBML worker";
                                    	mJobsCondition.wait(mJobsMutex);
                                	}

                                    if(mJobs.size() == 0 || mIsTimeToDie)
                                    {
                                    	return;	//ends the life of the thread..
                                    }
                                    else
                                    {
                                    	//Get a job
                                    	rri = mJobs.front();
                                    	mJobs.pop_front();
                                    }

                                }		//Causes the scoped lock to unlock

                                //Do the job
                                if(rri)
                                {
                                	Log(lInfo)<<"Loading model into instance: "<<rri->getInstanceID();
                                	rri->loadSBMLFromFile(mModelFileName);
                                }
                            }
                            Log(lDebug)<<"Exiting thread";
    	                }
};

class Simulate : public RoadRunnerThread
{
	public:
    					Simulate() :
                        RoadRunnerThread()
						{
                        	start();
                        }

    	void 			worker()
    	                {
	                        RoadRunner *rri = NULL;
							while(!mIsTimeToDie)
                            {
                                {	//Scope for the mutex lock...
                                	ScopedLock lock(mJobsMutex);
                                	if(mJobs.size() == 0)
                                	{
                                    	Log(lDebug5)<<"Waiting for jobs in Simulate worker";
                                    	mJobsCondition.wait(mJobsMutex);
                                	}

                                    if(mJobs.size() == 0 || mIsTimeToDie)
                                    {
                                    	return;	//ends the life of the thread..
                                    }
                                    else
                                    {
                                    	//Get a job
                                    	rri = mJobs.front();
                                    	mJobs.pop_front();
                                    }

                                }		//Causes the scoped lock to unlock

                                //Do the job
                                if(rri)
                                {
                                	Log(lInfo)<<"Simulating RR instance: "<<rri->getInstanceID();
                                	if(!rri->simulate2())
                                    {
                                    	Log(lError)<<"Failed simulating instance: "<<rri->getInstanceID();
                                    }
                                }

                            }
                            Log(lDebug)<<"Exiting thread";
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
    const int nrOfRRInstances = 1000;
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
    	loadSBML.push_back(new LoadSBML("r:\\models\\test_1.xml"));
        simulate.push_back(new Simulate());
    }

    //Populate threads with jobs. Observe the threads starts working as soon it gets a job..
    int instance = 0;
    do
    {
        for(int j = 0; j < threadCount; j++)
        {
            if(instance < nrOfRRInstances)
            {
                loadSBML[j]->addJob(rrInstance[instance]);
                instance++;
            }
        }
    }
    while(instance < nrOfRRInstances);


    //Check that all jobs been done
    for(int j = 0; j < threadCount; j++)
    {
    	do
        {
            //Sleep(10);

        }while(loadSBML[j]->getNrOfJobsInQueue() != 0);
    }

    //All jobs done.. exit the threads..
    for(int j = 0; j < threadCount; j++)
    {
		loadSBML[j]->exit();
    }

    //Make sure all threads have exited...
    for(int j = 0; j < threadCount; j++)
    {
		loadSBML[j]->join();
    }

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

    //Simulate them using threads..
    instance = 0;
    do
    {
        for(int j = 0; j < threadCount; j++)
        {
            if(instance < nrOfRRInstances)
            {
                simulate[j]->addJob(rrInstance[instance]);
                instance++;
            }
        }
    }
    while(instance < nrOfRRInstances);


    //Check that all jobs been done
    for(int j = 0; j < threadCount; j++)
    {
    	do
        {
        }while(simulate[j]->getNrOfJobsInQueue() != 0);
    }

    //All jobs done.. exit the threads..
    for(int j = 0; j < threadCount; j++)
    {
		simulate[j]->exit();
    }

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
