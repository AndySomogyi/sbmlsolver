#pragma hdrstop
#include "NLEQSolver.h"
#include "rrExecutableModel.h"
#include "rrStringUtils.h"
#include "rrUtils.h"
#include "nleq/nleq1.h"
#include "rrLogger.h"
#include "rrUtils.h"
#include "rrException.h"
#include "rrConfig.h"
#include "rrNLEQInterface.h"

#include <Poco/Mutex.h>
#include <assert.h>
#include <math.h>
#include <vector>
#include <unordered_map>

namespace rr
{

NLEQSolver::NLEQSolver(ExecutableModel *_model) :
    model(_model)
{
    resetSettings();
}

NLEQSolver::~NLEQSolver()
{
}

void NLEQSolver::syncWithModel(ExecutableModel* m)
{
    model = m;
}

void NLEQSolver::loadConfigSettings()
{
	Solver::loadConfigSettings();
	// Load settings specific to solver integrator

	NLEQSolver::setValue("maximum_iterations", Config::getInt(Config::STEADYSTATE_MAXIMUM_NUM_STEPS));
	NLEQSolver::setValue("relative_tolerance", Config::getDouble(Config::STEADYSTATE_RELATIVE));
	NLEQSolver::setValue("minimum_damping", Config::getDouble(Config::STEADYSTATE_MINIMUM_DAMPING));
}

void NLEQSolver::resetSettings()
{
    settings.clear();
    hints.clear();
    descriptions.clear();

    // Set default settings.
    addSetting("maximum_iterations", 100, "The maximum number of iterations the solver is allowed to use (int)", "(int) Iteration caps off at the maximum, regardless of whether a solution has been reached");
    addSetting("minimum_damping", 1e-4, "The minimum damping factor (double).", "(double) Minumum damping factor used by the algorithm");
    addSetting("relative_tolerance", 1e-16, "Specifies the relative tolerance (double).", "(double) Relative tolerance used by the solver");
    NLEQSolver::loadConfigSettings();
}

std::string NLEQSolver::getSolverName() const {
	return NLEQSolver::getName();
}

std::string NLEQSolver::getName() {
	return "nleq";
}

std::string NLEQSolver::getSolverDescription() const {
	return NLEQSolver::getDescription();
}

std::string NLEQSolver::getDescription() {
	return "CVODE is a deterministic ODE solver from the SUNDIALS suite "
		"of timecourse integrators. It implements an Adams-Moulton solver "
		"for non-stiff problems and a backward differentiation formula "
		"(BDF) solver for stiff problems.";
}

std::string NLEQSolver::getSolverHint() const {
	return NLEQSolver::getHint();
}

std::string NLEQSolver::getHint() {
	return "Steady-state nonlinear systems of equations solver";
}

Solver::SolverMethod NLEQSolver::getSolverMethod() const
{
	return Solver::SolverMethod::SteadyState;
}

double NLEQSolver::solve(const vector<double>& yin)
{
    Log(Logger::LOG_DEBUG) << "NLEQSolver::solve";

    NLEQInterface* nleq = new NLEQInterface(model);

//     nleq->maxIterations = getValue("maximum_iterations");
//     nleq->relativeTolerance = getValue("relative_tolerance");
//     nleq->minDamping = getValue("minimum_damping");

    double result = nleq->solve(yin);
    delete nleq;
    return result;
}

}    //end of namespace


