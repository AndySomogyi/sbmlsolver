#include <iostream>
#include "rrRoadRunner.h"
#include "rrLogger.h"
#include "rrUtils.h"
#include "rrException.h"
#include "rrLoadModel.h"
#include "rrSimulate.h"
#include "rrRoadRunnerList.h"
int main(int argc, char** argv)
{
	try
    {
        LogOutput::mLogToConsole = true;

        //Create some roadrunners
        const int nrOfRRInstances 	= 16;
        const int threadCount  		= 2;

        //Use a list of roadrunners
        RoadRunnerList rrs(nrOfRRInstances, "r:\\rrThreads");

        const string model = "r:\\models\\test_1.xml";

        //Load models..
        Log(lInfo)<<" ---------- LOADING/GENERATING MODELS ------";

        LoadModel loadModel(rrs, model, threadCount);
        loadModel.waitForAll();


        //Set parameters
        Log(lInfo)<<" ---------- SETTING PARAMETERS -------------";

        //Setup instances with different variables
        for(int i = 0; i < nrOfRRInstances; i++)
        {
            double val = rrs[i]->getValue("k1");
            rrs[i]->setValue("k1", val/(2.5*(i + 1)));
            rrs[i]->setNumPoints(500);
            rrs[i]->setTimeEnd(150);
            rrs[i]->setTimeCourseSelectionList("S1");
        }

        //Simulate
        Log(lInfo)<<" ---------- SIMULATING ---------------------";

        //Simulate them using a pool of threads..
        Simulate simulate(rrs, threadCount);
        simulate.waitForAll();

        //Write data to a file
        SimulationData allData;
        for(int i = nrOfRRInstances -1 ; i >-1 ; i--) //"Backwards" because bad plotting program..
        {
            RoadRunner* rr = rrs[i];
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
