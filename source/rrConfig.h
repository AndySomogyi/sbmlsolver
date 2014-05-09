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
#include "Variant.h"

namespace rr
{

/**
 * read or store default values.
 *
 * Many of RoadRunner classes use a number of configration parameters. Most of these can be set
 * using the Config class. The values stored in the Config class only determine the defaut values of
 * parameters.
 *
 * The Config class will look in the following locations for the config file, and will load the
 * values from the first config file it finds. If it does not find a config file in one of the
 * following locations, a default set of configuration parameters are used. The search locations of
 * the config file are:
 *
 * #1: the ROADRUNNER_CONFIG environment variable
 *
 * #2: try the user's home directory for roadrunner.conf, i.e.:
 *
 * /Users/andy/roadrunner.conf
 *
 * #3: try the user's home directory for .roadrunner.conf, i.e.:
 *
 * /Users/andy/.roadrunner.conf
 *
 * #4: try the same directory as the roadrunner shared library, this
 * will be the same directory as the python _roadrunner.pyd python extension module, i.e.:
 *
 * /Users/andy/local/lib/roadrunner.conf
 *
 * #5: try one directory up from the where the shared library or program is at, i.e.:
 *
 * /Users/andy/local/roadrunner.conf
 *
 * The conf file is just a plain text file of where each line may
 * be key / value pair separated by a �":", i.e.
 *
 * KEY_NAME : Value Any line that does not match this format is ignored, and keys that are not found
 * are also ignored. Therefore, any line that does not start w* ith a word character is considered a
 * comment.
 *
 * All of the configuration managment functions are static method of the Config class, and all of
 * the configuration keys are static attributes * of the Config class, these are documented in the
 * Configuration Functions section.
 *
 * As all of the Config class methods are static, one never instantiates the Config class.
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
         * Do variable time step integration when using a deterministic
         * integrator
         */
        SIMULATEOPTIONS_DETERMINISTIC_VARIABLE_STEP,


        /**
         * Do variable time step integration when using stochastic
         * integrators.
         */
        SIMULATEOPTIONS_STOCHASTIC_VARIABLE_STEP,

        /**
         * Default integrator to use
         */
        SIMULATEOPTIONS_INTEGRATOR,

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
         * Disable SBML conserved moiety warnings.
         *
         * this disables warnings based on the following bitfields:
         *
         * ROADRUNNER_DISABLE_WARNINGS_STEADYSTATE: disable the warning in
         * RoadRunner::steadyState about possible singular jacobian.
         *
         * ROADRUNNER_DISABLE_WARNINGS_CONSERVED_MOIETY: disable the
         * conserved moieties warning if enabled when loading a sbml document.
         */
        ROADRUNNER_DISABLE_WARNINGS,

        /**
         * RoadRunner by default dynamically generates accessor properties
         * for all sbml symbol names on the model object when it is retrieved
         * in Python. This feature is very nice for interactive use, but
         * can slow things down. If this feature is not needed, it
         * can be disabled here.
         */
        ROADRUNNER_DISABLE_PYTHON_DYNAMIC_PROPERTIES,


        /**
         * a hex value consisting of a combination of the following values:
         *
         * IdCheckON         0x01
         * IdCheckOFF        0xfe
         * SBMLCheckON       0x02
         * SBMLCheckOFF      0xfd
         * SBOCheckON        0x04
         * SBOCheckOFF       0xfb
         * MathCheckON       0x08
         * MathCheckOFF      0xf7
         * UnitsCheckON      0x10
         * UnitsCheckOFF     0xef
         * OverdeterCheckON  0x20
         * OverdeterCheckOFF 0xdf
         * PracticeCheckON   0x40
         * PracticeCheckOFF  0xbf
         * AllChecksON       0x7f
         *
         * The default value is AllChecksON & UnitsCheckOFF
         */
        SBML_APPLICABLEVALIDATORS,

        /**
         * default step size for Jabobian finite differece calculations
         */
        ROADRUNNER_JACOBIAN_STEP_SIZE,

        /**
         * Needs to be the last item in the enum, no mater how many
         * other items are added, this is used internally to create
         * a static array.
         */
        CONFIG_END
    };

    enum ROADRUNNER_DISABLE_WARNINGS_VALUES
    {
        ROADRUNNER_DISABLE_WARNINGS_STEADYSTATE          =  (0x1 << 0),  // => 0x00000001
        ROADRUNNER_DISABLE_WARNINGS_CONSERVED_MOIETY     =  (0x1 << 1)   // => 0x00000010
    };


    /**
     * read the config value as a string.
     */
    static std::string getString(Keys);

    /**
     * read the config value as an integer.
     */
    static int getInt(Keys);

    static bool getBool(Keys);

    /**
     * read the config value as a double.
     */
    static double getDouble(Keys);

    /**
     * If a config file was found in one of the above locations, its full path is returned here.
     * Otherwise, if no file was found, an empty string is returned.
     */
    static std::string getConfigFilePath();

    /**
     * set the value of a config key.
     * note, this value is only used in any new objects created after it has been set.
     */

    static void setValue(Keys, const Variant& value);

    static const Variant& getValue(Keys);

    /**
     * Read all of the values stored in a configuration file and set all the keys
     * to these values.
     */
    static void readConfigFile(const std::string& path);

    /**
     * write the currently set configuration values to a config file. The contents of the
     * file are cleared and the new values written.
     */
    static void writeConfigFile(const std::string& path);
};

} /* namespace rr */

#endif /* RRCONFIG_H_ */
