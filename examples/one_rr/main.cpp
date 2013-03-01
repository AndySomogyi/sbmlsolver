#include <iostream>
#include "rrRoadRunner.h"
#include "rrException.h"
#include "rrUtils.h"
#include "rrLogger.h"
using namespace rr;

int main(int argc, char** argv)
{
	const char* rootPath = "..";

	try
    {
        LogOutput::mLogToConsole = true;
        gLog.SetCutOffLogLevel(lInfo);
		string tmpFolder = JoinPath(rootPath, "temp");

        const string modelFile = JoinPath(rootPath, "models", "test_1.xml");

        //Load modelFiles..
        Log(lInfo)<<" ---------- LOADING/GENERATING MODELS ------";

        RoadRunner rr1;
        if(!rr1.loadSBMLFromFile(modelFile))
        {
			Log(lError)<<"There was a problem loading model in file: "<<modelFile;
            throw(Exception("Bad things in loadSBMLFromFile function"));
        }

        Log(lInfo)<<" ---------- SIMULATE ---------------------";

        Log(lInfo)<<"Data:"<<rr1.simulate();

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
