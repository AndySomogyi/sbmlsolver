#ifndef rrSteadyStateSolverH
#define rrSteadyStateSolverH
#include "rrExporter.h"
#include "rrCapability.h"
//---------------------------------------------------------------------------

namespace rr
{

class RR_DECLSPEC SteadyStateSolver
{

public:
    SteadyStateSolver(const string& funcName, const string& description);
    virtual ~SteadyStateSolver();
    virtual double solve(const vector<double>& yin) = 0;
};

}
#endif
