#pragma hdrstop
#include "rrLoadModels.h"
//---------------------------------------------------------------------------

namespace rr
{
LoadModels::LoadModels(const int& nrThreads, const string& model, vector<RoadRunner*> &rrInstance) : ThreadPool()
{
    //create nrThreads that can load SBML models
    for(int i = 0; i < nrThreads; i++)
    {
        LoadModel* lmThread = new LoadModel(model);
        mThreads.push_back(lmThread);
    }

    //The following will add jobs and get them done too..
    for(int i = 0; i < rrInstance.size(); i++)
    {
        addJob(rrInstance[i]);
    }

}


}