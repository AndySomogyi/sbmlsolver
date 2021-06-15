// == PREAMBLE ================================================

// * Licensed under the Apache License, Version 2.0; see README

// == FILEDOC =================================================

/** @file SteadyStateSolver.h
* @author ETS, WBC, JKM
* @date Sep 7, 2013
* @copyright Apache License, Version 2.0
* @brief Contains the base class for RoadRunner solvers
**/

# ifndef RR_STEADY_STATE_SOLVER_H_
# define RR_STEADY_STATE_SOLVER_H_

#include "rrLogger.h"
#include "rrOSSpecifics.h"
#include "Dictionary.h"
#include "rrException.h"
#include "Solver.h"
#include "Registrar.h"
#include "RegistrationFactory.h"

#include <memory>
#include <unordered_map>
#include <stdexcept>

namespace rr
{
    class ExecutableModel;

    /*-------------------------------------------------------------------------------------------
        SteadyStateSolver is an abstract base class that provides an interface to specific steady-state solver
        class implementations.
    ---------------------------------------------------------------------------------------------*/
    class RR_DECLSPEC SteadyStateSolver : public Solver
    {
    public:

        using Solver::Solver;

        ~SteadyStateSolver() override = default;

        /**
        * @author JKM
        * @brief Called whenever a new model is loaded to allow integrator
        * to reset internal state
        */
        virtual void syncWithModel(ExecutableModel* m) = 0;

        virtual double solve() = 0;

        virtual void loadConfigSettings();

		/**
		* @author KC
		* @brief Return a std::string representation of the solver
		*/
		std::string toString() const override;

		/**
		* @author KC
		* @brief Return std::string representation a la Python __repr__ method
		*/
		std::string toRepr() const override;

		/**
		 * Implements settings shared by
		 * all steady state solvers.
		 */
		void resetSettings() override;

    };

    static std::mutex steadyStateSolverFactoryMutex;
    static std::mutex steadyStateSolverRegistrationMutex;

    /**
     * @author JKM, WBC
     * @brief Constructs new integrators
     * @details Implements the factory and singleton patterns.
     * Constructs a new integrator given the name (e.g. cvode, gillespie)
     * and returns a base pointer to @ref rr::SteadyStateSolver.
     */
    class RR_DECLSPEC SteadyStateSolverFactory : public RegistrationFactory
    {
    public:

        /**
         * @author JKM, WBC
         * @brief Returns the singleton instance of the solver factory
         * @details If one exists return is otherwise create
         * one. This method implements the sigleton pattern
         * and is thread safe due to use of std::mutex.
         */
        static SteadyStateSolverFactory& getInstance();

        static void Register();
    };

}

# endif /* RR_STEADY_STATE_SOLVER_H_ */
