//
// Created by Ciaran on 14/03/2021.
//

#ifndef ROADRUNNER_APPROXSTEADYSTATEDECORATOR_H
#define ROADRUNNER_APPROXSTEADYSTATEDECORATOR_H

#include "SteadyStateSolverDecorator.h"

namespace rr {

    class ApproxSteadyStateDecorator : public SteadyStateSolverDecorator {
    public:
        ~ApproxSteadyStateDecorator() override = default;

        explicit ApproxSteadyStateDecorator(SteadyStateSolver* solver) ;

        double solve() override;

    private:
        std::string decoratorName() const override;

    };
}


#endif //ROADRUNNER_APPROXSTEADYSTATEDECORATOR_H
