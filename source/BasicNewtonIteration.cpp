//
// Created by Ciaran on 26/03/2021.
//

#include "BasicNewtonIteration.h"

namespace rr {
    
    BasicNewtonIteration::BasicNewtonIteration(ExecutableModel *executableModel) 
    : NewtonIteration(executableModel) {}

    std::string BasicNewtonIteration::getName() const {
        return BasicNewtonIteration::getBasicNewtonIterationName();
    }

    std::string BasicNewtonIteration::getDescription() const {
        return BasicNewtonIteration::getBasicNewtonIterationDescription();
    }

    std::string BasicNewtonIteration::getHint() const {
        return BasicNewtonIteration::getBasicNewtonIterationHint();
    }

    std::string BasicNewtonIteration::getBasicNewtonIterationName() {
        return "newton";
    }

    std::string BasicNewtonIteration::getBasicNewtonIterationDescription() {
        return "Sundials implementation of newton iteration algorithm "
               "for solving steady state problems.";
    }

    std::string BasicNewtonIteration::getBasicNewtonIterationHint() {
        return "Newton iteration";
    }

    double BasicNewtonIteration::solve() {
        return solveForSteadyState(this, KIN_NONE);
    }

}