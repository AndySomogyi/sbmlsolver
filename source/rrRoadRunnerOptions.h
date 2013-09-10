/*
 * rrRoadRunnerOptions.h
 *
 *  Created on: Sep 9, 2013
 *      Author: andy
 */

#ifndef RRROADRUNNEROPTIONS_H_
#define RRROADRUNNEROPTIONS_H_

#include "rrExporter.h"
#include <stdint.h>

namespace rr
{

/**
 * Options for loading SBML into RoadRunner.
 *
 * Future version may add additional fields to the end of this struct,
 * this way we can maintain binary compatibility with older RoadRunner versions.
 */
struct RR_DECLSPEC LoadSBMLOptions
{
    /**
     * the list of ODE solvers RoadRunner currently supports.
     */
    enum Integrator
    {
        CVODE
    };

    enum IntegratorOpt
    {
        /**
         * Is the model a stiff system? setting this to stiff causes
         * RoadRunner to load a stiff solver which could potentially be
         * extremly slow
         */
        Stiff = (0x1 << 0), // => 0x00000001
    };

    enum ModelGeneratorOpt
    {
        /**
         * perform conservation analysis.
         *
         * This causes a re-ordering of the species, so results generated
         * with this flag enabled can not be compared index wise to results
         * generated otherwise.
         *
         * currently only implemented with the C code generating model
         */
        ComputeAndAssignConsevationLaws = (0x1 << 0), // => 0x00000001

        /**
         * Should the model be recompiled?
         * The LLVM ModelGenerator maintins a hash table of currently running
         * models. If this flag is NOT set, then the generator will look to see
         * if there is already a running instance of the given model and
         * use the generated code from that one.
         */
        ForceReCompile                  = (0x1 << 1), // => 0x00000010
    };

    /**
     * initializes the struct with the default options.
     */
    LoadSBMLOptions();

    /**
     * the version this struct
     */
    uint16_t version;

    /**
     * sizeof this struct
     */
    uint16_t size;

    /**
     * which integrator to use
     */
    Integrator integrator;

    /**
     * Set of options to use when configuring the integrator.
     * This is a bitfield composed of options from the IntegratorOpt enum.
     */
    uint32_t integratorOpt;

    uint32_t modelGeneratorOpt;
};

/**
 * RoadRunner simulation options.
 */
struct RR_DECLSPEC SimulateOptions
{
    SimulateOptions();

    double startTime;
    double endTime;
    int numberOfDataPoints;

};


} /* namespace rr */
#endif /* RRROADRUNNEROPTIONS_H_ */
