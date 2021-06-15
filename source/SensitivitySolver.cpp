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

    void SensitivitySolver::loadConfigSettings() {
        CVODEIntegrator::loadConfigSettings();
    }


    SensitivitySolverFactory &SensitivitySolverFactory::getInstance() {
        return RegistrationFactory::getInstance<SensitivitySolverFactory>(sensitivitySolverMutex);
    }

    void SensitivitySolverFactory::Register() {
        static bool flag = false;
        if (!flag) {
            std::lock_guard<std::mutex> mtx(sensitivityRegistrationMutex);
            SensitivitySolverFactory::getInstance().registerSolver(new ForwardSensitivitySolver);
            SensitivitySolverFactory::getInstance().registerSolver(new AdjointSensitivitySolver);
            flag = true;
        }
    }


}
