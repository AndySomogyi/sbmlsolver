#include "rrSteadyStateSolver.h"
#include "rrNLEQ1Interface.h"
#include "rrNLEQ2Interface.h"

using namespace std;


namespace rr
{


rrSteadyStateSolver* rrSteadyStateSolverFactory::New(const Dictionary* doct,
        ExecutableModel* model)
{
    return new NLEQ1Interface(model);
}

std::vector<std::string> rr::rrSteadyStateSolverFactory::getSteadyStateNames()
{
    std::vector<std::string> res;
    res.push_back("NLEQ1");
    res.push_back("NLEQ2");
    return res;
}

std::vector<const Dictionary*> rr::rrSteadyStateSolverFactory::getSteadyStateOptions()
{
    std::vector<const Dictionary*> res;
    res.push_back(NLEQ1Interface::getSteadyStateOptions());
    return res;
}

const Dictionary* rr::rrSteadyStateSolverFactory::getSteadyStateOptions(
        const std::string& name)
{
    return NLEQ1Interface::getSteadyStateOptions();;
}

}
