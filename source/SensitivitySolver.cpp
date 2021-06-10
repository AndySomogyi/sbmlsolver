//
// Created by Ciaran Welsh on 09/06/2021.
//

#include "SensitivitySolver.h"
#include "ForwardSensitivitySolver.h"
#include "AdjointSensitivitySolver.h"
#include "LLVMExecutableModel.h"

namespace rr {

    SensitivitySolver *SensitivitySolver::makeSensitivitySolver(const string &name, ExecutableModel* executableModel) {
        if (name == "forward"){
            return new ForwardSensitivitySolver(executableModel);
        } else if (name == "adjoint"){
            return new AdjointSensitivitySolver(executableModel);
        } else {
            std::ostringstream err;
            err << "No sensitivity solver called \"" << name << "\"";
            throw std::invalid_argument(err.str());
        }
    }
}
