#include <iostream>
#include "rrLoadModelThread.h"
#include "rrSimulateThread.h"
#include "rrLogger.h"
#include "rrRoadRunner.h"
#include "rrUtils.h"

using namespace std;
using namespace rr;


int main()
{
	RoadRunner lRR;
	LogOutput::mLogToConsole = true;
	gLog.SetCutOffLogLevel(lDebug);

    Log(lInfo)<<"======================";
    Log(lInfo)<<lRR.getInfo();
    Log(lInfo)<<"======================";

    string model = joinPath("..", "models", "test_1.xml");
    LoadModelThread load(model, false, &lRR);

    load.start();
    load.waitForFinish();

    Sleep(1000);
    SimulateThread sim(&lRR, false);
	sim.start();
    sim.waitForFinish();

    Log(lInfo)<<lRR.getSimulationResult();

    for(int i = 0; i < 2; i++)
    {
    	lRR.setNumPoints(10);
	    sim.addJob(&lRR);
    	sim.start();
	    sim.waitForFinish();
	    Log(lInfo)<<lRR.getSimulationResult();

    }
    return 0;
}

#if defined(CG_IDE)
#pragma comment(lib, "roadrunner.lib")
#pragma comment(lib, "poco_foundation-static.lib")
#pragma comment(lib, "rr-libstruct-static.lib")
#endif
