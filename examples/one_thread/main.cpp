#include <iostream>
#include "rrLoadModelThread.h"
#include "rrSimulateThread.h"
#include "rrLogger.h"
#include "rrRoadRunner.h"

using namespace std;
using namespace rr;


int main()
{
	RoadRunner lRR;
	LogOutput::mLogToConsole = true;
	gLog.SetCutOffLogLevel(lInfo);

    Log(lInfo)<<"======================";
    Log(lInfo)<<lRR.getInfo();
    Log(lInfo)<<"======================";
    
    LoadModelThread load(JoinPath("..", "models", "test_1.xml"), &lRR);

    load.start();
    load.waitForFinish();

    SimulateThread sim(&lRR);
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
#if defined(__CODEGEARC__)
#pragma comment(lib, "roadrunner.lib")
#pragma comment(lib, "poco_foundation-static.lib")
#endif
