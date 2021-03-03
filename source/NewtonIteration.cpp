//
// Created by Ciaran on 26/02/2021.
//

#include "NewtonIteration.h"
#include "rrExecutableModel.h"
#include <cassert>
#include "KinsolErrHandler.h"

#include "sunmatrix/sunmatrix_dense.h"
#include "sunlinsol/sunlinsol_dense.h"

namespace rr {

    NewtonIteration::NewtonIteration(ExecutableModel *executableModel) : KinsolSteadyStateSolver(
            executableModel) {
        // note: we deliberately use the NewtonIteration namespace here
        // because calling virtual methods from constructors is dangerous.
        // We *must* ensure we call the right version of createKinsol
        NewtonIteration::createKinsol();
    }

    void NewtonIteration::createKinsol() {
        int err;

        createKinsol_();

        // todo don't forget about this.
        KINSetPrintLevel(mKinsol_Memory, 3);

        // kinsolDyDtFcn has the signature of "KINSysFn"
        if ((err = KINInit(mKinsol_Memory, kinsolDyDtFcn < NewtonIteration > , mStateVector)) != KIN_SUCCESS) {
            decodeKinsolError(err);
        }

        /* Create dense SUNMatrix for jacobian */
        int numEq = mStateVector->ops->nvgetlength(mStateVector);
        jac = SUNDenseMatrix(numEq, numEq);
        KinsolCheckForNull(jac, "SUNDenseMatrix", "SUNMatrix")

        /* Create dense SUNLinearSolver object */
        linearSolver = SUNLinSol_Dense(mStateVector, jac);
        KinsolCheckForNull(jac, "SUNLinSol_Dense", "SUNLinearSolver")

        /* Attach the matrix and linear solver to KINSOL */
        if ((err = KINSetLinearSolver(mKinsol_Memory, linearSolver, jac))){
            decodeKinsolError(err);
        }

    }

    void NewtonIteration::freeKinsol() {
        freeKinsol_();

        if( linearSolver){
            SUNLinSolFree(linearSolver);
        }

        if(jac){
            SUNMatDestroy(jac);
        }

        linearSolver = nullptr;
        jac = nullptr;
    }

    double NewtonIteration::solve() {
        KINSol(
                mKinsol_Memory,   // kinsol memory block
                mStateVector,     // initial guess and solution vector
                KIN_NONE,  // global strategy, other options defined in kinsol.h
                uscale,      //scaling vector for the variable cc
                fscale      //scaling vector for the variable fval
        );

        getSolverStats();

        // update the model's state values
        mModel->setStateVector(mStateVector->ops->nvgetarraypointer(mStateVector));

        return 0.0;
    }

    void NewtonIteration::updateKinsol() {

    }

    std::string NewtonIteration::getName() const {
        return "NewtonIteration";
    }

    std::string NewtonIteration::getDescription() const {
        return "Uses the sundials implementation of newton "
               "iteration";
    }

    std::string NewtonIteration::getHint() const {
        return "Newton Iteration";
    }


}
