//
// Created by Ciaran Welsh on 09/06/2021.
//

#include "AdjointSensitivitySolver.h"

namespace rr {

    std::string AdjointSensitivitySolver::getName() const {
        return getAdjointSensitivitySolverName();
    }

    std::string AdjointSensitivitySolver::getDescription() const {
        return getAdjointSensitivitySolverDescription();
    }

    std::string AdjointSensitivitySolver::getHint() const {
        return getAdjointSensitivitySolverHint();
    }

    SensitivityResult AdjointSensitivitySolver::solve() {
        return SensitivityResult();
    }

    std::string AdjointSensitivitySolver::getAdjointSensitivitySolverName() {
        return "adjoint";
    }

    std::string AdjointSensitivitySolver::getAdjointSensitivitySolverDescription() {
        return "Interface to the sundials cvodes adjoint sensitivities solver";
    }

    std::string AdjointSensitivitySolver::getAdjointSensitivitySolverHint() {
        return "adjoint sensitivities solver";
    }


}

