//
// Created by Ciaran on 08/03/2021.
//

#ifndef ROADRUNNER_NLEQSOLVER_H
#define ROADRUNNER_NLEQSOLVER_H

#include "rrExporter.h"
#include "SteadyStateSolver.h"

namespace rr {

    /**
     * @brief Base class for NLEQ type steady state solvers.
     */
    class RR_DECLSPEC NLEQSolver : public SteadyStateSolver {
    public:
        /**
        * Creates a new Instance of NLEQ for the given Model
        */
        explicit NLEQSolver(ExecutableModel *_model);

        ~NLEQSolver() override;

        /**
        * @brief Called whenever a new model is loaded to allow integrator
        * to reset internal state
        */
        void syncWithModel(ExecutableModel *m) override;

        void loadConfigSettings() override;

        /**
        * @brief Reset all integrator settings to their respective default values
        */
        void resetSettings() override;

        /**
         * @brief main solver method.
         * @details NLEQ1 and NLEQ2 have the same interface, which
         * means we only need 1 method for both.
         * @param which either "nleq1" or "nleq2"
         */
        template<class NLEQSolverType>
        double solveNLEQ() {

            auto nleq = std::unique_ptr<NLEQSolverType>(new NLEQSolverType(model));
            //rrLog(Logger::LOG_DEBUG) << "NLEQSolver::solve: " << std::endl;

            nleq->setItem("allowPreSim", getValue("allow_presimulation"));
            nleq->setItem("preSimMaximumSteps", getValue("presimulation_maximum_steps"));
            nleq->setItem("preSimTime", getValue("presimulation_time"));
            nleq->setItem("allowApprox", getValue("allow_approx"));
            nleq->setItem("approxTolerance", getValue("approx_tolerance"));
            nleq->setItem("approxMaximumSteps", getValue("approx_maximum_steps"));
            nleq->setItem("approxTime", getValue("approx_time"));
            nleq->setItem("relativeTolerance", getValue("relative_tolerance"));
            nleq->setItem("maxIterations", getValue("maximum_iterations"));
            nleq->setItem("minDamping", getValue("minimum_damping"));
            nleq->setItem("broyden", getValue("broyden_method"));
            nleq->setItem("linearity", getValue("linearity"));

            double result = nleq->solve();
            return result;
        }

    private:
        ExecutableModel *model = nullptr; // Model generated from the SBML. Static so we can access it from standalone function
    };

}


#endif //ROADRUNNER_NLEQSOLVER_H
