Controlling Simulation Settings
______________________________

.. class:: SimulateOptions(*args)
   :module: roadrunner

   The SimulateOptions object allows tuning a variety of simulation and integration 
   options such as tolerances and time step size. 
   
   This is the full set of options that determines how RoadRunner performs
   a simulation of an SBML model.
   
   This is a superset of the values stored in a SBML test suite settings file, the
   documentation of the fields which correspond to an SBML test suite settings was
   taken from http://sbml.org
   
   This object can be read from a SBML test suite options file by using a file
   name in the constructor.
   
   :param sbmlSettingFilePath: (optional) if this is given, the settings are read 
                               from this settings file, if not, the default values are set.
   
.. attribute:: SimulateOptions.integratorFlags
   :module: roadrunner

   A bitfield which may contain the following options. In python these options are
   also available as separate properties which set the integratorFlags bitfield.

.. attribute:: SimulateOptions.STIFF
   :module: roadrunner

   Use the stiff (implicit) integrator. Defaults to off. 

.. attribute:: SimulateOptions.MULTI_STEP
   :module: roadrunner
   
   * Experimental *

   Perform a multi-step simulation. In multi-step simulation, one may monitor
   the variable time stepping via the IntegratorListener events system. 

   
.. attribute:: SimulateOptions.absolute
   :module: roadrunner
            
   A number representing the absolute difference permitted for the integrator
   tolerance.
      
   
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
   duration accordingly and visa versa.
      
   
.. attribute:: SimulateOptions.flags
   :module: roadrunner
      
   
   can be set to ResetModel so that the model is reset to its initial state
   when the simulation is run. 
      
   
.. attribute:: SimulateOptions.relative
   :module: roadrunner
      
   
   A float-point number representing the relative difference permitted.
   Defaults 0.0001
      
   
.. attribute:: SimulateOptions.resetModel
   :module: roadrunner
      
   
   Causes the model to be reset to the original conditions specified
   in the SBML when the simulation is run.
      
   
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
   
   The ordering of the symbols in variable is what determines the output 
   ordering. The order of symbols in either amounts or concentrations do not
   effect the output ordering.
      
   NOTE:If a listed variable has two underscores in it ('__'), that variable
   is actually present only in a submodel of the main model, from the
   Hierarchical Model Composition package, in the format submodelID__variableID.
   If the model is flattened, the variable will appear automatically.

.. attribute:: SimulateOptions.initialTimeStep
   :module: roadrunner

   A user specified initial time step. If this is <=  0, the integrator
   will attempt to determine a safe initial time step.

   Note, for each number of steps given to RoadRunner.simulate or RoadRunner.integrate
   the internal integrator may take many many steps to reach one of the external time
   steps. This value specifies an initial value for the internal integrator
   time step.
     
.. attribute:: SimulateOptions.minimumTimeStep
   :module: roadrunner

   Specify the minimum time step that the internal integrator
   will use. Uses integrator estimated value if <= 0.
     
.. attribute:: SimulateOptions.maximumTimeStep
   :module: roadrunner

   Specify the maximum time step size that the internal integrator
   will use. Uses integrator estimated value if <= 0.


.. attribute:: SimulateOptions.maximumNumSteps
   :module: roadrunner

   Specify the maximum number of steps the internal integrator will use
   before reaching the user specified time span. Uses the integrator
   default value if <= 0.

