The Main RoadRunner Class
_________________________

.. class:: RoadRunner

   The main RoadRunner class. Its objects, i.e. ``rr = RoadRunner()`` perform 
   the libRoaRunner functions, i.e. ``rr.simulate()``.



.. method:: RoadRunner.__init__(uriOrSBML = "", options = None)

   Creates a new RoadRunner object. If the first argument is specified, 
   it should be a string containing either the contents of an SBML document, 
   or a formatted URI specifying the path or location of a SBML document. 

   If options is given, it should be a LoadSBMLOptions object. 

   If no arguments are given, a document may be loaded at any future time
   using the load method. 

   :param uriOrSBML: a URI, local path or SBML document contents.
   :type name: str (optional)

   :param options: (LoadSBMLOptions) an options object specifying how the 
                   SBML document should be loaded
   :type name: str (optional)


.. method:: RoadRunner.load(uriOrDocument)
   :module: RoadRunner

   Loads an SBML document, given a string for file path, URI, or contents. 

   This method also accepts HTTP URI for remote files, however this feature is currently limited 
   to the Mac version, plan on enabling HTTP loading of SBML documents on Windows and Linux 
   shortly. 

   Some examples of loading files on Mac or Linux::
   
       >>> r.load("myfile.xml")                               # load a file from the current directory
       >>> r.load("/Users/Fred/myfile.xml")                   # absolute path
       >>> r.load("http://sbml.org/example_system.xml")       # remote file


   Or on Windows:

       >>> r.load("myfile.xml")                                  # load a file from the current directory
       >>> r.load("file://localhost/c:/Users/Fred/myfile.xml")   # using a URI

   One may also load the contents of a document::

       >>> myfile = open("myfile.xml, "r")
       >>> contents = file.read()
       >>> r.load(contents)

   In future version, we will also support loading directly from a libSBML Document object. 

   :param uriOrDocument: A string which may be a local path, URI or contents of an SBML document. 
   :type name: str






.. method:: RoadRunner.getCompiler()

   Return the JIT :class:`Compiler` object currently being used. 
   This object provides various information about the current processor and system.



.. method:: RoadRunner.getConfigurationXML()
   :module: RoadRunner

   recurse through all of the child configurable objects that this
   class ones and build an assemble all of their configuration parameters
   into a single xml document which is returned as a string.

   The value of this result depends on what child objects are presently loaded.


.. staticmethod:: RoadRunner.getExtendedVersionInfo()
   :module: RoadRunner

   getVersion plus info about dependent libs versions.



.. method:: RoadRunner.getInfo()
   :module: RoadRunner

   return info about the current state of the object

   :rtype: str



.. method:: RoadRunner.getInstanceCount()
   :module: RoadRunner

   Number of currently running RoadRunner instances.



.. method:: RoadRunner.getInstanceID()
   :module: RoadRunner

   When there are multiple instances of RoadRunner, this is the instance id.



.. method:: RoadRunner.getIntegrator()
   :module: RoadRunner

   get the integrator which is currently being used to
   time evolve the system.

.. method:: RoadRunner.getAvailableIntegrators()
   :module: RoadRunner
   
   Get a list of available integrator names.



.. staticmethod:: RoadRunner.getParamPromotedSBML(*args)
   :module: RoadRunner

   Takes an SBML document (in textual form) and changes all of the local parameters
   to be global parameters.

   :param str SBML: the contents of an SBML document
   :rtype: str




.. method:: RoadRunner.getCurrentSBML()
   :module: RoadRunner

   Returns the current state of the model in the form of an SBML string. 
   
   That is the SBML will reflect the current state of the model and not the 
   original SBML that was loaded into RoadRunner.

   :rtype: str


.. method:: RoadRunner.getSBML()
   :module: RoadRunner

   Returns the original SBML model that was loaded into roadrunner.

   :rtype: str

Selections
----------

.. method:: RoadRunner.getIds()

   Return a list of selection ids that this object can select on.

   :rtype: list


.. method:: RoadRunner.getValue(sel)

   Returns the value for a given selection.

   :param sel: a selection that is either a string or a SelectionRecord that was
               obtained from createSelection
   :type sel: str or SelectionRecord



.. method:: RoadRunner.getSelectedValues()
   :module: RoadRunner

   returns the values selected with SimulateOptions for the current model time / timestep

   :rtype: numpy.ndarray



.. attribute:: RoadRunner.selections
   :module: RoadRunner
            
   Get or set the list of current selections used for the simulation result columns. 


.. method:: RoadRunner.createSelection(sel)

   Create a new selection based on a selection string

   :rtype: roadrunner.SelectionRecord


Model Access
------------


.. method:: RoadRunner.isModelLoaded()
   :module: RoadRunner

   Return True if model was loaded; False otherwise



.. py:attribute:: RoadRunner.model
   :module: RoadRunner
   :annotation: None

   Get the currently loaded model. The model object contains the entire state of the SBML model.


.. method:: RoadRunner.getModel()
   :module: RoadRunner

   Function form of the RoadRunner.model property, identical to model. 

.. method:: RoadRunner.oneStep(startTime, stepSize)
   :module: RoadRunner

   Carry out a one step integration of the model. The method takes two arguments,
   the current time and the step size to us in the integration. The method returns
   the new time which will be currentTime + StepSize::

       newTime = rr.oneStep (10, 0.5)

 
.. method:: RoadRunner.reset()
   :module: RoadRunner

   This method resets all the floating species concentrations to their initial values.



.. method:: RoadRunner.setConfigurationXML(*args)
   :module: RoadRunner

   given a xml document, which should have been returned from getConfigurationXML,
   this method recurses though all the child configurable elements and sets their
   configuration to the values specified in the document.

   :param str xml: the contents of an xml document.



.. attribute:: RoadRunner.conservedMoietyAnalysis
   :module: RoadRunner

   Enables / Disables conserved moiety analysis (boolean). 

   If this is enabled, the SBML document (either current, or one about to be loaded) 
   is converted using the ConservedMoietyConverter. All of the linearly dependent 
   species are replaced with assignment rules and a new set of conserved moiety 
   parameters are introduced. 


Simulation
----------

Fast and easy time series simulations is one of the main objectives of the RoadRunner project. 

All simulation related tasks can be accomplished with the single ``simulate`` method. 
 

.. method:: RoadRunner.simulate(*args, **kwargs)
   :module: RoadRunner



   Simulate the optionally plot current SBML model. This is the one stop shopping method
   for simulation and plotting. 

   simulate accepts a up to four positional arguments and a large number of keyword args. 

   The first four (optional) arguments are treated as:
            
      1: Start Time, if this is a number. 

      2: End Time, if this is a number.

      3: Number of Steps, if this is a number.
            
      4: List of Selections. 

   All four of the positional arguments are optional. If any of the positional arguments are
   a list of string instead of a number, then they are interpreted as a list of selections. 

   
   There are a number of ways to call simulate.

   1: With no arguments. In this case, the current set of options from the previous 
      ``simulate`` call will be used. If this is the first time ``simulate`` is called, 
      then a default set of values is used. 

   2: With up to three positions arguments, described above. 

   3: With optional keyword arguments where keywords are listed below. 

      For example, to reset the model, simulate from 0 to 10 in 1000 steps and plot we can::
        
        rr.simulate(end=10, start=0, steps=1000, reset=True, plot=True)

      All of the options given to ``simulate`` are remembered and used as default arguments for
      subsequent calls, i.e. if one calls::

        rr.simulate (0, 3, 100, ["time", "[S1]"])

      The start time of 0, end time of 3, steps of 100 and the selection list will remain in effect,
      so that if this is followed by a call to::

        rr.simulate()

      This simulation will use the previous values. Note, that if the ``reset=True`` options was not
      given, this will continue the simulation using the previous model state, but time here will
      start at 0 and continue to 3. 

   simulate accepts the following list of keyword arguments:

   integrator
     A text string specifying which integrator to use. Currently supports "cvode"
     for deterministic simulation (default) and "gillespie" for stochastic 
     simulation.

   sel or selections
     A list of strings specifying what values to display in the output. 

   plot
     True or False
     If True, RoadRunner will create a basic plot of the simulation result using
     the built in plot routine which uses MatPlotLib. 

   absolute
     A number representing the absolute difference permitted for the integrator 
     tolerance.

   duration
     The duration of the simulation run, in the model's units of time.
     Note, setting the duration automatically sets the end time and visa versa.
     
   end
     The simulation end time. Note, setting the end time automatically sets 
     the duration accordingly and visa versa.

   relative
     A float-point number representing the relative difference permitted. 
     Defaults 0.0001

   resetModel (or just "reset"???)
     True or False
     Causes the model to be reset to the original conditions specified in 
     the SBML when the simulation is run.

   start
     The start time of the simulation time-series data. Often this is 0, 
     but not necessarily.

   steps
     The number of steps at which the output is sampled. The samples are evenly spaced. 
     When a simulation system calculates the data points to record, it will typically 
     divide the duration by the number of time steps. Thus, for N steps, the output 
     will have N+1 data rows.

   stiff
     True or False
     Use the stiff integrator. Only use this if the model is stiff and causes issues 
     with the regular integrator. The stiff integrator is slower than the conventional 
     integrator.

   multiStep
     True or False
     Perform a multi step integration.
     
     \* Experimental \*
     Perform a multi-step simulation. In multi-step simulation, one may monitor the 
     variable time stepping via the IntegratorListener events system.

   initialTimeStep
     A user specified initial time step. If this is <= 0, the integrator will attempt 
     to determine a safe initial time step.

     Note, for each number of steps given to RoadRunner.simulate or RoadRunner.integrate 
     the internal integrator may take many many steps to reach one of the external time steps. 
     This value specifies an initial value for the internal integrator time step.

   minimumTimeStep
     Specify the minimum time step that the internal integrator will use. 
     Uses integrator estimated value if <= 0.

   maximumTimeStep
     Specify the maximum time step size that the internal integrator will use. 
     Uses integrator estimated value if <= 0.

   maximumNumSteps
     Specify the maximum number of steps the internal integrator will use before 
     reaching the user specified time span. Uses the integrator default value if <= 0.


   :returns: a numpy array with each selected output time series being a
             column vector, and the 0'th column is the simulation time.
   :rtype: numpy.ndarray



.. py:attribute:: RoadRunner.simulateOptions
   :module: RoadRunner
   :annotation: None

   Get the SimulateOptions object where simulation options may be set.




.. py:function:: RoadRunner_getCopyright()
   :module: RoadRunner

   Returns the copyright string



.. py:function:: RoadRunner_getExtendedVersionInfo()
   :module: RoadRunner

   getVersion plus info about dependent libs versions.



.. py:function:: RoadRunner_getParamPromotedSBML(*args)
   :module: RoadRunner

   Takes an SBML document (in textual form) and changes all of the local parameters
   to be global parameters.

   :param str SBML: the contents of an SBML document
   :rtype: str



Steady State Sections
---------------------

.. attribute:: RoadRunner.steadyStateSelections
   :module: RoadRunner

   A list of SelectionRecords which determine what values are used for 
   a steady state calculation. This list may be set by assigning a list
   of valid selection symbols::

     >>> r.steadyStateSelections = ['S1', '[S2]', 'P1']
     >>> r.steadyStateSelections
     ['S1', '[S2]', 'P1']


.. method:: RoadRunner.steadyState()
   :module: RoadRunner

   Attempt to evaluate the steady state for the model. The method returns
   a value that indicates how close the solution is to the steady state.
   The smaller the value the better. Values less than 1E-6 usually indicate a
   steady state has been found. If necessary the method can be called a
   second time to improve the solution.

   :returns: the sum of squares of the steady state solution.

   :rtype: double


.. method:: RoadRunner.getSteadyStateValues()
   :module: RoadRunner
    
   Performs a steady state calculation (evolves the system to a steady
   state), then calculates and returns the set of values specified by
   the steady state selections.

   :returns: a numpy array corresponding to the values specified by steadyStateSelections

   :rtype: numpy.ndarray



Metabolic control analysis
--------------------------

In the special case when an SBML model is a purely reaction kinetics model -- no rate rules, no
assignment rules for chemical species, and time invariant stoichiometry, specialized analysis methods
related to metabolic control analysis are applicable. These methods are described in this section. 


.. method:: RoadRunner.getCC(variable, parameter)
   :module: RoadRunner

   Returns a scaled control coefficient with respect to a global parameter.
   
   For example::

     rr.getCC ('J1', 'Vmax')
     rr.getCC ('S1', 'Xo')
     rr.getCC ('S2', 'Km')

   The first returns a flux control coefficient with respect to flux J1. The second and third
   return concentration control coefficients with respect to species S1 and S2.

   :param variable: The id of a dependent variable of the coefficient, for example a
                    flux or species concentration.

   :param parameter: The id of the independent parameter, for example a kinetic constant
                     or boundary species

   :returns: the value of the control coefficient returned to the caller.

   :rtype: double


.. method:: RoadRunner.getuCC(variableId, parameterId)
   :module: RoadRunner

   Get unscaled control coefficient with respect to a global parameter.

   :param variableId: must be either a reaction or floating species.

   :param parameterId: must be either a global parameter, boundary species, or
                       conserved sum.

					   
.. method:: RoadRunner.getEE(reactionId, parameterId, steadyState=True)
   :module: RoadRunner

   Retrieve a single elasticity coefficient with respect to a global parameter.
   
   For example::

     x = rr.getEE ('J1', 'Vmax')

   :param str variable: The dependent variable of the coefficient, for example a flux or
                        species concentration.
   :param str parameter: The independent parameter, for example a kinetic constant or boundary
                         species
   :param Boolean steadyState: should the steady state value be computed.


.. method:: RoadRunner.getuEE(reactionId, parameterId)
   :module: RoadRunner

   Get unscaled elasticity coefficient with respect to a global parameter or species.


.. method:: RoadRunner.getEigenValueIds()
   :module: RoadRunner

   returns a list of selection symbols for the eigenvalues of the floating species. The eigen value
   selection symbol is ``eigen(XX)``, where ``XX`` is the floating species name. 

   
.. method:: RoadRunner.getFullEigenValues()
   :module: RoadRunner


   Calculates the eigen values of the Full Jacobian as a real matrix, first column real part, second
   column imaginary part.

   Note, only valid for pure reaction kinetics models (no rate rules, no floating species rules and
   time invariant stoichiometry). 

   :rtype: numpy.ndarray


.. method:: RoadRunner.getReducedEigenValues()
   :module: RoadRunner


   Calculates the eigen values of the Reduced Jacobian as a real matrix, first column real part, second
   column imaginary part.

   Only valid if moiety conversion is enabled. 

   Note, only valid for pure reaction kinetics models (no rate rules, no floating species rules and
   time invariant stoichiometry). 

   :rtype: numpy.ndarray


.. method:: RoadRunner.getFullJacobian()
   :module: RoadRunner

   Compute the full Jacobian at the current operating point.

   This is the Jacobian of ONLY the floating species.


.. method:: RoadRunner.getReducedJacobian()
   :module: RoadRunner

   Returns the *reduced* Jacobian for the independent species. This matrix will be non-singular
   for models that include moiety-conserved cycles.

   :rtype: numpy.ndarray




.. method:: RoadRunner.getScaledConcentrationControlCoefficientMatrix()
   :module: RoadRunner

   Returns the m by n matrix of scaled concentration control coefficients where m is the number
   of floating species and n the number of reactions.

   :rtype: numpy.ndarray


.. method:: RoadRunner.getScaledFloatingSpeciesElasticity(reactionId, speciesId)
   :module: RoadRunner

   Returns the scaled elasticity for a given reaction and given species.

   :param str reactionId: the SBML id of a reaction.
   :param str speciesId: the SBML id of a species.
   :rtype: double


.. method:: RoadRunner.getUnscaledParameterElasticity(reactionId, parameterId)
   :module: RoadRunner

    Returns the unscaled elasticity for a named reaction with respect to a
    named parameter
     
   :param str reactionId: the SBML id of a reaction.
   :param str parameterId: the SBML id of a parameter.
   :rtype: double


.. method:: RoadRunner.getUnscaledConcentrationControlCoefficientMatrix()
   :module: RoadRunner

   Returns the unscaled concentration control coefficient matrix.


.. method:: RoadRunner.getUnscaledElasticityMatrix()
   :module: RoadRunner

   Returns the unscaled species elasticity matrix at the current operating point.


.. method:: RoadRunner.getUnscaledFluxControlCoefficientMatrix()
   :module: RoadRunner

   Returns the unscaled flux control coefficient matrix.


.. method:: RoadRunner.getUnscaledSpeciesElasticity(reactionIndx, speciesIndx)
   :module: RoadRunner

   Get a single species elasticity value. 


   :param int reactionIndx: index of reaction
   :param int speciesIndx: index of species.


.. method:: RoadRunner.getScaledFluxControlCoefficientMatrix()
   :module: RoadRunner

   Returns the n by n matrix of scaled flux control coefficients where n is the number of reactions.

   :rtype: numpy.ndarray



.. method:: RoadRunner.getScaledElasticityMatrix()
   :module: RoadRunner

   Returns the scaled elasticity matrix at the current operating point.

   :rtype: numpy.ndarray


Stochiometric Analysis
----------------------

.. method:: RoadRunner.getFullStoichiometryMatrix()
   :module: RoadRunner


   Get the stoichiometry matrix that coresponds to the full model, even it
   it was converted via conservation conversion.


.. method:: RoadRunner.getReducedStoichiometryMatrix()
   :module: RoadRunner

   get the reduced stochiometry matrix. If conservation conversion is enabled,
   this is the matrix that coresponds to the independent species.

   A synonym for getNrMatrix().



.. method:: RoadRunner.getConservationMatrix()
   :module: RoadRunner

   Returns a conservation matrix :math:`\Gamma` which is a :math:`c \times m` matrix
   where :math:`c` is the number of conservation laws and :math:`m` the number of species.



.. method:: RoadRunner.getL0Matrix()
   :module: RoadRunner

   Returns the L0 matrix for the current model. The L0 matrix is an (m-r) by r matrix that expresses
   the dependent reaction rates in terms of the independent rates. m is the number of floating species
   and r is the rank of the stoichiometry matrix.

   :rtype: numpy.ndarray



.. method:: RoadRunner.getLinkMatrix()
   :module: RoadRunner

   Returns the full link matrix, L for the current model. The Link matrix is an m by r matrix where m
   is the number of floating species and r the rank of the stoichiometric matrix, N.

   :rtype: numpy.ndarray

.. method:: RoadRunner.getNrMatrix()
   :module: RoadRunner

   Returns the reduced stoichiometry matrix, :math:`N_R`, which will have only r rows where r is the rank of
   the full stoichiometry matrix. The matrix will be reordered such that the rows of :math:`N_R` are independent.

   :rtype: numpy.ndarray


.. method:: RoadRunner.getKMatrix()
   :module: RoadRunner

   Returns the K matrix, :math:`ker(N_R)`, (right nullspace of Nr)
   The K matrix has the structure, :math:`[I K0]'`

   :rtype: numpy.ndarray


Analysis
--------

.. method:: RoadRunner.getFrequencyResponse(startFrequency, numberOfDecades, numberOfPoints, parameterName, variableName, useDB, useHz)
   :module: RoadRunner

   Compute the frequency response

   :rtype: numpy.ndarray
