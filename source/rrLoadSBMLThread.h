#ifndef rrLoadSBMLThreadH
#define rrLoadSBMLThreadH
#include "Poco/Thread.h"
#include "Poco/Mutex.h"
#include "Poco/ScopedLock.h"
#include "rrExporter.h"
#include "rrRoadRunnerThread.h"
//---------------------------------------------------------------------------

namespace rr
{

class RR_DECLSPEC LoadSBML : public RoadRunnerThread
{
	protected:
		string			mModelFileName;

	public:
    					LoadSBML(const string& modelFile);
    	void 			worker();

};

class Simulate : public RoadRunnerThread
{
	public:
    					Simulate();
    	void 			worker();
};


}
#endif
