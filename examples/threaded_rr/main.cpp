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
	                        	std::cout << "Loading SBML" << std::endl;
                            	mRR->loadSBMLFromFile(mModel);
                                Sleep(100);
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
    gLog.Init("test");

	RoadRunner lRR;

	LogOutput::mLogToConsole = true;
	gLog.SetCutOffLogLevel(lDebug3);
    Log(lInfo)<<"Hello";
    LoadSBML loadSBML(&lRR, "..\\models\\test_1.xml");


    loadSBML.start();
	while(loadSBML.isRunning())
    {
		Log(lInfo)<<"Running..";
    }

    loadSBML.join();
    return 0;
}

#pragma comment(lib, "roadrunner.lib")
#pragma comment(lib, "poco_foundation-static.lib")
