#ifndef rrLoadModelFromFileH
#define rrLoadModelFromFileH
#include "rrThreadPool.h"
#include "rrRoadRunnerList.h"
//---------------------------------------------------------------------------
namespace rr
{

class RR_DECLSPEC LoadModelFromFile : public ThreadPool
{
    public:
	    			LoadModelFromFile(RoadRunnerList& rrs, const string& modelFName, const int& nrThreads = 16);
				   ~LoadModelFromFile();
};

}
#endif
