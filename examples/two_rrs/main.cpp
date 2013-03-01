#include <iostream>
#include "rrRoadRunner.h"
#include "rrLogger.h"
#include "rrUtils.h"
#include "rrException.h"

using namespace rr;

int main(int argc, char** argv)
{
	try
    {
        LogOutput::mLogToConsole = true;

        //Create some roadrunners
        const int instanceCount 	= 1;
        const int threadCount  		= 1;

        //Use a list of roadrunners
#if defined(WIN32)
const char* rootPath = "r:";
#else
const char* rootPath = "..";
#endif
        gLog.SetCutOffLogLevel(lInfo);
		string tmpFolder = JoinPath(rootPath, "temp");

        const string modelFile = JoinPath(rootPath, "models", "test_1.xml");

        //Load modelFiles..
        Log(lInfo)<<" ---------- LOADING/GENERATING MODELS ------";
        
        RoadRunner rr1, rr2;
        rr1.loadSBMLFromFile(modelFile);
        rr2.loadSBMLFromFile(modelFile);

        Log(lInfo)<<" ---------- SIMULATE ---------------------";

        Log(lInfo)<<"Data:"<<rr1.simulate();
        Log(lInfo)<<"Data:"<<rr2.simulate();
    }
    catch(const Exception& ex)
    {
    	Log(lError)<<"There was a  problem: "<<ex.getMessage();
    }

    return 0;
}

#pragma comment(lib, "roadrunner.lib")
#pragma comment(lib, "poco_foundation-static.lib")
#pragma comment(lib, "rr-libstruct-static.lib")
