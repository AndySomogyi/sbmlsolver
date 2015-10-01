Controlling Simulation Settings
_______________________________

.. class:: SimulateOptions(*args)
   :module: RoadRunner

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
   
.. attribute:: SimulateOptions.integrator
   :module: RoadRunner

   A bitfield which may contain the following options. In python these options are
   also available as separate properties which set the integratorFlags bitfield.


.. attribute:: SimulateOptions.amounts
   :module: RoadRunner
      
   
   A list of the variable whose output in the results file is in amount
   (not concentration) units. This list of variables must be a subset of
   the names listed in variables.
      
   
.. attribute:: SimulateOptions.concentrations
   :module: RoadRunner
      
   
   A list of the variable whose output in the results file is in concentration
   (not amount) units. This list of variables must be a subset of the names
   listed in variables.
      
   
.. attribute:: SimulateOptions.duration
   :module: RoadRunner
      
   
   The duration of the simulation run, in the model's units of time. Note, 
   setting the duration automatically sets the end time and visa versa.
      
   
.. attribute:: SimulateOptions.end
   :module: RoadRunner
      
   
   The simulation end time. Note, setting the end time automatically sets the 
   duration accordingly and visa versa.
      
	  
.. attribute:: SimulateOptions.resetModel
   :module: RoadRunner
      
   
   Causes the model to be reset to the original conditions specified
   in the SBML when the simulation is run.
      
   
.. attribute:: SimulateOptions.start
   :module: RoadRunner
      
   
   The start time of the simulation time-series data.
   Often this is 0, but not necessarily.
      
   
.. attribute:: SimulateOptions.steps
   :module: RoadRunner
      
   
   The number of steps at which the output is sampled. The samples are evenly spaced.
   When a simulation system calculates the data points to record, it will typically
   divide the duration by the number of time steps. Thus, for N steps, the output
   will have N+1 data rows.

