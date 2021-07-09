//
// Created by Ciaran on 08/03/2021.
//

#include <iostream>
#include "NLEQSolver.h"
#include "rrConfig.h"

namespace rr {

    NLEQSolver::NLEQSolver(ExecutableModel *model) :
            SteadyStateSolver(model) {
        NLEQSolver::resetSettings();
    }

    NLEQSolver::~NLEQSolver() = default;

    void NLEQSolver::setModel(ExecutableModel *m) {
        mModel = m;
    }

    void NLEQSolver::loadConfigSettings() {
        SteadyStateSolver::loadConfigSettings();
        NLEQSolver::setValue("relative_tolerance",  Setting(Config::getDouble(Config::STEADYSTATE_RELATIVE)));
        NLEQSolver::setValue("maximum_iterations",  Setting(Config::getInt(Config::STEADYSTATE_MAXIMUM_NUM_STEPS)));
        NLEQSolver::setValue("minimum_damping",     Setting(Config::getDouble(Config::STEADYSTATE_MINIMUM_DAMPING)));
        NLEQSolver::setValue("broyden_method",      Setting(Config::getInt(Config::STEADYSTATE_BROYDEN)));
        NLEQSolver::setValue("linearity",           Setting(Config::getInt(Config::STEADYSTATE_LINEARITY)));
    }

    void NLEQSolver::resetSettings() {
        SteadyStateSolver::resetSettings();

        // Set default settings.
        addSetting("relative_tolerance", Setting(1e-12), "Relative Tolerance", "Specifies the relative tolerance (double).",
                   "(double) Relative tolerance used by the solver");

        addSetting("maximum_iterations", Setting(100), "Maximum Iterations",
                   "The maximum number of iterations the solver is allowed to use (int)",
                   "(int) Iteration caps off at the maximum, regardless of whether a solution has been reached");

        addSetting("minimum_damping", Setting(1e-20), "Minimum Damping", "The minimum damping factor (double).",
                   "(double) Minumum damping factor used by the algorithm");

        addSetting("broyden_method", Setting(0), "Broyden Method", "Switches on Broyden method (int)",
                   "(int) Broyden method is a quasi-Newton approximation for rank-1 updates");

        addSetting("linearity", Setting(3), "Problem Linearity", "Specifies linearity of the problem (int).",
                   "(int) 1 is for linear problem and 4 is for extremly nonlinear problem");

        NLEQSolver::loadConfigSettings();
    }

}    //end of namespace





