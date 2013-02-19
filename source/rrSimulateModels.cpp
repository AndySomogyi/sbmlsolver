#pragma hdrstop
#include "rrSimulateModels.h"
//---------------------------------------------------------------------------

namespace rr
{

SimulateModels::SimulateModels(const int& nrThreads, vector<RoadRunner*> &rrInstance) : ThreadPool()
{
    //create nrThreads that can load SBML models
    for(int i = 0; i < nrThreads; i++)
    {
        Simulate* sThread = new Simulate();
        mThreads.push_back(sThread);
    }

    //The following will add jobs and get them done too..
    for(int i = 0; i < rrInstance.size(); i++)
    {
        addJob(rrInstance[i]);
    }
}

}
