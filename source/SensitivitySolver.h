//
// Created by Ciaran Welsh on 09/06/2021.
//

#ifndef ROADRUNNER_SENSITIVITIES_H
#define ROADRUNNER_SENSITIVITIES_H

#include <iostream>
#include "CVODEIntegrator.h"
#include "SensitivityResult.h"


namespace rr {

    class ForwardSensitivitySolver;

    class AdjointSensitivitySolver;

    class CVODEIntegrator;

    class SensitivitySolver : public CVODEIntegrator {
    public:
        using CVODEIntegrator::CVODEIntegrator;

        /**
         * @brief factory method for creating a SensitivitySolver
         */
//        static SensitivitySolver *makeSensitivitySolver(const std::string &name, ExecutableModel *executableModel);

        /**
        * @brief Get the name of this solver
        */
        virtual std::string getName() const override = 0;

        /**
        * @brief Get the description of this solver
        */
        virtual std::string getDescription() const override = 0;

        /**
        * @brief Get a (user-readable) hint for this solver
        */
        virtual std::string getHint() const override = 0;

        /**
         * @brief solve sensitivities for this model
         */
        virtual SensitivityResult solve() = 0;

        std::string toRepr() const override;

        void resetSettings() override;

        void loadConfigSettings() override;

        std::string toString() const override;

    };

    /**
     * To make the singleton SensitivitySolverFactory thread safe
     */
    static std::mutex sensitivitySolverMutex;
    static std::mutex sensitivityRegistrationMutex;


    /**
     * @author JKM, WBC
     * @brief Constructs new integrators
     * @details Implements the factory and singleton patterns.
     * Constructs a new integrator given the name (e.g. cvode, gillespie)
     * and returns a base pointer to @ref rr::SensitivitySolver.
     */
    class RR_DECLSPEC SensitivitySolverFactory: public RegistrationFactory {
    public:
        static SensitivitySolverFactory& getInstance();

        static void Register();
    };


}

#endif //ROADRUNNER_SENSITIVITIES_H
