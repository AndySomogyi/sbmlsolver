//
// Created by Ciaran Welsh on 09/06/2021.
//

#include "AdjointSensitivitySolver.h"

namespace rr {

    std::string AdjointSensitivitySolver::getName() const {
        return "adjoint";
    }

    std::string AdjointSensitivitySolver::getDescription() const {
        return "Interface to the sundials cvodes adjoint sensitivities solver";
    }

    std::string AdjointSensitivitySolver::getHint() const {
        return "adjoint sensitivities solver";
    }

    SensitivityResult AdjointSensitivitySolver::solve() {
        return SensitivityResult();
    }

    Solver *AdjointSensitivitySolver::construct(ExecutableModel *executableModel) const {
        return new AdjointSensitivitySolver(executableModel);
    }


}

