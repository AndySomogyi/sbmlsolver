#include <iostream>
#include "rrRoadRunner.h"
#include "rrLogger.h"
#include "rrUtils.h"
#include "rrException.h"
#include "rrLoadModel.h"
#include "rrSimulate.h"
#include "rrSimulateThread.h"
#include "rrRoadRunnerList.h"

using namespace rr;
int main(int argc, char** argv)
{
	try
    {
        LogOutput::mLogToConsole = true;
       	gLog.SetCutOffLogLevel(lDebug);
        //Create some roadrunners
        const int 	instanceCount 	= 1;
        const int 	threadCount  	= 1;
		const char* rootPath 		= "..";

		string tmpFolder = joinPath(rootPath, "temp");
        //Use a list of roadrunners
        RoadRunnerList rrs(instanceCount, tmpFolder);

        const string modelFile = joinPath(rootPath, "models", "test_1.xml");
		string sbml = getFileContent(modelFile);
        //Load modelFiles..
        Log(lInfo)<<" ---------- LOADING/GENERATING MODELS ------";


        LoadModel loadModel(rrs, sbml, threadCount);
        loadModel.waitForFinish();

      	//Set parameters
        Log(lInfo)<<" ---------- SETTING UP PARAMETERS -------------";

        //Setup instances with different variables
        for(int i = 0; i < instanceCount; i++)
        {
            rrs[i]->setValue("k1", 1./(2.5*(i + 1)));
            rrs[i]->setNumPoints(500);
            rrs[i]->setTimeEnd(150);
            rrs[i]->setTimeCourseSelectionList("S1");
        }

        //Simulate
        Log(lInfo)<<" ---------- SIMULATING ---------------------";

//        //Simulate them using a pool of threads..
        Simulate simulate(rrs, threadCount);
        simulate.waitForFinish();

//		//Thread by thread
//		for(int i = 0; i < rrs.count(); i++)
//        {
//			SimulateThread sim(rrs[i]);
//            sim.start();
//            sim.waitForFinish();
//        }
        //Write data to a file
        if(instanceCount < 500)
        {
            RoadRunnerData allData;
            for(int i = instanceCount -1 ; i >-1 ; i--) //"Backwards" because bad plotting program..
            {
                RoadRunner* rr = rrs[i];
                RoadRunnerData data = rr->getSimulationResult();
                allData.append(data);
            }

        	allData.writeTo(joinPath(rootPath, "temp", "allData.dat"));
        }
        else
        {
        	Log(lInfo)<<"Not writing out that much data...";
        }
    }
    catch(const Exception& ex)
    {
    	Log(lError)<<"There was a  problem: "<<ex.getMessage();
    }

    return 0;
}

#if defined(CG_IDE)
#pragma comment(lib, "roadrunner.lib")
#pragma comment(lib, "poco_foundation-static.lib")
#endif
