Configuration
_____________

Many of RoadRunner classes use a number of configration parameters. Most of these can be set using
the Config class. 

The Config class will look in the following locations for the config file:

#1: the ROADRUNNER_CONFIG environment variable


#2: try the users's home directory for roadrunner.conf, i.e.::
  
  /Users/andy/roadrunner.conf

#3: try the users's home directory for .roadrunner.conf, i.e.::
  
  /Users/andy/.roadrunner.conf

#4: try the same directory as the roadrunner shared library, this will be the same directory as the python _roadrunner.pyd
python extension module, i.e.::
  
  /Users/andy/local/lib/roadrunner.conf

#4: try one directory up from the where the shared library or program is at, i.e.::
  
  /Users/andy/local/roadrunner.conf

The conf file is just a plain text file of where each line may be key / value pair separated by a
":", i.e. ::

  KEY_NAME : Value

Any line that does not match this format is ignored, and keys that are not found are also
ignored. Therefore, any line that does not start with a word character is considered a comment.

.. class:: Config
   :module: roadrunner


Available Configuration Parameters
----------------------------------

.. attribute:: Config.LOADSBMLOPTIONS_CONSERVED_MOIETIES
   :module: roadrunner

   perform conservation analysis.
      
   This causes a re-ordering of the species, so results generated
   with this flag enabled can not be compared index wise to results
   generated otherwise.

   Moiety conservation is only compatable with simple models which do NOT have any events or rules
   which define or alter any floating species, and which have simple constant stiochiometries. 
  




.. attribute:: Config.LOADSBMLOPTIONS_RECOMPILE
   :module: roadrunner

   Should the model be recompiled?
   The LLVM ModelGenerator maintins a hash table of currently running
   models. If this flag is NOT set, then the generator will look to see
   if there is already a running instance of the given model and
   use the generated code from that one.
  
   If only a single instance of a model is run, there is no
   need to cache the models, and this can safetly be enabled,
   realizing some performance gains.


.. attribute:: Config.LOADSBMLOPTIONS_READ_ONLY
   :module: roadrunner

   If this is set, then a read-only model is generated. A read-only
   model can be simulated, but no code is generated to set model
   values, i.e. parameters, amounts, values, etc...
  
   It takes a finite amount of time to generate the model value setting
   functions, and if they are not needed, one may see some performance
   gains, especially in very large models.


.. attribute:: Config.LOADSBMLOPTIONS_MUTABLE_INITIAL_CONDITIONS
   :module: roadrunner

   Generate accessor functions to allow changing of initial
   conditions.




.. attribute:: Config.LOADSBMLOPTIONS_OPTIMIZE_GVN
   :module: roadrunner

   GVN - This pass performs global value numbering and redundant load
   elimination cotemporaneously.




.. attribute:: Config.LOADSBMLOPTIONS_OPTIMIZE_CFG_SIMPLIFICATION
   :module: roadrunner

   CFGSimplification - Merge basic blocks, eliminate unreachable blocks,
   simplify terminator instructions, etc...




.. attribute:: Config.LOADSBMLOPTIONS_OPTIMIZE_INSTRUCTION_COMBINING
   :module: roadrunner

   InstructionCombining - Combine instructions to form fewer, simple
   instructions. This pass does not modify the CFG, and has a tendency to make
   instructions dead, so a subsequent DCE pass is useful.




.. attribute:: Config.LOADSBMLOPTIONS_OPTIMIZE_DEAD_INST_ELIMINATION
   :module: roadrunner

   DeadInstElimination - This pass quickly removes trivially dead instructions
   without modifying the CFG of the function.  It is a BasicBlockPass, so it
   runs efficiently when queued next to other BasicBlockPass's.




.. attribute:: Config.LOADSBMLOPTIONS_OPTIMIZE_DEAD_CODE_ELIMINATION
   :module: roadrunner

   DeadCodeElimination - This pass is more powerful than DeadInstElimination,
   because it is worklist driven that can potentially revisit instructions when
   their other instructions become dead, to eliminate chains of dead
   computations.




.. attribute:: Config.LOADSBMLOPTIONS_OPTIMIZE_INSTRUCTION_SIMPLIFIER
   :module: roadrunner

   InstructionSimplifier - Remove redundant instructions.




.. attribute:: Config.LOADSBMLOPTIONS_USE_MCJIT
   :module: roadrunner

   Currently disabled. 

   Use the LLVM MCJIT JIT engine.
  
   Defaults to false.
  
   The MCJIT is the new LLVM JIT engine, it is not as well tested as the
   original JIT engine. Does NOT work on LLVM 3.1






.. attribute:: Config.SIMULATEOPTIONS_STEPS
   :module: roadrunner

   The number of steps at which the output is sampled. The samples are evenly spaced.
   When a simulation system calculates the data points to record, it will typically
   divide the duration by the number of time steps. Thus, for X steps, the output
   will have X+1 data rows.
   see SimulateOptions::steps




.. attribute:: Config.SIMULATEOPTIONS_DURATION
   :module: roadrunner

   see SimulateOptions::duration




.. attribute:: Config.SIMULATEOPTIONS_ABSOLUTE
   :module: roadrunner

   see SimulateOptions::absolute




.. attribute:: Config.SIMULATEOPTIONS_RELATIVE
   :module: roadrunner

   see SimulateOptions::relative




.. attribute:: Config.SIMULATEOPTIONS_STRUCTURED_RESULT
   :module: roadrunner

   see SimulateOptions::STRUCTURED_RESULT




.. attribute:: Config.SIMULATEOPTIONS_STIFF
   :module: roadrunner

   Is the model a stiff system? setting this to stiff causes
   RoadRunner to load a stiff solver which could potentially be
   extremly slow
   see SimulateOptions::STIFF




.. attribute:: Config.SIMULATEOPTIONS_MULTI_STEP
   :module: roadrunner

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
   see SimulateOptions::MULTI_STEP




.. attribute:: Config.SIMULATEOPTIONS_INITIAL_TIMESTEP
   :module: roadrunner

   A useer specified initial time step. If this is <=  0, the integrator
   will attempt to determine a safe initial time stpe.
  
   Note, for each number of steps given to RoadRunner::simulate or RoadRunner::oneStep,
   the internal integrator may take many many steps to reach one of the external time
   steps. This value specifies an initial value for the internal integrator
   time step.
   see SimulateOptions::initialTimeStep
        



.. attribute:: Config.SIMULATEOPTIONS_MINIMUM_TIMESTEP
   :module: roadrunner

   Specfify The Minimum Time Step That The Internal Integrator
   Will Use. Uses Integrator Estimated Value If <= 0.
   see SimulateOptions::minumumTimeStep
        



.. attribute:: Config.SIMULATEOPTIONS_MAXIMUM_TIMESTEP
   :module: roadrunner

   Specify The Maximum Time Step Size That The Internaal Integrator
   Will Use. Uses Integrator Estimated Value If <= 0.
   see SimulateOptions::maximumTimeStep
        



.. attribute:: Config.SIMULATEOPTIONS_MAXIMUM_NUM_STEPS
   :module: roadrunner

   Specify The Maximum Number Of Steps The Internal Integrator Will Use
   Before Reaching The User Specified Time Span. Uses The Integrator
   Default Value If <= 0.
   see SimulateOptions::maximumNumSteps
              

.. staticmethod:: Config.getString(key)
   :module: roadrunner

   get the value of the key as a string

.. staticmethod:: Config.getInt(key)
   :module: roadrunner

   get the value of a key as an integer


.. staticmethod:: Config.getDouble(key)
   :module: roadrunner

   get the value of the key as a double


.. staticmethod:: Config.getConfigFilePath()
   :module: roadrunner

   If roadrunner was able to find a configuration file on the file system, its full path is returned
   here. If no file was found, this returns a empty string. 


.. staticmethod:: Config.setValue(key,  value)
   :module: roadrunner

   Set the value of a configuration key. The value must be either a string, integer, double or
   boolean. 


.. staticmethod:: Config.readConfigFile(path)
   :module: roadrunner

   Read all of the values from a configuration file at the given path. This overrides any previously
   stored configuration. 


.. staticmethod:: Config.writeConfigFile(path)
   :module: roadrunner

   Write all of the current configuration values to a file.

