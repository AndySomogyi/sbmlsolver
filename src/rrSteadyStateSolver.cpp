#include "rrSteadyStateSolver.h"
#include "rrNLEQInterface.h"

using namespace std;


namespace rr
{


SteadyStateSolver* SteadyStateSolverFactory::New(const Dictionary* doct,
        ExecutableModel* model)
{
    return new NLEQInterface(model);
}

std::vector<std::string> rr::SteadyStateSolverFactory::getSteadyStateNames()
{
    std::vector<std::string> res;
    res.push_back("NLEQ");
    return res;
}

std::vector<const Dictionary*> rr::SteadyStateSolverFactory::getSteadyStateOptions()
{
    std::vector<const Dictionary*> res;
    res.push_back(NLEQInterface::getSteadyStateOptions());
    return res;
}

const Dictionary* rr::SteadyStateSolverFactory::getSteadyStateOptions(
        const std::string& name)
{
    return NLEQInterface::getSteadyStateOptions();;
}

}
