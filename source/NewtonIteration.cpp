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

    NewtonIteration::NewtonIteration(ExecutableModel *executableModel)
    : KinsolSteadyStateSolver(executableModel) {
        // note: we deliberately use the NewtonIteration namespace here
        // because calling virtual methods from constructors is dangerous.
        // We must ensure we call the right version of createKinsol
        NewtonIteration::resetSettings(); // note: must be called before createKinsol
        NewtonIteration::createKinsol();
    }

    NewtonIteration::~NewtonIteration() {
        NewtonIteration::freeKinsol();
    }

    void NewtonIteration::createKinsol() {
        int err;

        KinsolSteadyStateSolver::createKinsol();

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
        // memory leak somewhere???
        if ((err = KINSetLinearSolver(mKinsol_Memory, linearSolver, jac))) {
            decodeKinsolError(err);
        }
    }

    void NewtonIteration::freeKinsol() {
        KinsolSteadyStateSolver::freeKinsol();

        if (linearSolver) {
            SUNLinSolFree(linearSolver);
        }

        if (jac) {
            SUNMatDestroy(jac);
        }

        linearSolver = nullptr;
        jac = nullptr;
    }

    double NewtonIteration::solve() {
        assert(mKinsol_Memory && "Kinsol memory block is nullptr");
        assert(mStateVector && "Solvers state std::vector is nullptr");

        // ensures options have been correctly propagated to kinsol
        // before solving
        updateKinsol();

        int flag = KINSol(
                mKinsol_Memory,   // kinsol memory block
                mStateVector,     // initial guess and solution std::vector
                // global strategy, options defined in kinsol.h
                getValueAsString("strategy") == "basic" ? KIN_NONE : KIN_LINESEARCH,
                uscale,      //scaling std::vector for the variable cc
                fscale      //scaling std::vector for the variable fval
        );

        char *flagName = KINGetReturnFlagName(flag);
        //std::cout << "KINSol function returned \"" << flagName << "\" flag" << std::endl;

        // errors are handled automatically by the error handler for kinsol.
        // here we handle warnings and success flags
        switch (flag) {
            case KIN_SUCCESS: {
                rrLog(Logger::LOG_INFORMATION) << "Steady state found";
                break;
            }
            case KIN_INITIAL_GUESS_OK:{
                rrLog(Logger::LOG_INFORMATION) << "Steady state found. The guess u = u0 satisifed the "
                                            "system F(u) = 0 within the tolerances specified (the"
                                            "scaled norm of F(u0) is less than 0.01*fnormtol)." << std::endl;
                break;
            }
            case KIN_STEP_LT_STPTOL:{
                rrLog(Logger::LOG_WARNING) << "kinsol stopped based on scaled step length. This means that the current iterate may"
                                            "be an approximate solution of the given nonlinear system, but it is also quite possible"
                                            "that the algorithm is \"stalled\" (making insufficient progress) near an invalid solution,"
                                            "or that the scalar scsteptol is too large (see ScaledStepTol to"
                                            "change ScaledStepTol from its default value)." << std::endl;
                break;
            }
            default: {
                std::string errMsg = decodeKinsolError(flag);
                throw std::runtime_error("Kinsol Error: " + errMsg);
            };
        }
        free(flagName);

        getSolverStatsFromKinsol();

        // update the model's state values
        mModel->setStateVector(mStateVector->ops->nvgetarraypointer(mStateVector));

        return funcNorm;
    }

    void NewtonIteration::updateKinsol() {
        KinsolSteadyStateSolver::updateKinsol();
    }

    void NewtonIteration::resetSettings() {
        KinsolSteadyStateSolver::resetSettings();

        // options are: basic or linesearch
        std::string desc = "Two options, basic or linesearch. When basic, the algorithm follows "
                           "the Newton iteration algorithm which solves Xn+1 = Xn - J^-1(Xn) * f(Xn). "
                           "There are two update strategies implemented, the first (basic) and simplest is the "
                           "standard Newton strategy. The other method (linesearch) is a global strategy, "
                           "which attempts to use the direction implied by delta_n in the most efficient way for"
                           " furthering convergence of the nonlinear problem. This"
                           "technique is implemented in the second strategy, called Linesearch";
        addSetting(
                "strategy",
                "basic",
                "strategy",
                R"(Either "basic" or "linesearch". Strategy for underlying algorithm to use)",
                desc
        );
    }

    std::string NewtonIteration::getName() const {
        return NewtonIteration::getNewtonIterationName();
    }

    std::string NewtonIteration::getDescription() const {
        return NewtonIteration::getNewtonIterationDescription();
    }

    std::string NewtonIteration::getHint() const {
        return NewtonIteration::getNewtonIterationHint();
    }

    std::string NewtonIteration::getNewtonIterationName() {
        return "NewtonIteration";
    }

    std::string NewtonIteration::getNewtonIterationDescription() {
        return "Uses the sundials implementation of newton "
               "iteration";
    }

    std::string NewtonIteration::getNewtonIterationHint() {
        return "Newton Iteration";
    }


}
