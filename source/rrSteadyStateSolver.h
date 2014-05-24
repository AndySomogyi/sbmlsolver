#ifndef rrSteadyStateSolverH
#define rrSteadyStateSolverH
#include "rrExporter.h"
#include <string>
#include <vector>
//---------------------------------------------------------------------------

namespace rr
{

/**
 * @internal
 */
class RR_DECLSPEC SteadyStateSolver
{

public:
    SteadyStateSolver(const std::string& funcName, const std::string& description);
    virtual ~SteadyStateSolver();
    virtual double solve(const std::vector<double>& yin) = 0;
};

}
#endif
