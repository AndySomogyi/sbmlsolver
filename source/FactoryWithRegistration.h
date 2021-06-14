//
// Created by Ciaran Welsh on 14/06/2021.
//

#ifndef ROADRUNNER_FACTORYWITHREGISTRATION_H
#define ROADRUNNER_FACTORYWITHREGISTRATION_H

#include "rrExporter.h"
#include <iostream>
#include "Registrar.h"
#include "Solver.h"

namespace rr {

    // fwd
    class ExecutableModel;
    class Solver;

    typedef std::vector<Registrar *> RegistrarVector;

    /**
     * @author JKM, WBC
     * @brief Constructs new integrators
     * @details Implements the factory and singleton patterns.
     * Constructs a new integrator given the name (e.g. cvode, gillespie)
     * and returns a base pointer to @ref rr::Integrator.
     */
    class RR_DECLSPEC FactoryWithRegistration {
    public:
        virtual ~FactoryWithRegistration();

        /**
         * @author JKM, WBC
         * @brief Constructs a new integrator given the name
         * (e.g. cvode, gillespie)
         */
        Solver *New(const std::string& name, ExecutableModel *m) const;

        /**
         * @author JKM, WBC
         * @brief Registers a new integrator with the factory
         * so that it can be constructed
         * @details Should be called at startup for new integrators.
         */
        virtual void registerSolver(Registrar *i);

        /**
         * @author JKM, WBC
         * @brief Returns the singleton instance of the integrator factory
         */
//        static FactoryWithRegistration &getInstance();

        virtual std::size_t size() const;

        virtual std::string name(std::size_t n) const;

        virtual std::string hint(std::size_t n) const;

        virtual std::string description(std::size_t n) const;

    protected:
        /**
         * @author JKM, WBC
         * @brief Prevents external instantiation
         */
        FactoryWithRegistration() = default;

        RegistrarVector registrars;

    };


}

#endif //ROADRUNNER_FACTORYWITHREGISTRATION_H
