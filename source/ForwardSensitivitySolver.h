//
// Created by Ciaran Welsh on 09/06/2021.
//

#ifndef ROADRUNNER_FORWARDSENSITIVITYSOLVER_H
#define ROADRUNNER_FORWARDSENSITIVITYSOLVER_H

#include "CvodesIntegrator.h"
#include "Matrix3D.h"

namespace rr {

    class ForwardSensitivitySolver : public CvodesIntegrator {
    public:
        using CvodesIntegrator::CvodesIntegrator;

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

        Solver* construct(ExecutableModel* executableModel) const override;

        /**
         * @brief resets all settings back to default values
         */
         void resetSettings() override;

        /**
         * @brief solve sensitivities for this model
         */
//        Matrix3D<double, double> solve();
    };

}


#endif //ROADRUNNER_FORWARDSENSITIVITYSOLVER_H
