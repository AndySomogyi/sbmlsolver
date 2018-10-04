#include "rrSteadyStateSolver.h"
#include "SteadyStateSolver.h"
#include "rrNLEQ1Interface.h"
#include "rrNLEQ2Interface.h"

using namespace std;


namespace rr
{


rrSteadyStateSolver* rrSteadyStateSolverFactory::New(const Dictionary* doct,
        ExecutableModel* model)
{
    SteadyStateSolver* steady_state_solver;
    if (steady_state_solver->getName() == "NLEQ1")
    {
        return new NLEQ1Interface(model);
    }
    else if (steady_state_solver->getName() == "NLEQ2")
    {
        return new NLEQ2Interface(model);
    }
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
    SteadyStateSolver* steady_state_solver;

    if (steady_state_solver->getName() == "NLEQ1")
    {
        res.push_back(NLEQ1Interface::getSteadyStateOptions());
    }
    else if (steady_state_solver->getName() == "NLEQ2")
    {
        res.push_back(NLEQ2Interface::getSteadyStateOptions());
    }

    return res;
}

const Dictionary* rr::rrSteadyStateSolverFactory::getSteadyStateOptions(
        const std::string& name)
{
    SteadyStateSolver* steady_state_solver;
    if (steady_state_solver->getName() == "NLEQ1")
    {
        return NLEQ1Interface::getSteadyStateOptions();;
    }
    else if (steady_state_solver->getName() == "NLEQ2")
    {
        return NLEQ2Interface::getSteadyStateOptions();;
    }
}

}
