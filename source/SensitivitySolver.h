//
// Created by Ciaran Welsh on 09/06/2021.
//

#ifndef ROADRUNNER_SENSITIVITIES_H
#define ROADRUNNER_SENSITIVITIES_H

#include <iostream>
#include "CVODEIntegrator.h"
#include "SensitivityResult.h"


namespace rr {

    class ForwardSensitivitySolver;

    class AdjointSensitivitySolver;

    class CVODEIntegrator;

    class SensitivitySolver : public CVODEIntegrator {
    public:
        using CVODEIntegrator::CVODEIntegrator;

        /**
         * @brief factory method for creating a SensitivitySolver
         */
        static SensitivitySolver *makeSensitivitySolver(const std::string &name, ExecutableModel *executableModel);

        /**
        * @brief Get the name of this solver
        */
        virtual std::string getName() const = 0;

        /**
        * @brief Get the description of this solver
        */
        virtual std::string getDescription() const = 0;

        /**
        * @brief Get a (user-readable) hint for this solver
        */
        virtual std::string getHint() const = 0;

        /**
         * @brief solve sensitivities for this model
         */
        virtual SensitivityResult solve() = 0;

        std::string toRepr() const override;

        void resetSettings() override;

        void loadConfigSettings() override;

        std::string toString() const override;

    };


    /**
     * @author JKM, WBC
     * @brief Handles constructing a solver and contains meta
     * information about it
     */
    class RR_DECLSPEC SensitivitySolverRegistrar {
    protected:
        typedef SensitivitySolver *(*SensitivitySolverCtor)(ExecutableModel *model);

    public:
        virtual ~SensitivitySolverRegistrar();

        /**
         * @author JKM, WBC
         * @brief Gets the name associated with this integrator type
         */
        virtual std::string getName() const = 0;

        /**
         * @author JKM, WBC
         * @brief Gets the description associated with this integrator type
         */
        virtual std::string getDescription() const = 0;

        /**
         * @author JKM, WBC
         * @brief Gets the hint associated with this integrator type
         */
        virtual std::string getHint() const = 0;

        /**
         * @author JKM, WBC
         * @brief Constructs a new integrator of a given type
         */
        virtual SensitivitySolver *construct(ExecutableModel *model) const = 0;
    };

    /**
     * @author JKM, WBC
     * @brief Constructs new integrators
     * @details Implements the factory and singleton patterns.
     * Constructs a new integrator given the name (e.g. cvode, gillespie)
     * and returns a base pointer to @ref rr::SensitivitySolver.
     */
    class RR_DECLSPEC SensitivitySolverFactory {
    public:
        virtual ~SensitivitySolverFactory();

        /**
         * @author JKM, WBC
         * @brief Constructs a new solver given the name
         * (e.g. cvode, gillespie)
         */
        SensitivitySolver *New(const std::string &name, ExecutableModel *m) const;

        /**
         * @author JKM, WBC
         * @brief Registers a new solver with the factory
         * so that it can be constructed
         * @details Should be called at startup for new solvers.
         */
        void registerSensitivitySolver(SensitivitySolverRegistrar *i);

        /**
         * @author JKM, WBC
         * @brief Returns the singleton instance of the solver factory
         */
        static SensitivitySolverFactory &getInstance();

        // ** Indexing *********************************************************

        std::size_t getNumSensitivitySolvers() const;

        std::vector<std::string> getListSensitivitySolverNames();

        std::string getSensitivitySolverName(std::size_t n) const;

        std::string getSensitivitySolverHint(std::size_t n) const;

        std::string getSensitivitySolverDescription(std::size_t n) const;

    private:
        /**
         * @author JKM, WBC
         * @brief Prevents external instantiation
         */
        SensitivitySolverFactory() {}

        typedef std::vector<SensitivitySolverRegistrar *> SensitivitySolverRegistrars;
        SensitivitySolverRegistrars mRegisteredSensitivitySolvers;
    };


}

#endif //ROADRUNNER_SENSITIVITIES_H
