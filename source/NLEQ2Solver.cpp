#pragma hdrstop

#include "NLEQ2Solver.h"
#include "rrNLEQ2Interface.h"
//#include "rrNLEQ2Interface.h"


namespace rr {

    std::string NLEQ2Solver::getName() const {
        return NLEQ2Solver::getNLEQName();
    }

    std::string NLEQ2Solver::getNLEQName() {
        return "nleq2";
    }

    std::string NLEQ2Solver::getDescription() const {
        return NLEQ2Solver::getNLEQDescription();
    }

    std::string NLEQ2Solver::getNLEQDescription() {
        return "NLEQ2 is a non-linear equation solver which uses a global Newton "
               "method with rank strategies and adaptive damping strategies (see http://elib.zib.de/pub/elib/codelib/NewtonLib/)";
    }

    std::string NLEQ2Solver::getHint() const {
        return NLEQ2Solver::getNLEQHint();
    }

    std::string NLEQ2Solver::getNLEQHint() {
        return "Steady-state nonlinear systems of equations solver";
    }

    double NLEQ2Solver::solve() {
        return solveNLEQ<NLEQ2Interface>();
    }

}    //end of namespace


