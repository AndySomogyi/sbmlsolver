//
// Created by Ciaran on 12/03/2021.
//

#include "PresimulationProgramDecorator.h"
#include "SteadyStateSolverDecorator.h"
#include "CVODEIntegrator.h"
#include "rrException.h"
#include "rrExecutableModel.h"

namespace rr {


    PresimulationProgramDecorator::PresimulationProgramDecorator(SteadyStateSolver *solver)
            : SteadyStateSolverDecorator(solver) {}

    double PresimulationProgramDecorator::solve() {
        if (!solver_->getModel()) {
            throw NullPointerException("PresimulationProgramDecorator::solve(): mModel instance in solver object is nullptr");
        }
        try {
            // try no presimulation first
            return SteadyStateSolverDecorator::solve();
        } catch  (std::exception& e) {
            std::vector<double> times = getValue("presimulation_times").convert<std::vector<double>>();
            for (const auto &timePoint: times) {
                CVODEIntegrator integrator(solver_->getModel());
                // integrate one interval between 0 and presimulation_time.
                integrator.integrate(0, timePoint);
                solver_->syncWithModel(solver_->getModel());

                try {
                    return solver_->solve();
                } catch (std::exception &err) {
                    // can't use any err more specific since errors used
                    // inherited from exception
                    continue;
                }
            }
        }

    }

    std::string PresimulationProgramDecorator::decoratorName() const {
        return "PresimulationProgram";
    }
}