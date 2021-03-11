//
// Created by Ciaran on 11/03/2021.
//

#ifndef ROADRUNNER_PRESIMULATION_H
#define ROADRUNNER_PRESIMULATION_H

#include "rrExecutableModel.h"
#include "SteadyStateSolver.h"

namespace rr{

    class ExecutableModel; ///fwd

    /**
     * @brief Integrate model before using the
     * steady state solving algorithm.
     * @details This is a private class used by SteadyStateSolver types.
     * The "allow_presimulation"
     * option must be set to true for this to happen (default = false).
     * Uses the CVODEIntegrator to integrate the model
     * between boundary of 0 and presimulation_time. The argument
     * to presimulation_maximum_steps gets passed on to CVODEIntegrator's
     * maximum_num_steps argument. All other CVODEIntegrator options are
     * set to their default. If the model is stiff, the user will have to
     * set the stiff flag in the RoadRunner before solving.
     * @note presimulation has been refactored into its own class for
     * testability - we can't test this feature if it is encoded directly
     * in KinsolSteadyStateSolver where it needs to be a private method
     */
    class Presimulation {
    public:
        /**
         * @brief default constructor for presimulation class
         */
        Presimulation() = default;

        /**
         * @brief construct a Presimulation class from
         * @param model: a model to integrate
         * @param presimulation_maximum_steps: maximal steps CVODEIntegrator is allowed
         * @param presimulation_time: end time for presimulation
         * @param stiff: use stiff solver for integration
         */
        Presimulation(ExecutableModel *model, SteadyStateSolver* solver);

        void simulate(const Dictionary *options = nullptr);

    private:
        /**
         * @brief non owning model pointer
         */
        ExecutableModel *model_;

        /**
         * @brief Non owning pointer to solver
         * @details the solver dependency is injected
         */
        SteadyStateSolver* solver_;

    };
}


#endif //ROADRUNNER_PRESIMULATION_H
