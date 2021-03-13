//
// Created by Ciaran on 12/03/2021.
//

#include "PresimulationDecorator.h"
#include "SteadyStateSolverDecorator.h"
#include "CVODEIntegrator.h"
#include "rrException.h"
#include "rrExecutableModel.h"

namespace rr {


    PresimulationDecorator::PresimulationDecorator(SteadyStateSolver *solver)
    : SteadyStateSolverDecorator(solver) {}

    double PresimulationDecorator::solve() {
        if (!solver_->getModel()){
            throw NullPointerException("PresimulationDecorator::solve(): mModel instance in solver object is nullptr");
        }
        CVODEIntegrator integrator(solver_->getModel());

        integrator.integrate(0, solver_->getValueAsDouble("presimulation_time") );
        solver_->syncWithModel(solver_->getModel());
        return solver_->solve();
    }
}