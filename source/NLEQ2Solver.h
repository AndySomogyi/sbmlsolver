// == PREAMBLE ================================================

// * Licensed under the Apache License, Version 2.0; see README

// == FILEDOC =================================================

/** @file NLEQ2Solver.h
* @author ETS, WBC, JKM, KC
* @date 08/03/2015
* @copyright Apache License, Version 2.0
* @brief Provides an interface to NLEQ
**/

#ifndef rrNLEQ2SolverH
#define rrNLEQ2SolverH
#include <vector>
#include "NLEQSolver.h"
#include "rrExecutableModel.h"

using std::vector;

namespace rr
{

/**
 * @internal
 */
class RR_DECLSPEC NLEQ2Solver : public NLEQSolver
{
    public:

        using NLEQSolver::NLEQSolver;

        /**
        * @author WBC, JKM
        * @brief Get the name for this Solver
        * @note Delegates to @ref getName
        */
        std::string getName() const override;

        /**
        * @author JKM
        * @brief Get the name for this Solver
        */
        static std::string getNLEQName();

        /**
        * @author WBC
        * @brief Get the description for this Solver
        * @note Delegates to @ref getDescription
        */
        std::string getDescription() const override;

        /**
        * @author JKM
        * @brief Get the description for this Solver
        */
        static std::string getNLEQDescription();

        /**
        * @author WBC
        * @brief Get the hint for this Solver
        * @note Delegates to @ref getHint
        */
        std::string getHint() const override;

        /**
        * @author JKM
        * @brief Get the hint for this Solver
        */
        static std::string getNLEQHint();

        // ** Solver routines
        double solve() override;

};


    // ** Registration *********************************************************


    class NLEQ2SolverRegistrar : public SteadyStateSolverRegistrar {
        public:
            /**
            * @author JKM
            * @brief Gets the name associated with this integrator type
            */
            virtual std::string getName() const {
                return NLEQ2Solver::getNLEQName();
            }

            /**
            * @author JKM
            * @brief Gets the description associated with this integrator type
            */
            virtual std::string getDescription() const {
                return NLEQ2Solver::getNLEQDescription();
            }

            /**
            * @author JKM
            * @brief Gets the hint associated with this integrator type
            */
            virtual std::string getHint() const {
                return NLEQ2Solver::getNLEQHint();
            }

            /**
            * @author JKM
            * @brief Constructs a new integrator of a given type
            */
            virtual SteadyStateSolver* construct(ExecutableModel *model) const {
                return new NLEQ2Solver(model);
            }
    };
}

#endif
