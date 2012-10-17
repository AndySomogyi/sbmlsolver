#ifndef rrSteadyStateSolverH
#define rrSteadyStateSolverH
#include "rrObject.h"
//---------------------------------------------------------------------------

namespace rr
{

class RR_DECLSPEC ISteadyStateSolver : public rrObject
{
    /// <sumary>
    /// The actual solver routine aking the call to NLEQ1
    /// </sumary>
    /// <para name="yin">Array of Model variables</param>
    /// <returns>sus of squares </returns>
    public:
        virtual double solve(const vector<double>& yin) = 0;
};

}
#endif
