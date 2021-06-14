//
// Created by Ciaran Welsh on 09/06/2021.
//

#include "SensitivitySolver.h"
#include "ForwardSensitivitySolver.h"
#include "AdjointSensitivitySolver.h"
#include "LLVMExecutableModel.h"

namespace rr {

    std::string SensitivitySolver::toString() const {
        std::stringstream ss;
        ss << "< roadrunner.SensitivitySolver() >\n";
        ss << "  name: " << getName() << "\n";
        ss << "  settings:\n";
        ss << getSettingsRepr();
        return ss.str();
    }

    std::string SensitivitySolver::toRepr() const {
        std::stringstream ss;
        ss << "< roadrunner.SensitivitySolver() \"" << getName() << "\" " << settingsPyDictRepr() << " >\n";
        return ss.str();
    }

    void SensitivitySolver::resetSettings() {
        CVODEIntegrator::resetSettings(); // integrator settings affect sensitivity

    }

    void SensitivitySolver::loadConfigSettings() {}

    SensitivitySolver *SensitivitySolver::makeSensitivitySolver(const string &name, ExecutableModel *executableModel) {
        return nullptr;
    }

    SensitivitySolverFactory &SensitivitySolverFactory::getInstance() {
        return FactoryWithRegistration::getInstance<SensitivitySolverFactory>(sensitivitySolverMutex);
    }


}
