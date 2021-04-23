//
// Created by Ciaran on 26/03/2021.
//

#ifndef ROADRUNNER_BASICNEWTONITERATION_H
#define ROADRUNNER_BASICNEWTONITERATION_H

#include "NewtonIteration.h"

namespace rr {

    /**
     * @brief Solve for steady state using Kinsol's implementation 
     * of Newton Iteration. 
     * @details The kinsol docs refer to this algorithm as the "basic" 
     * newton iteration to differentiate it from the "linesearch" strategy 
     * which is a variant of the "basic" newton iteration. 
     * @see LinesearchNewtonIteration
     */
    class BasicNewtonIteration : public NewtonIteration {

    public:
        
        ~BasicNewtonIteration() override = default ;

        explicit BasicNewtonIteration(ExecutableModel *executableModel);

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
        static std::string getBasicNewtonIterationName();

        /**
         * @brief static version of getDescription
         */
        static std::string getBasicNewtonIterationDescription();

        /**
         * @brief static version of getHint
         */
        static std::string getBasicNewtonIterationHint();

        /**
         * @brief solve the currently defined problem
         */
        double solve() override;

    };
    
    
    class BasicNewtonIterationRegistrar : public SteadyStateSolverRegistrar {
    public:
        /**
        * @brief Gets the name associated with this integrator type
        */
        std::string getName() const override {
            return BasicNewtonIteration::getBasicNewtonIterationName();
        }

        /**
        * @brief Gets the description associated with this integrator type
        */
        std::string getDescription() const override {
            return BasicNewtonIteration::getBasicNewtonIterationDescription();
        }

        /**
        * @brief Gets the hint associated with this integrator type
        */
        std::string getHint() const override {
            return BasicNewtonIteration::getBasicNewtonIterationHint();
        }

        /**
        * @brief Constructs a new integrator of a given type
        */
        SteadyStateSolver *construct(ExecutableModel *model) const override {
            return new BasicNewtonIteration(model);
        }
    };


}

#endif //ROADRUNNER_BASICNEWTONITERATION_H
