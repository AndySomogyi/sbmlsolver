//
// Created by Ciaran on 08/07/2021.
//

#ifndef ROADRUNNER_SENSITIVITYSOLVER_H
#define ROADRUNNER_SENSITIVITYSOLVER_H

#include "Solver.h"
#include "Matrix.h"

namespace rr {

    /**
     * @brief generic interface for all SensitivitySolvers
     */
    class SensitivitySolver : public Solver {
    public:
        using Solver::Solver;

        virtual ~SensitivitySolver() = default;

        /**
         * @brief Get the current values of sensitivities
         */
        virtual Matrix<double> getSensitivities() = 0;

    };

    /**
     * @brief generic interface for sensitivity solvers that
     * integrate the model and compute sensitivities at
     * each time point
     */
    class TimeSeriesSensitivitySolver : public SensitivitySolver {
    public:
        using SensitivitySolver::SensitivitySolver;
        virtual ~TimeSeriesSensitivitySolver() = default;

        /**
         * @brief integrate the model from t0 to t0 + hstep.
         * @note this signature is the same as that found in
         * Integrator().
         * @details integrating the model should update the
         * sensitivities, which are available from getSensitivities()
         */
        virtual double integrate(double t0, double hstep) = 0;

    };

    /**
     * @brief genetic interface for sensitivity solvers that
     * solve for steady state before computing model sensitivities
     */
    class SteadyStateSensitivitySolver : public SensitivitySolver {
    public:
        using SensitivitySolver::SensitivitySolver;
        virtual ~SteadyStateSensitivitySolver() = default;

        /**
         * @brief solves the model for steady state.
         * @details this method is called prior to computing sensitivities
         * to get sensitivities at steady state.
         */
        virtual double solveSteadyState() = 0;

    };
}

#endif //ROADRUNNER_SENSITIVITYSOLVER_H
