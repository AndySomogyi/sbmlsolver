
#include "SensitivitySolverFactory.h"

namespace rr {

    /**
     * To make the singleton SensitivitySolverFactory thread safe
     */
    static std::mutex sensitivitySolverMutex;
    static std::mutex sensitivityRegistrationMutex;


    SensitivitySolverFactory &SensitivitySolverFactory::getInstance() {
        return RegistrationFactory::getInstance<SensitivitySolverFactory>(sensitivitySolverMutex);
    }

    void SensitivitySolverFactory::Register() {
        std::lock_guard<std::mutex> mtx(sensitivityRegistrationMutex);
        static bool flag = false;
        if (!flag) {
            SensitivitySolverFactory::getInstance().registerSolver(new ForwardSensitivitySolver);
            flag = true;
        }
    }
}

