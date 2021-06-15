//
// Created by Ciaran Welsh on 09/06/2021.
//

#ifndef ROADRUNNER_FORWARDSENSITIVITYSOLVER_H
#define ROADRUNNER_FORWARDSENSITIVITYSOLVER_H

#include "SensitivitySolver.h"

namespace rr {

    class ForwardSensitivitySolver : public SensitivitySolver {
    public:
        using SensitivitySolver::SensitivitySolver;

        /**
        * @brief Get the name of this solver
        */
        std::string getName() const override;

        /**
        * @brief Get the description of this solver
        */
        std::string getDescription() const override;

        /**
        * @brief Get a (user-readable) hint for this solver
        */
        std::string getHint() const override;

        /**
        * @brief Get the name of this solver
        */
        static std::string getForwardSensitivitySolverName();

        /**
        * @brief Get the description of this solver
        */
        static std::string getForwardSensitivitySolverDescription();

        /**
        * @brief Get a (user-readable) hint for this solver
        */
        static std::string getForwardSensitivitySolverHint();

        /**
         * @brief resets all settings back to default values
         */
         void resetSettings() override;

        /**
         * @brief solve sensitivities for this model
         */
        SensitivityResult solve() override;
    };

    class ForwardSensitivityRegistrar : public Registrar {
    public:
        /**
        * @author JKM
        * @brief Gets the name associated with this integrator type
        */
        std::string getName() const override {
            return ForwardSensitivitySolver::getForwardSensitivitySolverName();
        }

        /**
        * @author JKM
        * @brief Gets the description associated with this integrator type
        */
        std::string getDescription() const override {
            return ForwardSensitivitySolver::getForwardSensitivitySolverDescription();
        }

        /**
        * @author JKM
        * @brief Gets the hint associated with this integrator type
        */
        std::string getHint() const override {
            return ForwardSensitivitySolver::getForwardSensitivitySolverHint();
        }

        /**
        * @author JKM
        * @brief Constructs a new sensitivity solver of a given type
        */
        SensitivitySolver *construct(ExecutableModel *model) const override {
            return new ForwardSensitivitySolver(model);
        }
    };
}


#endif //ROADRUNNER_FORWARDSENSITIVITYSOLVER_H
