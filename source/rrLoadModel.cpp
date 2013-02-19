#pragma hdrstop
#include "rrLoadModel.h"
//---------------------------------------------------------------------------

namespace rr
{

LoadModel::LoadModel(vector<RoadRunner*> &rrInstances, const string& model, const int& nrThreads)
:
ThreadPool()
{
    //create nrThreads that can load SBML models
    for(int i = 0; i < nrThreads; i++)
    {
        LoadModelThread* lmThread = new LoadModelThread(model);
        mThreads.push_back(lmThread);
    }

    //The following will add jobs and get them done too..
    for(int i = 0; i < rrInstances.size(); i++)
    {
        addJob(rrInstances[i]);
    }
}


}