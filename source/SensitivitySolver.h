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
        static SensitivitySolver* makeSensitivitySolver(const std::string& name, ExecutableModel* executableModel);

//        /**
//        * @brief Get the name of this solver
//        */
//        virtual std::string getName() const = 0;
//
//        /**
//        * @brief Get the description of this solver
//        */
//        virtual std::string getDescription() const = 0;
//
//        /**
//        * @brief Get a (user-readable) hint for this solver
//        */
//        virtual std::string getHint() const = 0;

        /**
         * @brief solve sensitivities for this model
         */
        virtual SensitivityResult solve() = 0;


    };

}

#endif //ROADRUNNER_SENSITIVITIES_H
