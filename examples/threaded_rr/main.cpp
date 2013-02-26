#include <iostream>
#include "rrRoadRunner.h"
#include "rrRoadRunnerThread.h"
#include "rrLogger.h"

using namespace std;
using namespace rr;



class SimulateSBML : public RoadRunnerThread
{
	protected:
	public:
    					SimulateSBML(RoadRunner* rr, const string& modelFile) :
                        RoadRunnerThread(rr),
                        mModel(modelFile){}

		string			mModel;
    	                void worker()
    	                {
		                	Log(lInfo)<<"Starting SimulateSBML thread";

                            if(mRR)
                            {
	                        	std::cout << "Loading SBML" << std::endl;
                            	mRR->loadSBMLFromFile(mModel);
                                mRR->simulate();
                            }
                            else
                            {
                            	Log(lError)<<"RoadRunner Handle is NULL";
                            }

		                	Log(lInfo)<<"Exiting SimulateSBML thread";
    	                }
};

int main()
{
	RoadRunner lRR;

	LogOutput::mLogToConsole = true;
	gLog.SetCutOffLogLevel(lInfo);
    Log(lInfo)<<"Hello";
    SimulateSBML SimulateSBML(&lRR, "..\\models\\test_1.xml");

    SimulateSBML.start();
	while(SimulateSBML.isRunning())
    {
		Log(lInfo)<<"Running..";
    }

    SimulateSBML.join();

    return 0;
}

#pragma comment(lib, "roadrunner.lib")
#pragma comment(lib, "poco_foundation-static.lib")
