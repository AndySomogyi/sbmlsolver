#ifndef telSimulateH
#define telSimulateH
//---------------------------------------------------------------------------
#include "telRoadRunnerList.h"
#include "telThreadPool.h"

namespace tlp
{

class JOBS_DECLSPEC Simulate : public ThreadPool
{
    public:
                        Simulate(RoadRunnerList& rrs, const int& nrThreads = 16);
                       ~Simulate();

};

}
#endif
