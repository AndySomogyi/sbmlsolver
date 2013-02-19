#ifndef rrSimulateH
#define rrSimulateH
//---------------------------------------------------------------------------

#include "rrThreadPool.h"
namespace rr
{

class RR_DECLSPEC Simulate : public ThreadPool
{
    public:
    	Simulate(vector<RoadRunner*> &rrInstance, const int& nrThreads = 16);

};

}
#endif
