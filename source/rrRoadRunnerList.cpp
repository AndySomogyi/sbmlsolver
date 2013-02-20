#pragma hdrstop
#include "rrRoadRunner.h"
#include "rrRoadRunnerList.h"
//---------------------------------------------------------------------------


namespace rr
{
RoadRunnerList::RoadRunnerList(const int& nrOfRRs, const string& tempFolder)
{
    for(int i = 0; i < nrOfRRs; i++)
    {
    	mRRs.push_back(new RoadRunner);
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

RoadRunner*	RoadRunnerList::operator[](const int& index)
{
	if(index > -1 && index < count())
    {
		return mRRs[index];
    }
    return NULL;
}
}
