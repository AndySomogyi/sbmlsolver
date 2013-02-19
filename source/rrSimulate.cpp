#pragma hdrstop
#include "rrSimulate.h"
//---------------------------------------------------------------------------

namespace rr
{

Simulate::Simulate(RoadRunnerList& rrs, const int& nrThreads)
:
ThreadPool()
{
    //create nrThreads that can load SBML models
    for(int i = 0; i < nrThreads; i++)
    {
        SimulateThread* sThread = new SimulateThread();
        mThreads.push_back(sThread);
    }

    //The following will add jobs and get them done too..
    for(int i = 0; i < rrs.count(); i++)
    {
        addJob(rrs[i]);
    }
}

}
