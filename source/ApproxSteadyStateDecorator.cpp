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

            // integrate and collect the sundials N_Vector
            //  note that we do not necessarily need to start at 0 (manaully verified, results are accurate).
            integrator.integrate(end - (2*stepSize), stepSize);
            solver_->syncWithModel(solver_->getModel());
            N_Vector stateVectorAtTMinus2 = integrator.getStateVector();

            auto stateVectorAtTMinus2ArrPtr = std::make_unique<double*>(new double[numVariables]) ;
            for (int i=0; i<numVariables; i++){
                // make the copy
                (*stateVectorAtTMinus2ArrPtr)[i] = stateVectorAtTMinus2->ops->nvgetarraypointer(stateVectorAtTMinus2)[i];
            }

            // integrate collect the new sundials N_Vector
            integrator.integrate(end - stepSize, stepSize);
            N_Vector stateVectorAtT = integrator.getStateVector();

            double *stateVectorAtTArrPtr = stateVectorAtT->ops->nvgetarraypointer(stateVectorAtT);
            solver_->syncWithModel(solver_->getModel());

            double tol = 0;
            for (int i = 0; i < stateVectorAtT->ops->nvgetlength(stateVectorAtT); i++) {
                tol += sqrt(
                        pow(
                                ((*stateVectorAtTMinus2ArrPtr)[i] - stateVectorAtTArrPtr[i]) / stepSize,
                                2)
                );
            }
            rrLog(Logger::LOG_DEBUG) << "Steady state approximation done";

            if (tol > thresh) {
                std::ostringstream err;
                err << "Failed to converge while running steady state approximation. "
                    << "Tolerance " << tol << " is not greater than threshold " << thresh
                    << ". Try increasing the time point at which the approximation is conducted "
                       "(with the \"approx_time\" argument) or increasing the "
                       "number of steps parameter (with the \"approx_maximum_steps\" argument). Note "
                       "that the \"approx_maximum_steps\" parameter is only used to compute step size and "
                       "a full integration with \"approximate_maximum_steps\" is *not* performed. Be aware that "
                       "your model might not have a steady state";
                throw CoreException(err.str());
            }
            return tol;
        }
    }

    std::string ApproxSteadyStateDecorator::decoratorName() const {
        return "Approximate";
    }

}
