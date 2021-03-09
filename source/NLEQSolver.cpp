//
// Created by Ciaran on 08/03/2021.
//

#include "NLEQSolver.h"
#include "rrConfig.h"

namespace rr {

    NLEQSolver::NLEQSolver(ExecutableModel *_model) :
            model(_model) {
        NLEQSolver::resetSettings();
    }

    NLEQSolver::~NLEQSolver() = default;

    void NLEQSolver::syncWithModel(ExecutableModel *m) {
        model = m;
    }

    void NLEQSolver::loadConfigSettings() {
        SteadyStateSolver::loadConfigSettings();
        NLEQSolver::setValue("allow_presimulation", Config::getBool(Config::STEADYSTATE_PRESIMULATION));
        NLEQSolver::setValue("presimulation_maximum_steps",
                             Config::getInt(Config::STEADYSTATE_PRESIMULATION_MAX_STEPS));
        NLEQSolver::setValue("presimulation_time", Config::getDouble(Config::STEADYSTATE_PRESIMULATION_TIME));
        NLEQSolver::setValue("allow_approx", Config::getBool(Config::STEADYSTATE_APPROX));
        NLEQSolver::setValue("approx_tolerance", Config::getDouble(Config::STEADYSTATE_APPROX_TOL));
        NLEQSolver::setValue("approx_maximum_steps", Config::getInt(Config::STEADYSTATE_APPROX_MAX_STEPS));
        NLEQSolver::setValue("approx_time", Config::getDouble(Config::STEADYSTATE_APPROX_TIME));
        NLEQSolver::setValue("relative_tolerance", Config::getDouble(Config::STEADYSTATE_RELATIVE));
        NLEQSolver::setValue("maximum_iterations", Config::getInt(Config::STEADYSTATE_MAXIMUM_NUM_STEPS));
        NLEQSolver::setValue("minimum_damping", Config::getDouble(Config::STEADYSTATE_MINIMUM_DAMPING));
        NLEQSolver::setValue("broyden_method", Config::getInt(Config::STEADYSTATE_BROYDEN));
        NLEQSolver::setValue("linearity", Config::getInt(Config::STEADYSTATE_LINEARITY));
    }

    void NLEQSolver::resetSettings() {
        SteadyStateSolver::resetSettings();

        // Set default settings.
        addSetting("allow_approx", false, "Allow Approximiation",
                   "Flag for using steady state approximation routine when steady state solver fails (bool).",
                   "(bool) Approximation routine will run only when the default solver fails to fine a solution. This flag does not affect usage of approximation routine for pre-simulation");
        addSetting("approx_tolerance", 1e-6, "Approximation Tolerance",
                   "Tolerance for steady state approximation routine (double).",
                   "(double) Absolute tolerance used by steady state approximation routine. Only used when steady state approximation routine is used");
        addSetting("approx_maximum_steps", 10000, "Approximation Maximum Steps",
                   "Maximum number of steps that can be taken for steady state approximation routine (int).",
                   "(int) Takes priority over approx_time. Only used when steady state approximation routine is used");
        addSetting("approx_time", 10000, "Approximation Time",
                   "End time for steady state approximation routine (double).",
                   "(double) approx_maximum_steps takes priority. Only used when steady state approximation routine is used");
        addSetting("relative_tolerance", 1e-12, "Relative Tolerance", "Specifies the relative tolerance (double).",
                   "(double) Relative tolerance used by the solver");
        addSetting("maximum_iterations", 100, "Maximum Iterations",
                   "The maximum number of iterations the solver is allowed to use (int)",
                   "(int) Iteration caps off at the maximum, regardless of whether a solution has been reached");
        addSetting("minimum_damping", 1e-20, "Minimum Damping", "The minimum damping factor (double).",
                   "(double) Minumum damping factor used by the algorithm");
        addSetting("broyden_method", 0, "Broyden Method", "Switches on Broyden method (int)",
                   "(int) Broyden method is a quasi-Newton approximation for rank-1 updates");
        addSetting("linearity", 3, "Problem Linearity", "Specifies linearity of the problem (int).",
                   "(int) 1 is for linear problem and 4 is for extremly nonlinear problem");
        //NLEQSolver::loadConfigSettings();
    }

}    //end of namespace





