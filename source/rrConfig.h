/*
 * rrConfig.h
 *
 *  Created on: Mar 24, 2014
 *      Author: andy
 */

#ifndef RRCONFIG_H_
#define RRCONFIG_H_

#include <string>
#include "rrExporter.h"

namespace rr
{

/**
 * read or store default values.
 */
class RR_DECLSPEC Config
{
public:

    enum Keys {
        /**
         * perform conservation analysis.
         *
         * This causes a re-ordering of the species, so results generated
         * with this flag enabled can not be compared index wise to results
         * generated otherwise.
         *
         * currently only implemented with the C code generating model
         */
        LOADSBMLOPTIONS_CONSERVED_MOIETIES = 0,

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
        LOADSBMLOPTIONS_RECOMPILE,

        /**
         * If this is set, then a read-only model is generated. A read-only
         * model can be simulated, but no code is generated to set model
         * values, i.e. parameters, amounts, values, etc...
         *
         * It takes a finite amount of time to generate the model value setting
         * functions, and if they are not needed, one may see some performance
         * gains, especially in very large models.
         */
        LOADSBMLOPTIONS_READ_ONLY,

        /**
         * Generate accessor functions to allow changing of initial
         * conditions.
         */
        LOADSBMLOPTIONS_MUTABLE_INITIAL_CONDITIONS,

        /**
         * GVN - This pass performs global value numbering and redundant load
         * elimination cotemporaneously.
         */
        LOADSBMLOPTIONS_OPTIMIZE_GVN,

        /**
         * CFGSimplification - Merge basic blocks, eliminate unreachable blocks,
         * simplify terminator instructions, etc...
         */
        LOADSBMLOPTIONS_OPTIMIZE_CFG_SIMPLIFICATION,

        /**
         * InstructionCombining - Combine instructions to form fewer, simple
         * instructions. This pass does not modify the CFG, and has a tendency to make
         * instructions dead, so a subsequent DCE pass is useful.
         */
        LOADSBMLOPTIONS_OPTIMIZE_INSTRUCTION_COMBINING,

        /**
         * DeadInstElimination - This pass quickly removes trivially dead instructions
         * without modifying the CFG of the function.  It is a BasicBlockPass, so it
         * runs efficiently when queued next to other BasicBlockPass's.
         */
        LOADSBMLOPTIONS_OPTIMIZE_DEAD_INST_ELIMINATION,

        /**
         * DeadCodeElimination - This pass is more powerful than DeadInstElimination,
         * because it is worklist driven that can potentially revisit instructions when
         * their other instructions become dead, to eliminate chains of dead
         * computations.
         */
        LOADSBMLOPTIONS_OPTIMIZE_DEAD_CODE_ELIMINATION,

        /**
         * InstructionSimplifier - Remove redundant instructions.
         */
        LOADSBMLOPTIONS_OPTIMIZE_INSTRUCTION_SIMPLIFIER,

        /**
         * Use the LLVM MCJIT JIT engine.
         *
         * Defaults to false.
         *
         * The MCJIT is the new LLVM JIT engine, it is not as well tested as the
         * original JIT engine. Does NOT work on LLVM 3.1
         */
        LOADSBMLOPTIONS_USE_MCJIT,


        /**
         * The number of steps at which the output is sampled. The samples are evenly spaced.
         * When a simulation system calculates the data points to record, it will typically
         * divide the duration by the number of time steps. Thus, for X steps, the output
         * will have X+1 data rows.
         * see SimulateOptions::steps

         */

        SIMULATEOPTIONS_STEPS,

        /**
         * see SimulateOptions::duration
         */
        SIMULATEOPTIONS_DURATION,

        /**
         * see SimulateOptions::absolute
         */
        SIMULATEOPTIONS_ABSOLUTE,

        /**
         * see SimulateOptions::relative
         */
        SIMULATEOPTIONS_RELATIVE,

        /**
         * see SimulateOptions::STRUCTURED_RESULT
         */
        SIMULATEOPTIONS_STRUCTURED_RESULT,

        /**
         * Is the model a stiff system? setting this to stiff causes
         * RoadRunner to load a stiff solver which could potentially be
         * extremly slow
         * see SimulateOptions::STIFF
         */
        SIMULATEOPTIONS_STIFF,

        /**
         * The MULTI_STEP option tells the solver to take a series of internal steps
         * and then return the solution at the point reached by that step.
         *
         * In simulate, this option will likely be slower than normal mode,
         * but may be useful to monitor solutions as they are integrated.
         *
         * This is intended to be used in combination with the
         * IntegratorListener. It this option is set, and there is a
         * IntegratorListener set, RoadRunner::integrate will run the
         * integrator in a series of internal steps, and the listner
         * will by notified at each step.
         *
         * Highly Experimental!!!
         * see SimulateOptions::MULTI_STEP
         */
        SIMULATEOPTIONS_MULTI_STEP,

        /**
         * A useer specified initial time step. If this is <=  0, the integrator
         * will attempt to determine a safe initial time stpe.
         *
         * Note, for each number of steps given to RoadRunner::simulate or RoadRunner::oneStep,
         * the internal integrator may take many many steps to reach one of the external time
         * steps. This value specifies an initial value for the internal integrator
         * time step.
         * see SimulateOptions::initialTimeStep
         */
        SIMULATEOPTIONS_INITIAL_TIMESTEP,

        /**
         * Specfify The Minimum Time Step That The Internal Integrator
         * Will Use. Uses Integrator Estimated Value If <= 0.
         * see SimulateOptions::minumumTimeStep
         */
        SIMULATEOPTIONS_MINIMUM_TIMESTEP,

        /**
         * Specify The Maximum Time Step Size That The Internaal Integrator
         * Will Use. Uses Integrator Estimated Value If <= 0.
         * see SimulateOptions::maximumTimeStep
         */
        SIMULATEOPTIONS_MAXIMUM_TIMESTEP,

        /**
         * Specify The Maximum Number Of Steps The Internal Integrator Will Use
         * Before Reaching The User Specified Time Span. Uses The Integrator
         * Default Value If <= 0.
         * see SimulateOptions::maximumNumSteps
         */
        SIMULATEOPTIONS_MAXIMUM_NUM_STEPS,


        /**
         * disable SBML conserved moiety warnings.
         */
        ROADRUNNER_DISABLE_WARNINGS,

        /**
         * Needs to be the last item in the enum, no mater how many
         * other items are added, this is used internally to create
         * a static array.
         */
        CONFIG_END
    };

    static std::string getString(Keys);

    static int getInt(Keys);

    static double getDouble(Keys);

    static std::string getConfigFilePath();

    static void setValue(Keys, const std::string& value);

    static void setValue(Keys, int);

    static void setValue(Keys, double);

    static void setValue(Keys, bool);

    static void readConfigFile(const std::string& path);

    static void writeConfigFile(const std::string& path);
};

} /* namespace rr */

#endif /* RRCONFIG_H_ */
