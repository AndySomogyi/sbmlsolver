//
// Created by Ciaran on 26/02/2021.
//

#ifndef ROADRUNNER_NEWTONITERATION_H
#define ROADRUNNER_NEWTONITERATION_H

#include "KinsolSteadyStateSolver.h"
#include <kinsol/kinsol.h>           /* access to KINSOL func., consts. */
#include <nvector/nvector_serial.h>  /* access to serial N_Vector       */
#include <sundials/sundials_types.h> /* defs. of realtype, sunindextype */

namespace rr {

    class RR_DECLSPEC NewtonIteration : public KinsolSteadyStateSolver {

    public:

        ~NewtonIteration() override ;

        explicit NewtonIteration(ExecutableModel *executableModel);

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
         * @brief static version of getName
         */
        static std::string getNewtonIterationName();

        /**
         * @brief static version of getDescription
         */
        static std::string getNewtonIterationDescription();

        /**
         * @brief static version of getHint
         */
        static std::string getNewtonIterationHint();

        /**
         * @brief solve the currently defined problem
         */
        double solve() override;

        /**
         * @brief reset settings back to default.
         */
        void resetSettings() override;

    public:
        void createKinsol() override;

        void freeKinsol() override;

        void updateKinsol() override;

        /**
         * @brief matrix for jacobian
         */
        SUNMatrix jac = nullptr;

        /**
         * @brief linear solver for newton methods
         */
        SUNLinearSolver linearSolver = nullptr;
    };


    class NewtonIterationRegistrar : public SteadyStateSolverRegistrar {
    public:
        /**
        * @brief Gets the name associated with this integrator type
        */
        std::string getName() const override {
            return NewtonIteration::getNewtonIterationName();
        }

        /**
        * @brief Gets the description associated with this integrator type
        */
        std::string getDescription() const override {
            return NewtonIteration::getNewtonIterationDescription();
        }

        /**
        * @brief Gets the hint associated with this integrator type
        */
        std::string getHint() const override {
            return NewtonIteration::getNewtonIterationHint();
        }

        /**
        * @brief Constructs a new integrator of a given type
        */
        SteadyStateSolver *construct(ExecutableModel *model) const override {
            return new NewtonIteration(model);
        }
    };

}


#endif //ROADRUNNER_NEWTONITERATION_H
