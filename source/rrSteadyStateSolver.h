#ifndef rrSteadyStateSolverH
#define rrSteadyStateSolverH
#include "rrObject.h"
#include "rrCapability.h"
//---------------------------------------------------------------------------

namespace rr
{

class RR_DECLSPEC SteadyStateSolver : public rrObject
{
	protected:
    	Capability				  	mCapability;

    public:
	    		 					SteadyStateSolver(const string& funcName, const string& description);
		virtual 				   ~SteadyStateSolver();
        virtual double 				solve(const vector<double>& yin) = 0;
        virtual Capability&			getCapability() = 0;
};

}
#endif
