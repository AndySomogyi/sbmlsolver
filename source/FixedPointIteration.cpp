//
// Created by Ciaran on 26/02/2021.
//

#include "FixedPointIteration.h"
#include "KinsolErrHandler.h"

namespace rr {

    FixedPointIteration::FixedPointIteration(ExecutableModel *executableModel) : KinsolSteadyStateSolver(
            executableModel) {
        // note: we deliberately use the NewtonIteration namespace here
        // because calling virtual methods from constructors is dangerous.
        // We *must* ensure we call the right version of createKinsol
        FixedPointIteration::createKinsol();
    }

    void FixedPointIteration::createKinsol() {
        int err;

        // call shared initialization protocol
        KinsolSteadyStateSolver::createKinsol();

        int stateVectorSize = mModel->getStateVector(nullptr);

        /**
         * What things to implement as options?
         *  - Function                  default val
         *  - KINSetPrintLevel          0
         *  - KINSetNumMaxIters         200
         */

        // todo don't forget about these.
        KINSetPrintLevel(mKinsol_Memory, 3);

        KINSetDampingAA(mKinsol_Memory, 1.0);

        KINSetNoResMon(mKinsol_Memory, false);

        KINSetMAA(mKinsol_Memory, stateVectorSize);

        KINInit(mKinsol_Memory, kinsolDyDtFcn < FixedPointIteration > , mStateVector);
    }

    void FixedPointIteration::freeKinsol() {
        KinsolSteadyStateSolver::freeKinsol();
    }

    double FixedPointIteration::solve() {
        KINSol(
                mKinsol_Memory,   // kinsol memory block
                mStateVector,     // initial guess and solution std::vector
                KIN_FP,  // global strategy, other options defined in kinsol.h
                uscale,      //scaling std::vector for the variable cc
                fscale      //scaling std::vector for the variable fval
        );

        getSolverStatsFromKinsol();

        return 0.0;
    }

    void FixedPointIteration::updateKinsol() {

    }

    std::string FixedPointIteration::getName() const {
        return "FixedPointIteration";
    }

    std::string FixedPointIteration::getDescription() const {
        return "Uses the sundials implementation of fixed point "
               "iteration";
    }

    std::string FixedPointIteration::getHint() const {
        return "Fixed Point Iteration";
    }


}
