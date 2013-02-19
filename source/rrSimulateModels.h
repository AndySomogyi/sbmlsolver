#ifndef rrSimulateModelsH
#define rrSimulateModelsH
//---------------------------------------------------------------------------

#include "rrThreadPool.h"
namespace rr
{

class RR_DECLSPEC SimulateModels : public ThreadPool
{
    public:
	    			SimulateModels(const int& nrThreads, vector<RoadRunner*> &rrInstance);

};

}
#endif
