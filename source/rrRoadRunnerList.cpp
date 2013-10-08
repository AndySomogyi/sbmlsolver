#ifdef USE_PCH
#include "rr_pch.h"
#endif
#pragma hdrstop
#include "rrRoadRunner.h"
#include "rrRoadRunnerList.h"
#include "rrStringUtils.h"
//---------------------------------------------------------------------------


namespace rr
{
RoadRunnerList::RoadRunnerList(const int& nrOfRRs, const string& tempFolder)
{
    string installFolder = "..";

    for(int i = 0; i < nrOfRRs; i++)
    {
        RoadRunner* rri = new RoadRunner("", tempFolder, joinPath(installFolder, "rr_support"));
        mRRs.push_back(rri);
        mRRs[i]->setTempFileFolder(tempFolder);
    }
}

RoadRunnerList::~RoadRunnerList()
{
    for(int i = 0; i < mRRs.size(); i++)
    {
        delete mRRs[i];
    }
}

unsigned int RoadRunnerList::count()
{
    return mRRs.size();
}

RoadRunner*    RoadRunnerList::operator[](const int& index)
{
    if(index > -1 && index < count())
    {
        return mRRs[index];
    }
    return NULL;
}
}
