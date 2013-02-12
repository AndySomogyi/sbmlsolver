#include <iostream>
#include "rrRoadRunner.h"
#include "rrRoadRunnerThread.h"
#include "rrLogger.h"
#include "rrUtils.h"

using namespace std;
using namespace rr;

class LoadSBML : public RoadRunnerThread
{
	protected:
	public:
    					LoadSBML(RoadRunner* rr, const string& modelFile) :
                        RoadRunnerThread(rr),
                        mModel(modelFile){}

		string			mModel;

    	                void worker()
    	                {
                        	//Log(lInfo)<<"======================= Running thread:"<<mThreadName<<endl;

                            if(mRR)
                            {
                            	mRR->loadSBMLFromFile(mModel);
                                mRR->simulate();
                            }
                            else
                            {
                            	Log(lError)<<"RoadRunner Handle is NULL";
                            }

		                	//Log(lInfo)<<"======================= Exiting thread"<<mThreadName<<endl;;
    	                }
};

int main(int argc, char** argv)
{
    string logFile = "RoadRunner.log";
    gLog.Init("test", lDebug);//, unique_ptr<LogFile>(new LogFile(logFile.c_str())));
	gLog.SetCutOffLogLevel(lInfo);

	//Create a thousand roadrunners
    const int nrOfRRInstances = 1000;
	vector<RoadRunner*> rrs;

    for(int i = 0; i < nrOfRRInstances; i++)
    {
    	rrs.push_back(new RoadRunner);
        rrs[i]->setTempFileFolder("r:\\rrThreads");
    }

	LogOutput::mLogToConsole = true;
    Log(lInfo)<<"Hello";

    vector<LoadSBML*> loadSBMLs; //vector of many loadSBML threads

	//Load the same model into different instances

    for(int i = 0; i < nrOfRRInstances; i++)
    {
    	loadSBMLs.push_back(new LoadSBML(rrs[i], "..\\models\\test_1.xml"));
        loadSBMLs[i]->setThreadName(ToString(i));
    }

    int threadCount = 4;
    for(int i = 0; i < nrOfRRInstances; i+=threadCount)
    {
     	for(int j = 0; j < threadCount; j++)
        {
        	if((j + i) < nrOfRRInstances)
            {
        		loadSBMLs[i+j]->start();
            }
        }

     	for(int j = 0; j < threadCount; j++)
        {
        	if((j + i) < nrOfRRInstances)
            {
        		loadSBMLs[i+j]->join();
            }
        }

    }

    //Start each thread
    Pause(true);
    return 0;
}

#pragma comment(lib, "roadrunner.lib")
#pragma comment(lib, "poco_foundation-static.lib")
