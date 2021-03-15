//
// Created by Ciaran on 14/03/2021.
//

#include "ApproxSteadyStateDecorator.h"
#include "CVODEIntegrator.h"
#include "rrExecutableModel.h"

namespace rr {

    ApproxSteadyStateDecorator::ApproxSteadyStateDecorator(SteadyStateSolver *solver)
            : SteadyStateSolverDecorator(solver) {}

    double ApproxSteadyStateDecorator::solve() {

        if (!solver_->getModel()) {
            throw NullPointerException(
                    "ApproxSteadyStateDecorator::solve(): mModel instance in solver object is nullptr");
        }
        try {
            return SteadyStateSolverDecorator::solve();
        } catch (std::runtime_error &e) {
            const double &end = getValueAsDouble("approx_time");
            const int &steps = getValueAsInt("approx_maximum_steps");
            const double &thresh = getValueAsDouble("approx_tolerance");

            //  step_size * num_steps = duration
            const double stepSize = end / steps;

            const int &numVariables = mModel->getStateVector(nullptr);

            CVODEIntegrator integrator(solver_->getModel());

            // integrate betwee (0, timeMinus1), collect the sundials N_Vector
            integrator.integrate(0, end - stepSize);
            N_Vector stateVectorAtTMinus1 = integrator.getStateVector();
            double *stateVectorAtTMinus1ArrPtr = stateVectorAtTMinus1->ops->nvgetarraypointer(stateVectorAtTMinus1);

            // integrate between (timeMinus1, approx_time), collect the sundials N_Vector
            integrator.integrate(end - stepSize, end);
            N_Vector stateVectorAtT = integrator.getStateVector();
            double *stateVectorAtTArrPtr = stateVectorAtT->ops->nvgetarraypointer(stateVectorAtT);

            solver_->syncWithModel(solver_->getModel());

            double tol = 0;
            for (int i = 0; i < stateVectorAtT->ops->nvgetlength(stateVectorAtT); i++) {
                tol += sqrt(
                        pow(
                                (stateVectorAtTMinus1ArrPtr[i] - stateVectorAtTArrPtr[i]) / (end / steps),
                                2)
                );
            }
            rrLog(Logger::LOG_DEBUG) << "Steady state approximation done";

            if (tol > thresh) {
                throw CoreException("Failed to converge while running steady state approximation routine. Try increasing "
                                    "the time or maximum number of iteration via changing the settings under r.steadyStateSolver "
                                    "where r is an roadrunner instance. Model might not have a steady state.");
            }

            return tol;
        }
    }

    std::string ApproxSteadyStateDecorator::decoratorName() const {
        return "Approximate(" + solver_->getName() + ")";
    }

}
