//
// Created by Ciaran Welsh on 09/06/2021.
//

#include "ForwardSensitivitySolver.h"

namespace rr{

    std::string ForwardSensitivitySolver::getName() const {
        return "forward";
    }

    std::string ForwardSensitivitySolver::getDescription() const {
        return "Interface to the sundials cvodes forward sensitivities solver";
    }

    std::string ForwardSensitivitySolver::getHint() const {
        return "forward sensitivities solver";
    }

    SensitivityResult ForwardSensitivitySolver::solve() {
        return SensitivityResult();
    }

    void ForwardSensitivitySolver::resetSettings() {
        SensitivitySolver::resetSettings();
    }

    Solver *ForwardSensitivitySolver::construct(ExecutableModel *executableModel) const {
        return new ForwardSensitivitySolver(executableModel);
    }


}