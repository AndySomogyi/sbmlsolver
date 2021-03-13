//
// Created by Ciaran on 12/03/2021.
//

#include "SteadyStateSolverDecorator.h"

namespace rr {

    void SteadyStateSolverDecorator::syncWithModel(ExecutableModel *m) {
        return solver_->syncWithModel(m);
    }

    double SteadyStateSolverDecorator::solve() {
        return solver_->solve();
    }

    SteadyStateSolverDecorator::SteadyStateSolverDecorator(SteadyStateSolver *solver)
        : SteadyStateSolver(solver->getModel()), solver_(solver){
        settings = solver_->getSettings();
    }

    std::string SteadyStateSolverDecorator::getName() const {
        return solver_->getName();
    }

    std::string SteadyStateSolverDecorator::getDescription() const {
        return solver_->getDescription();
    }

    std::string SteadyStateSolverDecorator::getHint() const {
        return solver_->getHint();
    }

    void SteadyStateSolverDecorator::resetSettings()  {
        return solver_->resetSettings();
    }

}