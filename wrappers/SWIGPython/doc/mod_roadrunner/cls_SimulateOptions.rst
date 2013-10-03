
.. class:: SimulateOptions(*args)
   :module: roadrunner

   RoadRunner simulation options.
   
   This is the full set of options that determines how RoadRunner performs
   a simulation of an sbml model.
   
   This is a superset of the values stored in a sbml test suite settings file, the
   documentation of the fields which correspond to an sbml test suite settings was
   taken from http://sbml.org
   
   This object can be read from a sbml test suite options file by using a file
   name in the constructor.
   
   :param sbmlSettingFilePath: (optional) if this is given, the settings are read 
                               from this settings file, if not, the default values are set.
   
   
   
.. attribute:: SimulateOptions.absolute
   :module: roadrunner
            
   A number representing the absolute difference permitted for the integrator
   tolerence.
      
   
.. attribute:: SimulateOptions.amounts
   :module: roadrunner
      
   
   A list of the variable whose output in the results file is in amount
   (not concentration) units. This list of variables must be a subset of
   the names listed in variables.
      
   
.. attribute:: SimulateOptions.concentrations
   :module: roadrunner
      
   
   A list of the variable whose output in the results file is in concentration
   (not amount) units. This list of variables must be a subset of the names
   listed in variables.
      
   
.. attribute:: SimulateOptions.duration
   :module: roadrunner
      
   
   The duration of the simulation run, in the model's units of time. Note, 
   setting the duration automatically sets the end time and visa versa.
      
   
.. attribute:: SimulateOptions.end
   :module: roadrunner
      
   
   The simulation end time. Note, setting the end time automatically sets the 
   duration accoringly and visa versa.
      
   
.. attribute:: SimulateOptions.flags
   :module: roadrunner
      
   
   can be set to ResetModel so that the model is reset to its intial state
   when the simulation is run. 
      
   
.. attribute:: SimulateOptions.relative
   :module: roadrunner
      
   
   A float-point number representing the relative difference permitted.
   Defaults 0.0001
      
   
.. attribute:: SimulateOptions.resetModel
   :module: roadrunner
      
   
   Causes the model to be reset to the original conditions specified
   in the sbml when the simulation is run.
      
   
.. attribute:: SimulateOptions.start
   :module: roadrunner
      
   
   The start time of the simulation time-series data.
   Often this is 0, but not necessarily.
      
   
.. attribute:: SimulateOptions.steps
   :module: roadrunner
      
   
   The number of steps at which the output is sampled. The samples are evenly spaced.
   When a simulation system calculates the data points to record, it will typically
   divide the duration by the number of time steps. Thus, for X steps, the output
   will have X+1 data rows.
      
   
.. attribute:: SimulateOptions.stiff
   :module: roadrunner
      
   
   Use the stiff integrator.
      
   
.. attribute:: SimulateOptions.variables
   :module: roadrunner
      
   
   The variables (in addition to time) whose values will be saved in the result.
   These are SBML model id's. Order is significant, as this determines the order
   of the columns in the result matrix.
      
   Important: if a symbol in this list refers to a species in the model,
   then that symbol should also be listed in either the amount or concentration
   lists below. If a species symbol is listed in variables, but is not listed
   in either amounts or concentrations, then it defaults to an amount value. 
   
   The ordering of the symbols in variabls is what determines the output 
   ordering. The order of symbols in either amounts or concetrations do not
   effect the output ordering.
      
   NOTE:If a listed variable has two underscores in it ('__'), that variable
   is actually present only in a submodel of the main model, from the
   Hierarchical Model Composition package, in the format submodelID__variableID.
   If the model is flattened, the variable will appear automatically.
