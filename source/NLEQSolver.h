// == PREAMBLE ================================================

// * Licensed under the Apache License, Version 2.0; see README

// == FILEDOC =================================================

/** @file NLEQSolver.h
* @author ETS, WBC, JKM
* @date 08/03/2015
* @copyright Apache License, Version 2.0
* @brief Provides an interface to NLEQ
**/

#ifndef rrNLEQSolverH
#define rrNLEQSolverH
#include <vector>
#include "Solver.h"
#include "rrExecutableModel.h"
using std::vector;

namespace rr
{

/**
 * @internal
 */
class RR_DECLSPEC NLEQSolver : public Solver
{
    public:
        /**
        * Creates a new Instance of NLEQ for the given Model
        */
        NLEQSolver(ExecutableModel *_model = NULL);
        ~NLEQSolver();

        /**
        * @author JKM
        * @brief Called whenever a new model is loaded to allow integrator
        * to reset internal state
        */
        virtual void syncWithModel(ExecutableModel* m);

        void loadConfigSettings();

        /**
        * @author WBC
        * @brief Get the name for this Solver
        * @note Delegates to @ref getName
        */
        std::string getSolverName() const;

        /**
        * @author JKM
        * @brief Get the name for this Solver
        */
        static std::string getName();

        /**
        * @author WBC
        * @brief Get the description for this Solver
        * @note Delegates to @ref getDescription
        */
        std::string getSolverDescription() const;

        /**
        * @author JKM
        * @brief Get the description for this Solver
        */
        static std::string getDescription();

        /**
        * @author WBC
        * @brief Get the hint for this Solver
        * @note Delegates to @ref getHint
        */
        std::string getSolverHint() const;

        /**
        * @author JKM
        * @brief Get the hint for this Solver
        */
        static std::string getHint();

        // ** Getters / Setters ************************************************

        /**
        * @author WBC, ETS, MTK
        * @brief Always deterministic for CVODE
        */
        SolverMethod getSolverMethod() const;


        // ** Solver routines
        double solve(const vector<double>& yin);

    private:
        ExecutableModel *model; // Model generated from the SBML. Static so we can access it from standalone function
};


    // ** Registration *********************************************************


    class NLEQSolverRegistrar : public SolverRegistrar {
        public:
            /**
            * @author JKM
            * @brief Gets the name associated with this integrator type
            */
            virtual std::string getName() const {
                return NLEQSolver::getName();
            }

            /**
            * @author JKM
            * @brief Gets the description associated with this integrator type
            */
            virtual std::string getDescription() const {
                return NLEQSolver::getDescription();
            }

            /**
            * @author JKM
            * @brief Gets the hint associated with this integrator type
            */
            virtual std::string getHint() const {
                return NLEQSolver::getHint();
            }

            /**
            * @author JKM
            * @brief Constructs a new integrator of a given type
            */
            virtual Solver* construct(ExecutableModel *model) const {
                return new NLEQSolver(model);
            }
    };
}

#endif
