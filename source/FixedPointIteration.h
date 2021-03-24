//
// Created by Ciaran on 26/02/2021.
//

#ifndef ROADRUNNER_FIXEDPOINTITERATION_H
#define ROADRUNNER_FIXEDPOINTITERATION_H

#include "KinsolSteadyStateSolver.h"
#include <kinsol/kinsol.h>           /* access to KINSOL func., consts. */
#include <nvector/nvector_serial.h>  /* access to serial N_Vector       */
#include <sundials/sundials_types.h> /* defs. of realtype, sunindextype */

namespace rr {


    class RR_DECLSPEC FixedPointIteration : public KinsolSteadyStateSolver {

    public:

        explicit FixedPointIteration(ExecutableModel* executableModel) ;

        ~FixedPointIteration() override;
        /**
        * 
        * @brief Get the name of this solver
        */
        std::string getName() const override;

        /**
        * 
        * @brief Get the description of this solver
        */
        std::string getDescription() const override;

        /**
        * 
        * @brief Get a (user-readable) hint for this solver
        */
        std::string getHint() const override;

        /**
         * @brief static version of getName
         */
        static std::string getFixedPointIterationName();

        /**
         * @brief static version of getDescription
         */
        static std::string getFixedPointIterationDescription();

        /**
         * @brief static version of getHint
         */
        static std::string getFixedPointIterationHint();

        double solve() override;

    private:
        
        void createKinsol() override;

        void freeKinsol() override;

        void updateKinsol() override;

    };

}


#endif //ROADRUNNER_FIXEDPOINTITERATION_H
