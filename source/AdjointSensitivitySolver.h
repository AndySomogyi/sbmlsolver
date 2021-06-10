//
// Created by Ciaran Welsh on 09/06/2021.
//

#ifndef ROADRUNNER_ADJOINTSENSITIVITIES_H
#define ROADRUNNER_ADJOINTSENSITIVITIES_H

#include "SensitivitySolver.h"

namespace rr {

    class SensitivitySolver;

    class AdjointSensitivities : public SensitivitySolver {
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
         * @brief solve sensitivities for this model
         */
        SensitivityResult solve() override;
    };
}


#endif //ROADRUNNER_ADJOINTSENSITIVITIES_H
