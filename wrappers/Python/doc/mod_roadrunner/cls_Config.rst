Configuration
_____________

Many of RoadRunner classes use a number of configuration parameters. Most of these can be set using
the Config class. The values stored in the Config class only determine the *default* values of
parameters. Most methods let specify explicit values for parameters. For example, if one ran a time
series simulation, they could specify various parameter via the optional SimulateOptions object::

  import roadrunner
  opt = roadrunner.SimulateOptions()
  opt.relative = 1.3e-5
  opt.absolute = 3e-12
  opt.steps = 100
  opt.duration = 1.1
  r = roadrunner.RoadRunner('/Users/andy/test.xml')
  r.simulate(opt)

If no SimulateOptions object is given, then the default configuration parameters are used. The
Config class will look in the following locations for the config file, and will load the values from
the first config file it finds. If it does not find a config file in one of the following locations,
a default set of configuration parameters are used. The search locations of the config file are:

#1: the ROADRUNNER_CONFIG environment variable

#2: try the user's home directory for roadrunner.conf, i.e.::
  
  /Users/andy/roadrunner.conf

#3: try the user's home directory for .roadrunner.conf, i.e.::
  
  /Users/andy/.roadrunner.conf

#4: try the same directory as the roadrunner shared library, this will be the same directory as the python _roadrunner.pyd
python extension module, i.e.::
  
  /Users/andy/local/lib/roadrunner.conf

#5: try one directory up from the where the shared library or program is at, i.e.::
  
  /Users/andy/local/roadrunner.conf

The conf file is just a plain text file of where each line may be key / value pair separated by a
":", i.e. ::

  KEY_NAME : Value

Any line that does not match this format is ignored, and keys that are not found are also
ignored. Therefore, any line that does not start with a word character is considered a comment.

All of the configuration management functions are static method of the Config class, and 
all of the configuration keys are static attributes of the Config class, these are documented in the 
Configuration Functions section. 

As all of the Config class methods are static, one never instantiates the Config class. 

Configuration Functions
-----------------------

.. staticmethod:: Config.setValue(key, value)
   :module: roadrunner

   Set the value of a configuration key. The value must be either a string, integer, double or
   boolean. If one wanted to change the value of the default integrator tolerances, one would::

     from roadrunner import Config
     Config.setValue(Config.SIMULATEOPTIONS_ABSOLUTE, 3.14e-12)
     Config.setValue(Config.SIMULATEOPTIONS_RELATIVE, 2.78e-5)


   Or, other options may be set to Boolean or integer values. To enable an optimization features,
   or to set default simulation time steps::

     Config.setValue(Config.LOADSBMLOPTIONS_OPTIMIZE_INSTRUCTION_SIMPLIFIER, True)
     Config.setValue(Config.SIMULATEOPTIONS_STEPS, 100)


.. staticmethod:: Config.getConfigFilePath()
   :module: roadrunner

   If roadrunner was able to find a configuration file on the file system, its full path is returned
   here. If no file was found, this returns a empty string. 


.. staticmethod:: Config.readConfigFile(path)
   :module: roadrunner

   Read all of the values from a configuration file at the given path. This overrides any previously
   stored configuration. This allows users to have any number of configuration files and load them
   at any time. Say someone had to use Windows, and they had a file in thier C: drive, this would be
   loaded via::
     
     Config.readConfigFile("C:/my_config_file.txt")

   Note, the forward slash works on both Unix and Windows, using the forward slash eliminates the
   need to use a double back slash, "\\\\".

.. staticmethod:: Config.writeConfigFile(path)
   :module: roadrunner

   Write all of the current configuration values to a file. This could be written to one of the
   default locations, or to any other location, and re-loaded at a later time. 



Available Configuration Parameters
----------------------------------
All of the configuration parameter keys are static attributes of the Config class and are listed
here. The variable type of the parameter is listed after the key name. 


.. attribute:: Config.LOADSBMLOPTIONS_CONSERVED_MOIETIES
   :module: roadrunner
   :annotation: bool

   perform conservation analysis.
      
   This causes a re-ordering of the species, so results generated
   with this flag enabled can not be compared index wise to results
   generated otherwise.

   Moiety conservation is only compatable with simple models which do NOT have any events or rules
   which define or alter any floating species, and which have simple constant stoichiometries. 

   Moiety conservation may cause unexpected results, be aware of what it is before enableing. 

   Not recommended for time series simulations.
  


.. attribute:: Config.LOADSBMLOPTIONS_RECOMPILE
   :module: roadrunner
   :annotation: bool

   Should the model be recompiled?
   The LLVM ModelGenerator maintains a hash table of currently running
   models. If this flag is NOT set, then the generator will look to see
   if there is already a running instance of the given model and
   use the generated code from that one.
  
   If only a single instance of a model is run, there is no
   need to cache the models, and this can safely be enabled,
   realizing some performance gains.


.. attribute:: Config.LOADSBMLOPTIONS_READ_ONLY
   :module: roadrunner
   :annotation: bool

   If this is set, then a read-only model is generated. A read-only
   model can be simulated, but no code is generated to set model
   values, i.e. parameters, amounts, values, etc...
  
   It takes a finite amount of time to generate the model value setting
   functions, and if they are not needed, one may see some performance
   gains, especially in very large models.


.. attribute:: Config.LOADSBMLOPTIONS_MUTABLE_INITIAL_CONDITIONS
   :module: roadrunner
   :annotation: bool

   Generate accessors functions to allow changing of initial
   conditions.



.. attribute:: Config.LOADSBMLOPTIONS_OPTIMIZE_GVN
   :module: roadrunner
   :annotation: bool

   GVN - This pass performs global value numbering and redundant load
   elimination cotemporaneously.




.. attribute:: Config.LOADSBMLOPTIONS_OPTIMIZE_CFG_SIMPLIFICATION
   :module: roadrunner
   :annotation: bool

   CFGSimplification - Merge basic blocks, eliminate unreachable blocks,
   simplify terminator instructions, etc...




.. attribute:: Config.LOADSBMLOPTIONS_OPTIMIZE_INSTRUCTION_COMBINING
   :module: roadrunner
   :annotation: bool

   InstructionCombining - Combine instructions to form fewer, simple
   instructions. This pass does not modify the CFG, and has a tendency to make
   instructions dead, so a subsequent DCE pass is useful.




.. attribute:: Config.LOADSBMLOPTIONS_OPTIMIZE_DEAD_INST_ELIMINATION
   :module: roadrunner
   :annotation: bool

   DeadInstElimination - This pass quickly removes trivially dead instructions
   without modifying the CFG of the function.  It is a BasicBlockPass, so it
   runs efficiently when queued next to other BasicBlockPass's.




.. attribute:: Config.LOADSBMLOPTIONS_OPTIMIZE_DEAD_CODE_ELIMINATION
   :module: roadrunner
   :annotation: bool

   DeadCodeElimination - This pass is more powerful than DeadInstElimination,
   because it is worklist driven that can potentially revisit instructions when
   their other instructions become dead, to eliminate chains of dead
   computations.


.. attribute:: Config.LOADSBMLOPTIONS_OPTIMIZE_INSTRUCTION_SIMPLIFIER
   :module: roadrunner
   :annotation: bool


   InstructionSimplifier - Remove redundant instructions.


.. attribute:: Config.LOADSBMLOPTIONS_USE_MCJIT
   :module: roadrunner
   :annotation: bool

   Currently disabled. 

   Use the LLVM MCJIT JIT engine.
  
   Defaults to false.
  
   The MCJIT is the new LLVM JIT engine, it is not as well tested as the
   original JIT engine. Does NOT work on LLVM 3.1



.. attribute:: Config.SIMULATEOPTIONS_STEPS
   :module: roadrunner
   :annotation: int

   The number of steps at which the output is sampled. The samples are evenly spaced.
   When a simulation system calculates the data points to record, it will typically
   divide the duration by the number of time steps. Thus, for N steps, the output
   will have N+1 data rows.


.. attribute:: Config.SIMULATEOPTIONS_DURATION
   :module: roadrunner
   :annotation: double

   The duration of the simulation run, in the model's units of time. Note, 
   setting the duration automatically sets the end time and visa versa.


.. attribute:: Config.SIMULATEOPTIONS_ABSOLUTE
   :module: roadrunner
   :annotation: double

   The absolute error tolerance used by the integrator. 

   A number representing the absolute difference permitted for the integrator
   tolerance. Defaults to 1.000000e-10.


.. attribute:: Config.SIMULATEOPTIONS_RELATIVE
   :module: roadrunner
   :annotation: double

   The relative error tolerance used by the integrator. 

   A float-point number representing the relative difference permitted.
   Defaults to 1.000000e-05.


.. attribute:: Config.SIMULATEOPTIONS_STRUCTURED_RESULT
   :module: roadrunner
   :annotation: bool

   A boolean option to return a structured array from the RoadRunner.simulate method. Structured
   arrays contain column names and other data. A structured array needs to be converted into regular
   arrays before they can be used in numpy functions. 



.. attribute:: Config.SIMULATEOPTIONS_STIFF
   :module: roadrunner
   :annotation: bool

   Is the model a stiff system? setting this to stiff causes
   RoadRunner to load a stiff solver which could potentially be
   extremly slow



.. attribute:: Config.SIMULATEOPTIONS_MULTI_STEP
   :module: roadrunner
   :annotation: bool

   The MULTI_STEP option tells the solver to take a series of internal steps
   and then return the solution at the point reached by that step.
  
   In simulate, this option will likely be slower than normal mode,
   but may be useful to monitor solutions as they are integrated.
  
   This is intended to be used in combination with the
   IntegratorListener. It this option is set, and there is a
   IntegratorListener set, RoadRunner::integrate will run the
   integrator in a series of internal steps, and the listner
   will by notified at each step.
  
   Highly Experimental!!!


.. attribute:: Config.SIMULATEOPTIONS_INITIAL_TIMESTEP
   :module: roadrunner
   :annotation: double

   A user specified initial time step. If this is <=  0, the integrator
   will attempt to determine a safe initial time step.
  
   Note, for each number of steps given to RoadRunner::simulate or RoadRunner::oneStep,
   the internal integrator may take many many steps to reach one of the external time
   steps. This value specifies an initial value for the internal integrator
   time step.


.. attribute:: Config.SIMULATEOPTIONS_MINIMUM_TIMESTEP
   :module: roadrunner
   :annotation: double

   Specify The Minimum Time Step That The Internal Integrator
   Will Use. Uses Integrator Estimated Value If <= 0.



.. attribute:: Config.SIMULATEOPTIONS_MAXIMUM_NUM_STEPS
   :module: roadrunner
   :annotation: int

   Specify The Maximum Number Of Steps The Internal Integrator Will Use
   Before Reaching The User Specified Time Span. Uses The Integrator
   Default Value If <= 0.



.. attribute:: Config.ROADRUNNER_DISABLE_PYTHON_DYNAMIC_PROPERTIES,
   :module: roadrunner
   :annotation: int

   RoadRunner by default dynamically generates accessors properties
   for all SBML symbol names on the model object when it is retrieved
   in Python. This feature is very nice for interactive use, but
   can slow things down. If this feature is not needed, it
   can be disabled here.
         

.. attribute:: Config.ROADRUNNER_DISABLE_WARNINGS,
   :module: roadrunner
   :annotation: int

   disable SBML conserved moiety warnings.

   Conserved Moiety Conversion may cause unexpected behavior, be aware of what it
   is before enabling. 

   RoadRunner will issue a warning in steadyState if conservedMoieties are NOT 
   enabled because of a potential singular Jacobian. To disable this warning, 
   set this value to 1

   A notice will be issued whenever a document is loaded and conserved moieties 
   are enabled. To disable this notice, set this value to 2.

   To disable both the warning and notice, set this value to 3

   Rationale for these numbers: This is actual a bit field, disabling the steady state 
   warning value is actually 0b01 << 0 which is 1, and the loading warning is 0b01 << 1 
   which is 2 and 0b01 & 0b10 is 0b11 which is 3 in decimal. 




