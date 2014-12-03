#include "rrSteadyStateSolver.h"
#include "rrNLEQInterface.h"


namespace rr
{

SteadyStateSolver* rr::SteadyStateSolver::New(const SteadyStateOptions*,
        ExecutableModel* m)
{
    return new NLEQInterface(m);
}

SteadyStateSolver* SteadyStateSolverFactory::New(const Dictionary* doct,
        ExecutableModel* model)
{
    return new NLEQInterface(model);
}

std::vector<std::string> rr::SteadyStateSolverFactory::getSteadyStateNames()
{
    return std::vector<std::string>();
}

std::vector<const Dictionary*> rr::SteadyStateSolverFactory::getSteadyStateOptions()
{
    return std::vector<const Dictionary*>();
}

const Dictionary* rr::SteadyStateSolverFactory::getSteadyStateOptions(
        const std::string& name)
{
    return NULL;
}

}
