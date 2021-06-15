//
// Created by Ciaran Welsh on 09/06/2021.
//

#include "ForwardSensitivitySolver.h"

namespace rr{

    std::string ForwardSensitivitySolver::getName() const {
        return ForwardSensitivitySolver::getForwardSensitivitySolverName();
    }

    std::string ForwardSensitivitySolver::getDescription() const {
        return ForwardSensitivitySolver::getForwardSensitivitySolverDescription();
    }

    std::string ForwardSensitivitySolver::getHint() const {
        return ForwardSensitivitySolver::getForwardSensitivitySolverHint();
    }

    std::string ForwardSensitivitySolver::getForwardSensitivitySolverName() {
        return "forward";
    }

    std::string ForwardSensitivitySolver::getForwardSensitivitySolverDescription() {
        return "Interface to the sundials cvodes forward sensitivities solver";
    }

    std::string ForwardSensitivitySolver::getForwardSensitivitySolverHint() {
        return "forward sensitivities solver";
    }

    SensitivityResult ForwardSensitivitySolver::solve() {
        return SensitivityResult();
    }

    void ForwardSensitivitySolver::resetSettings() {
        SensitivitySolver::resetSettings();
    }



}