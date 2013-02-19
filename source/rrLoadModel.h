#ifndef rrLoadModelH
#define rrLoadModelH
#include "rrThreadPool.h"
//---------------------------------------------------------------------------
namespace rr
{

class RR_DECLSPEC LoadModel : public ThreadPool
{
    public:
	    			LoadModel(vector<RoadRunner*> &rrInstances, const string& model, const int& nrThreads = 16);
};

}
#endif
