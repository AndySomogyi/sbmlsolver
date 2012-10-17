#include <iostream>
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "rrRoadRunner.h"
#include "rrLogger.h"

using namespace std;
using namespace rr;

class RoadRunnerThreadTask : public Poco::Runnable
{
	protected:
	    Poco::Thread 		mThread;
    	RoadRunner*			mRR;


    public:
	    					RoadRunnerThreadTask(RoadRunner* rr) : mRR(rr){}
    	virtual void        run() { worker();}		//This is called from runnable::start(this)

		virtual void        worker() = 0;
        void				join(){mThread.join();}


};

class LoadSBML : public RoadRunnerThreadTask
{
	protected:


	public:
    					LoadSBML(RoadRunner* rr, const string& modelFile) :
                        RoadRunnerThreadTask(rr),
                        mModel(modelFile){}

		string			mModel;
        void			start()
        				{
                        	mThread.start(*this);
                        }

    	                void worker()
    	                {
		                	Log(lInfo)<<"Starting loadSBML thread";
                        	std::cout << "Loading SBML" << std::endl;
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
    gLog.Init("test", gLog.GetLogLevel(), unique_ptr<LogFile>(new LogFile(logFile.c_str())));

	RoadRunner lRR;

//	lRR.Set

	LogOutput::mLogToConsole = true;
	gLog.SetCutOffLogLevel(lDebug3);
    Log(lInfo)<<"Hello";
    LoadSBML loadSBML(&lRR, "test_1.xml");


    loadSBML.run();

    loadSBML.join();
    return 0;
}

