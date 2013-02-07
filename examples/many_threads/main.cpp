#include <iostream>
#include "rrRoadRunner.h"
#include "rrRoadRunnerThread.h"
#include "rrLogger.h"

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
        void			start()
        				{
                        	mThread.start(*this);
                        }

    	                void worker()
    	                {
		                	Log(lInfo)<<"Starting loadSBML thread";

                            if(mRR)
                            {
                            	mRR->loadSBMLFromFile(mModel);
                            }
                            else
                            {
                            	Log(lError)<<"RoadRunner Handle is NULL";
                            }

		                	Log(lInfo)<<"Exiting loadSBML thread";
    	                }
};

int main(int argc, char** argv)
{
    string logFile = "RoadRunner.log";
    gLog.Init("test", lWarning);//, unique_ptr<LogFile>(new LogFile(logFile.c_str())));
	gLog.SetCutOffLogLevel(lError);

	//Create a thousand roadrunners
    const int nrOfRRInstances = 100;
	vector<RoadRunner*> rrs;

    for(int i = 0; i < nrOfRRInstances; i++)
    {
    	rrs.push_back(new RoadRunner);
        rrs[i]->setTempFileFolder("r:\\rrTemp");
    }

	LogOutput::mLogToConsole = true;
    Log(lInfo)<<"Hello";

    vector<LoadSBML*> loadSBMLs; //vector of many loadSBML threads

	//Load the same model into different instances
    int runAtOnce = 10;
    for(int i = 0; i < nrOfRRInstances; i++)
    {
    	loadSBMLs.push_back(new LoadSBML(rrs[i], "..\\models\\test_1.xml"));
   		loadSBMLs[i]->start();

        if((i+1) % runAtOnce == 0)
        {
        	for(int j = i - runAtOnce; j < i; j++)
            {
            	int threadNr = j + 1;
                loadSBMLs[threadNr]->join();
                cout<<"Thread "<<threadNr<<" exited..\n";
                delete loadSBMLs[threadNr];
                delete rrs[threadNr];
            }
        }
    }

    //Start each thread
    Pause(true);
    return 0;
}

#pragma comment(lib, "roadrunner.lib")
#pragma comment(lib, "poco_foundation-static.lib")
