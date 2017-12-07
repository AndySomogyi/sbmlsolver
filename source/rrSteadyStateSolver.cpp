#include "rrSteadyStateSolver.h"
#include "rrNLEQInterface.h"

using namespace std;


namespace rr
{


rrSteadyStateSolver* rrSteadyStateSolverFactory::New(const Dictionary* doct,
        ExecutableModel* model)
{
    return new NLEQInterface(model);
}

std::vector<std::string> rr::rrSteadyStateSolverFactory::getSteadyStateNames()
{
    std::vector<std::string> res;
    res.push_back("NLEQ2");
    return res;
}

std::vector<const Dictionary*> rr::rrSteadyStateSolverFactory::getSteadyStateOptions()
{
    std::vector<const Dictionary*> res;
    res.push_back(NLEQInterface::getSteadyStateOptions());
    return res;
}

const Dictionary* rr::rrSteadyStateSolverFactory::getSteadyStateOptions(
        const std::string& name)
{
    return NLEQInterface::getSteadyStateOptions();;
}

}
