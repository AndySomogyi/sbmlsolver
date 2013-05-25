#ifndef rrLoadModelH
#define rrLoadModelH
#include "rrThreadPool.h"
#include "rrRoadRunnerList.h"
//---------------------------------------------------------------------------
namespace rr
{

class RR_DECLSPEC LoadModel : public ThreadPool
{
    public:
	    			LoadModel(RoadRunnerList& rrs, const string& sbml, const int& nrThreads = 16, bool reCompile = true);
				   ~LoadModel();
};

}
#endif
