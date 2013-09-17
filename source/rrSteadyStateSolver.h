#ifndef rrSteadyStateSolverH
#define rrSteadyStateSolverH
#include "rrExporter.h"
#include "rrCapability.h"
//---------------------------------------------------------------------------

namespace rr
{

class RR_DECLSPEC SteadyStateSolver
{
    protected:
        Capability                      mCapability;

    public:
                                     SteadyStateSolver(const string& funcName, const string& description);
        virtual                    ~SteadyStateSolver();
        virtual double                 solve(const vector<double>& yin) = 0;
        virtual Capability&            getCapability() = 0;
};

}
#endif
