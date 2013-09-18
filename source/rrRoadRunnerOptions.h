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
         *
         * If only a single instance of a model is run, there is no
         * need to cache the models, and this can safetly be enabled,
         * realizing some performance gains.
         */
        ForceReCompile                  = (0x1 << 1), // => 0x00000010

        /**
         * If this is set, then a read-only model is generated. A read-only
         * model can be simulated, but no code is generated to set model
         * values, i.e. parameters, amounts, values, etc...
         *
         * It takes a finite amount of time to generate the model value setting
         * functions, and if they are not needed, one may see some performance
         * gains, especially in very large models.
         */
        ReadOnlyModel                   = (0x1 << 2)  // => 0x00000100
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
    enum Options
    {
        /**
         * reset the model to the initial state.
         */
        ResetModel = (0x1 << 0), // => 0x00000001
    };

    /**
     * init with default options.
     */
    SimulateOptions();

    double startTime;
    double endTime;
    int nDataPoints;

    /**
     * a bitmask of the options specified in the Options enumeration.
     */
    uint32_t flags;
};


} /* namespace rr */
#endif /* RRROADRUNNEROPTIONS_H_ */
