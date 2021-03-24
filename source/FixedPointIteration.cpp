//
// Created by Ciaran on 26/02/2021.
//

#include "FixedPointIteration.h"
#include "KinsolErrHandler.h"

namespace rr {

    FixedPointIteration::FixedPointIteration(ExecutableModel *executableModel)
        : KinsolSteadyStateSolver(executableModel) {
        // note: we deliberately use the NewtonIteration namespace here
        // because calling virtual methods from constructors is dangerous.
        // We *must* ensure we call the right version of createKinsol
        FixedPointIteration::resetSettings();
        FixedPointIteration::createKinsol();
    }

    FixedPointIteration::~FixedPointIteration() noexcept {
        FixedPointIteration::freeKinsol();
    }

    void FixedPointIteration::createKinsol() {
        int err;

        // call shared initialization protocol
        KinsolSteadyStateSolver::createKinsol();

        int stateVectorSize = mModel->getStateVector(nullptr);

        // kinsolDyDtFcn has the signature of "KINSysFn"
        if ((err = KINInit(mKinsol_Memory, kinsolDyDtFcn < FixedPointIteration > , mStateVector)) != KIN_SUCCESS) {
            decodeKinsolError(err);
        }


    }

    void FixedPointIteration::freeKinsol() {
        KinsolSteadyStateSolver::freeKinsol();
    }

    double FixedPointIteration::solve() {

        updateKinsol();

        int flag = KINSol(
                mKinsol_Memory,   // kinsol memory block
                mStateVector,     // initial guess and solution std::vector
                KIN_FP,       // no linear solver, whereas piccard iteration has linear solver
                uscale,      //scaling std::vector for the variable cc
                fscale      //scaling std::vector for the variable fval
        );

        getSolverStatsFromKinsol();

        return 0.0;
    }

    void FixedPointIteration::updateKinsol() {

    }

    std::string FixedPointIteration::getName() const {
        return getFixedPointIterationName();
    }

    std::string FixedPointIteration::getDescription() const {
        return getFixedPointIterationDescription();
    }

    std::string FixedPointIteration::getHint() const {
        return getFixedPointIterationHint();
    }

    std::string FixedPointIteration::getFixedPointIterationName() {
        return "FixedPointIteration";
    }

    std::string FixedPointIteration::getFixedPointIterationDescription() {
        return "Uses the sundials implementation of fixed point "
               "iteration";
    }

    std::string FixedPointIteration::getFixedPointIterationHint() {
        return "Fixed point iteration";
    }


}
