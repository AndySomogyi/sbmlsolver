/*
 * rrRoadRunnerOptions.h
 *
 *  Created on: Sep 9, 2013
 *      Author: andy
 */

#ifndef RRROADRUNNEROPTIONS_H_
#define RRROADRUNNEROPTIONS_H_

#include "rrExporter.h"
#include <string>
#include <vector>

#if defined(_MSC_VER)
#include "msc_stdint.h"
#else
#include <stdint.h>
#endif

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
        CONSERVED_MOIETIES               = (0x1 << 0),  // => 0x00000001

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
        RECOMPILE                        = (0x1 << 1),  // => 0x00000010

        /**
         * If this is set, then a read-only model is generated. A read-only
         * model can be simulated, but no code is generated to set model
         * values, i.e. parameters, amounts, values, etc...
         *
         * It takes a finite amount of time to generate the model value setting
         * functions, and if they are not needed, one may see some performance
         * gains, especially in very large models.
         */
        READ_ONLY                        = (0x1 << 2),  // => 0x00000100

        /**
         * Generate accessor functions to allow changing of initial
         * conditions.
         */
        MUTABLE_INITIAL_CONDITIONS       = (0x1 << 3),   // => 0x00001000

        /**
         * GVN - This pass performs global value numbering and redundant load
         * elimination cotemporaneously.
         */
        OPTIMIZE_GVN                     = (0x1 << 4),

        /**
         * CFGSimplification - Merge basic blocks, eliminate unreachable blocks,
         * simplify terminator instructions, etc...
         */
        OPTIMIZE_CFG_SIMPLIFICATION      = (0x1 << 5),

        /**
         * InstructionCombining - Combine instructions to form fewer, simple
         * instructions. This pass does not modify the CFG, and has a tendency to make
         * instructions dead, so a subsequent DCE pass is useful.
         */
        OPTIMIZE_INSTRUCTION_COMBINING   = (0x1 << 6),

        /**
         * DeadInstElimination - This pass quickly removes trivially dead instructions
         * without modifying the CFG of the function.  It is a BasicBlockPass, so it
         * runs efficiently when queued next to other BasicBlockPass's.
         */
        OPTIMIZE_DEAD_INST_ELIMINATION   = (0x1 << 7),

        /**
         * DeadCodeElimination - This pass is more powerful than DeadInstElimination,
         * because it is worklist driven that can potentially revisit instructions when
         * their other instructions become dead, to eliminate chains of dead
         * computations.
         */
        OPTIMIZE_DEAD_CODE_ELIMINATION   = (0x1 << 8),

        /**
         * InstructionSimplifier - Remove redundant instructions.
         */
        OPTIMIZE_INSTRUCTION_SIMPLIFIER  = (0x1 << 9)
    };

    enum LoadOpt
    {
        /**
         * Do not create a default selection list when the model is loaded.
         */
        NO_DEFAULT_SELECTIONS           = (0x1 << 0)   // => 0x00000001
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


    uint32_t modelGeneratorOpt;

    uint32_t loadFlags;
};

/**
 * RoadRunner simulation options.
 *
 * This is the full set of options that determines how RoadRunner performs
 * a simulation of an sbml model.
 *
 * This is a superset of the values stored in a sbml test suite settings file, the
 * documentation of the fields which correspond to an sbml test suite settings was
 * taken from http://sbml.org
 */
struct RR_DECLSPEC SimulateOptions
{
    enum Options
    {
        /**
         * reset the model to the initial state.
         */
        RESET_MODEL             = (0x1 << 0), // => 0x00000001

        /**
         * Simulate should return a raw result matrix without
         * adding any column names.
         */
        STRUCTURED_RESULT       = (0x1 << 1)  // => 0x00000010
    };

    /**
     * the list of ODE solvers RoadRunner currently supports.
     */
    enum Integrator
    {
        CVODE
    };

    enum IntegratorFlags
    {
        /**
         * Is the model a stiff system? setting this to stiff causes
         * RoadRunner to load a stiff solver which could potentially be
         * extremly slow
         */
        STIFF = (0x1 << 0), // => 0x00000001
    };

    /**
     * init with default options.
     */
    SimulateOptions();

    /**
     * Set the default flags, but the fields start, duration, absolute, relative, variables,
     * amounts and concentrations are loaded from an sbml test suite settings file.
     */
    SimulateOptions(const std::string& sbmlSettingFilePath);

    /**
     * a bitmask of the options specified in the Options enumeration.
     */
    uint32_t flags;

    /**
     * which integrator to use
     */
    Integrator integrator;

    /**
     * Set of options to use when configuring the integrator.
     * This is a bitfield composed of options from the IntegratorOpt enum.
     */
    uint32_t integratorFlags;

    /**
     * The number of steps at which the output is sampled. The samples are evenly spaced.
     * When a simulation system calculates the data points to record, it will typically
     * divide the duration by the number of time steps. Thus, for X steps, the output
     * will have X+1 data rows.
     */
    int steps;

    /**
     * The start time of the simulation time-series data.
     * Often this is 0, but not necessarily.
     */
    double start;

    /**
     * The duration of the simulation run, in the model's units of time.
     */
    double duration;

    /**
     * A number representing the absolute difference permitted.
     */
    double absolute;

    /**
     * A float-point number representing the relative difference permitted.
     * Defaults 0.0001
     */
    double relative;

    /**
     * The variables (in addition to time) whose values will be saved in the result.
     * These are SBML model id's. Order is significant, as this determines the order
     * of the columns in the result matrix.
     *
     * Important: if a symbol in this list refers to a species in the model,
     * then that symbol will also be listed in either the amount or concentration
     * lists below.
     *
     * NOTE:If a listed variable has two underscores in it ('__'), that variable
     * is actually present only in a submodel of the main model, from the
     * Hierarchical Model Composition package, in the format submodelID__variableID.
     * If the model is flattened, the variable will appear automatically.
     */
    std::vector<std::string> variables;

    /**
     * A list of the variable whose output in the results file is in amount
     * (not concentration) units. This list of variables must be a subset of
     * the names listed in variables.
     */
    std::vector<std::string> amounts;

    /**
     * A list of the variable whose output in the results file is in concentration
     * (not amount) units. This list of variables must be a subset of the names
     * listed in variables.
     */
    std::vector<std::string> concentrations;

    /**
     * The minumum relative error that the CVODE integrator supports
     * in order to to pass the sbml test suite using the default integtator.
     *
     * If a test suite config file is loaded, and the relative error is
     * higher than MIN_RELATIVE, it will be lowered to MIN_RELATIVE.
     */
    static const double MIN_RELATIVE;

    /**
     * The minumum absolute error that the CVODE integrator supports
     * in order to to pass the sbml test suite using the default integtator.
     *
     * If a test suite config file is loaded, and the relative error is
     * higher than MIN_ABSOLUTE, it will be lowered to MIN_ABSOLUTE.
     */
    static const double MIN_ABSOLUTE;
};



/**
 * A set of options that determine how the top level RoadRunner class
 * should behave.
 */
struct RR_DECLSPEC RoadRunnerOptions
{
    enum Options
    {
        /**
         * RoadRunner by default dynamically generates accessor properties
         * for all sbml symbol names on the model object when it is retrieved
         * in Python. This feature is very nice for interactive use, but
         * can slow things down. If this feature is not needed, it
         * can be disabled here.
         */
        DISABLE_PYTHON_DYNAMIC_PROPERTIES             = (0x1 << 0), // => 0x00000001
    };

    /**
     * a bitmask of the options specified in the Options enumeration.
     */
    uint32_t flags;

    RoadRunnerOptions() : flags(0)
    {
    }
};


} /* namespace rr */
#endif /* RRROADRUNNEROPTIONS_H_ */
