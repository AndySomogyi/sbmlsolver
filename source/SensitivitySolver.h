//
// Created by Ciaran on 08/07/2021.
//

#ifndef ROADRUNNER_SENSITIVITYSOLVER_H
#define ROADRUNNER_SENSITIVITYSOLVER_H

#include "Solver.h"

namespace rr {
    
    class SensitivitySolver : public Solver {
    public:
        using Solver::Solver;

        virtual ~SensitivitySolver() = default;
    };
}

#endif //ROADRUNNER_SENSITIVITYSOLVER_H
