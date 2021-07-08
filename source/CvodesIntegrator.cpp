//
// Created by Ciaran Welsh on 09/06/2021.
//

#include "CvodesIntegrator.h"
#include "ForwardSensitivitySolver.h"
#include "LLVMExecutableModel.h"

namespace rr {

    std::string CvodesIntegrator::toString() const {
        std::stringstream ss;
        ss << "< roadrunner.SensitivitySolver() >\n";
        ss << "  name: " << getName() << "\n";
        ss << "  settings:\n";
        ss << getSettingsRepr();
        return ss.str();
    }

    std::string CvodesIntegrator::toRepr() const {
        std::stringstream ss;
        ss << "< roadrunner.SensitivitySolver() \"" << getName() << "\" " << settingsPyDictRepr() << " >\n";
        return ss.str();
    }

    void CvodesIntegrator::resetSettings() {
        CVODEIntegrator::resetSettings(); // integrator settings affect sensitivity
    }

    void CvodesIntegrator::loadConfigSettings() {
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
            flag = true;
        }
    }


}
