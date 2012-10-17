#ifndef rrSimulateThreadH
#define rrSimulateThreadH
#include "mtkThread.h"

//---------------------------------------------------------------------------
class TMForm;

namespace rr
{
class RoadRunner;


class SimulateThread : public mtkThread
{
	protected:
		RoadRunner			   *mRR;
        TMForm                 *mHost;

    public:
	    						SimulateThread(RoadRunner* rr, TMForm* mainForm);
                   	   		   ~SimulateThread();
		void					AssignRRInstance(RoadRunner* rr);

		void					Worker(); 			//Worker function where it all happens


};

}
#endif
