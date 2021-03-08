//
// Created by Ciaran on 26/02/2021.
//

#ifndef ROADRUNNER_KINSOLSTEADYSTATESOLVER_H
#define ROADRUNNER_KINSOLSTEADYSTATESOLVER_H

#include <nvector/nvector_serial.h>  /* access to serial N_Vector       */
#include <sundials/sundials_types.h> /* defs. of realtype, sunindextype */
#include "kinsol/kinsol.h"
#include "SteadyStateSolver.h"
#include "rrExecutableModel.h"
#include "rrException.h"

namespace rr {
    class ExecutableModel;

    class RoadRunner;

    /**
     * Utility macro for checking for a variable for nullptr
     * @param x the pointer that is being checked for null
     * @param fcn the name of the function call that produced the null
     * @param the return type for @param fcn
     */
#define KinsolCheckForNull(x, fcn, type) \
    if (x == nullptr){                   \
        std::ostringstream err_;          \
        err_ << "The value of object with type: \"" << type << "\" " \
    "which is the output of sundials function " << fcn << " is nullptr" << std::endl;\
        throw  NullPointerException(err_.str());                                  \
    };                               \


    /**
     * @brief base class to steady state solvers from the
     * Sundials package.
     * @details The steady state solver in Sundials
     * is called Kinsol. They have multiple different strategies
     * some which require model integration, and some that do not.
     *
     */
    class KinsolSteadyStateSolver : public SteadyStateSolver {
    public:
        /**
         * @brief constructor for creating KinsolSteadyStateSolver
         * from an executable model only. This constructor
         * should be inherited by subclasses
         */
        explicit KinsolSteadyStateSolver(ExecutableModel *executableModel);

        /**
         * @brief The default constructor for constructing without a model.
         * @details useful only for testing.
         */
        KinsolSteadyStateSolver() = default;

        /**
         * @brief virtual destructor
         */
        ~KinsolSteadyStateSolver() override = default;

        /**
         * @brief implementation of Solver::syncWithModel.
         * @details called by the main RoadRunner class to
         * syncronize the model with its solvers (to be sure
         * were not simulating an out of date model).
         */
        void syncWithModel(ExecutableModel *m) override;

        /**
         * @brief set all elements of the fscale variable to @param value.
         */
        void setFScale(double value);

        /**
         * @brief set elements of the fscale variable to @param value.
         * @param value should have the same size as the state vector
         */
        void setFScale(const std::vector<double>& value);

        /**
         * @brief set all elements of the uscale variable to @param value.
         */
        void setUScale(double value);

        /**
         * @brief set elements of the uscale variable to @param value.
         * @param value should have the same size as the state vector
         */
        void setUScale(std::vector<double> value);


        /**
         * @brief kinsol callback function that implements system equations
         * @details This is the function that gets called by the
         * kinsol `KINSol` command for solving the system.
         * @note this member must be static in order to be used as a
         * function pointer by KINInit.
         */
        template<class KinsolSteadyStateSolverType>
        static int kinsolDyDtFcn(N_Vector stateVecIn, N_Vector stateVecOut, void *userData) {
            // we need to use the state vector y (chosen by the solver)
            // to compute the rates dydt and then update the state vector
            // using the rates.
            double *y = NV_DATA_S(stateVecIn);

            // initialize the out NVector with numerical limits for double
            N_VConst(std::numeric_limits<double>::max(), stateVecOut);
            double *dydt = NV_DATA_S(stateVecOut);

            std::cout << "State vec in: ";
            stateVecIn->ops->nvprint(stateVecIn);

            // cast user data back into our solver type
            auto solver = (KinsolSteadyStateSolverType *) userData;

            assert(solver && "userData pointer is NULL in callback kinsolDyDtFcn");

            ExecutableModel *model = solver->mModel;

            // collect the number of states in this model
            int numStates = model->getStateVector(nullptr);

            // multiply by step size (which defaults to 1)
            // this has an affect on accuracy and convergence
            // todo in testing a step was added, like in one of the examples
            //   but it may not be needed. So comment out for now. Remove before
            //   merge into develop branch!
            // double step = solver->getValueAsDouble("Step");

            // get rates from the model
            // note: some of the examples used a "step" of 0.1, I'm not sure how important this is
            //  given that it doesn't seem to be needed. Keep for now.
            // model->getStateVectorRate(model->getTime() + step, y, dydt);
            model->getStateVectorRate(model->getTime() , y, dydt);

            for (int i = 0; i < numStates; i++) {
                if (dydt[i] == std::numeric_limits<double>::max()) {
                    std::ostringstream err;
                    err << __FILE__":" << __LINE__ << ":" << "kinsolDyDtFcn";
                    err << ": steady state solver \"" << solver->getName()
                        << "\". The rate for variable \"" << model->getStateVectorId(i)
                        << "\" is equal to the numerical ";
                    err << "limits for a double." << std::endl;
                    throw KinsolException(err.str());
                }
            }
            rrLog(Logger::LOG_TRACE) << __FUNC__ << ", model: " << model;

            std::cout << "State vec out: ";
            stateVecOut->ops->nvprint(stateVecOut);
            //std::cout << "size of system: " << stateVecIn->ops->nvgetlength(stateVecIn) << std::endl;
            return KIN_SUCCESS;
        };

        /**
         * @brief getter for the underlying memory block
         * that is used by kinsol.
         * @details mostly useful in testing.
         */
        void *getKinsolMemory() const;

    protected:

        /**
         * @brief Destory kinsol objects
         * @details Destroys the kinsol objects that
         * are shared among KinsolSteadyStateSolvers
         */
        virtual void freeKinsol();

        /**
         * @brief Calls methods for updating options in kinsol
         * @details User passes in arguments like uScale and they
         * are stored at the class level. This method passed them
         * on to kinsol.
         */
        virtual void updateKinsol();

        /**
         * @brief Initialize kinsol objects.
         * @details All subclasses require some of the same
         * initial calls (such as N_VNew_Serial and KINCreate).
         */
        virtual void createKinsol();

        /**
         * @brief add settings to the Solver::settings
         * map when called for the first time or
         * reset the settings map back to default values
         * when called subsequent times.
         */
        void resetSettings() override;

        /**
        * @brief Pointer to the kinsol memory block
        */
        void *mKinsol_Memory = nullptr;

        /**
         * @brief a sundials N_Vector for storing system state variables
         */
        N_Vector mStateVector = nullptr;

        /**
         * @brief vector containing diagonal elements of scaling matrix DF for F(u)
         * chosen so that the components of D_F*F(u) (as a matrix multiplication) all
         * have roughly the same magnitude when u is not too near a root of F(u). In
         * the case of a fixed-point iteration, consider F(u) = G(u) - u.
         * @note here u is mStateVector, F is the vector of rate functions and D_u an internal
         * variable inside kinsol
         *
         */
        N_Vector fscale = nullptr;

        /**
         * @brief vector containing diagonal elements of scaling matrix Du for vector
         * u chosen so that the components of D_u*u (as a matrix multiplication) all
         * have roughly the same magnitude when u is close to a root of F(u).
         * @note here u is mStateVector, F is the vector of rate functions and D_u an internal
         * variable inside kinsol
         */
        N_Vector uscale = nullptr;


        /**
         * @brief after solve() has been called, this method
         * collects some details such as number of function evals
         *
         */
        void getSolverStats();

        /**
         * @brief
         */
        long int backtrackOps;
        long int betaCondFails;
        long int funcEvals;
        long int jacEvals;
        long int jtimesEvals;
        long int linConvFails;
        long int linFuncEvals;
        long int linIters;
        long int nonlinSolvIters;
        long int precEvals;
        long int precSolves;

        /**
         * @brief stores the number of iterations required
         * by the solver to converge.
         * @details empty until after solve() has been called.
         * This method uses kinsol's KINGetNumNonlinSolvIters function
         */
        long int nSolverIterations = 0;

        /**
         * @brief stores the number of function evaluations
         * executed by the solver.
         * @details empty until after solve() has been called
         * This method uses kinsol's KINGetNumFuncEvals function
         */
        long int nFuncEvals = 0;

    };
}


#endif //ROADRUNNER_KINSOLSTEADYSTATESOLVER_H
