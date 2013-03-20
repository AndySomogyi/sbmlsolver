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

		const char* rootPath = "..";

        gLog.SetCutOffLogLevel(lInfo);
		string tmpFolder = JoinPath(rootPath, "temp");

        const string modelFile = JoinPath(rootPath, "models", "feedback.xml");
        const string modelLib  = JoinPath(rootPath, "temp", "feedback.dll");

        //Load modelFiles..
        Log(lInfo)<<" ---------- LOADING/GENERATING MODELS ------";

		RoadRunner rr1;
		rr1.loadSBMLFromFile(modelFile);	//This will generate a model DLL

		ModelSharedLibrary lib;
		

		if(lib.load(modelLib))
		{
			Log(lInfo)<<"Shared lib loaded succesfully...";
		}
		else
		{
			Log(lInfo)<<"Shared lib was NOT loaded succesfully...";
		}
     

    }
    catch(const Exception& ex)
    {
    	Log(lError)<<"There was a  problem: "<<ex.getMessage();
    }

    //Pause(true);
    return 0;
}
#if defined(CG_IDE) || defined(MSVS)
#pragma comment(lib, "roadrunner-static.lib")
#pragma comment(lib, "poco_foundation-static.lib")
#pragma comment(lib, "rr-libstruct-static.lib")
#endif
