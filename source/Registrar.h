//
// Created by Ciaran Welsh on 14/06/2021.
//

#ifndef ROADRUNNER_REGISTRAR_H
#define ROADRUNNER_REGISTRAR_H

#include "rrExporter.h"
#include <iostream>
#include <vector>

namespace rr {

    // fwd
    class ExecutableModel;
    class Solver;

    /**
     * @author JKM, WBC
     * @brief Handles constructing a solver and contains meta
     * information about it
     */
    class RR_DECLSPEC Registrar {

    public:
        virtual ~Registrar() = default;

        /**
         * @author JKM, WBC
         * @brief Gets the name associated with this Solver type
         */
        virtual std::string getName() const = 0;

        /**
         * @author JKM, WBC
         * @brief Gets the description associated with this Solver type
         */
        virtual std::string getDescription() const = 0;

        /**
         * @author JKM, WBC
         * @brief Gets the hint associated with this Solver type
         */
        virtual std::string getHint() const = 0;

        /**
         * @author JKM, WBC
         * @brief Constructs a new Solver of a given type
         * @details the caller is responsible for deleting
         * memory associated with the returned Solver*.
         */
        virtual Solver *construct(ExecutableModel *model) const = 0;
    };

    typedef std::vector<Registrar *> RegistrarVector;

}


#endif //ROADRUNNER_REGISTRAR_H
