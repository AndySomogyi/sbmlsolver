#ifndef rrLoadModelsH
#define rrLoadModelsH
#include "rrThreadPool.h"
//---------------------------------------------------------------------------
namespace rr
{

class LoadModels : public ThreadPool
{
    public:
	    			LoadModels(const int& nrThreads, const string& model, vector<RoadRunner*> &rrInstance);
};

}
#endif
