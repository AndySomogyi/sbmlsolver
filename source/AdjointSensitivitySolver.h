//
// Created by Ciaran Welsh on 09/06/2021.
//

#ifndef ROADRUNNER_ADJOINTSENSITIVITYSOLVER_H
#define ROADRUNNER_ADJOINTSENSITIVITYSOLVER_H

#include "SensitivitySolver.h"

namespace rr {

    class SensitivitySolver;

    class AdjointSensitivitySolver : public SensitivitySolver {
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
        static std::string getAdjointSensitivitySolverName();

        /**
        * @brief Get the description of this solver
        */
        static std::string getAdjointSensitivitySolverDescription();

        /**
        * @brief Get a (user-readable) hint for this solver
        */
        static std::string getAdjointSensitivitySolverHint();

        /**
         * @brief solve sensitivities for this model
         */
        SensitivityResult solve() override;
    };


    class AdjointSensitivityRegistrar : public Registrar {
    public:
        /**
        * @author JKM
        * @brief Gets the name associated with this integrator type
        */
        std::string getName() const override {
            return AdjointSensitivitySolver::getAdjointSensitivitySolverName();
        }

        /**
        * @author JKM
        * @brief Gets the description associated with this integrator type
        */
        std::string getDescription() const override {
            return AdjointSensitivitySolver::getAdjointSensitivitySolverDescription();
        }

        /**
        * @author JKM
        * @brief Gets the hint associated with this integrator type
        */
        std::string getHint() const override {
            return AdjointSensitivitySolver::getAdjointSensitivitySolverHint();
        }

        /**
        * @author JKM
        * @brief Constructs a new integrator of a given type
        */
        Integrator *construct(ExecutableModel *model) const override {
            return new CVODEIntegrator(model);
        }
    };
}


#endif //ROADRUNNER_ADJOINTSENSITIVITYSOLVER_H
