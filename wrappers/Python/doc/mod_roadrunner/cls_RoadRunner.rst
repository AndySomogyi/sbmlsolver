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



.. method:: RoadRunner.createSelection(str)

   Creates a new SelectionRecord for the given selection string.



.. method:: RoadRunner.getCompiler()

   Return the compiler used to build the ExecutableModel.



.. method:: RoadRunner.getConfigurationXML()
   :module: roadrunner

   recurse through all of the child configurable objects that this
   class ones and build an assemble all of their configuration parameters
   into a single xml document which is returned as a string.

   The value of this result depends on what child objects are presently loaded.




.. staticmethod:: RoadRunner.getCopyright()
   :module: roadrunner

   Returns the copyright string



.. staticmethod:: RoadRunner.getExtendedVersionInfo()
   :module: roadrunner

   getVersion plus info about dependent libs versions.



.. method:: RoadRunner.getFloatingSpeciesAmountIds()
   :module: roadrunner

   Returns a list of the floating species Ids, but with the Ids surrounded
   by square brackets, i.e. 'S1' -> '\[S1]'





.. method:: RoadRunner.getInfo()
   :module: roadrunner

   return info about the current state of the object

   :rtype: str



.. method:: RoadRunner.getInstanceCount()
   :module: roadrunner

   Number of currently running RoadRunner instances.



.. method:: RoadRunner.getInstanceID()
   :module: roadrunner

   When there are multiple instances of RoadRunner, this is the instance id.



.. method:: RoadRunner.getIntegrator()
   :module: roadrunner

   get the integrator which is currently being used to
   time evolve the system.




.. staticmethod:: RoadRunner.getParamPromotedSBML(*args)
   :module: roadrunner

   Takes an SBML document (in textual form) and changes all of the local parameters
   to be global parameters.

   :param str SBML: the contents of an SBML document
   :rtype: str




.. method:: RoadRunner.getCurrentSBML()
   :module: roadrunner

   Returns the current state of the model in the form of an SBML string. 
   
   That is the SBML will reflect the current state of the model and not the 
   original SBML that was loaded into RoadRunner.

   :rtype: str


.. method:: RoadRunner.getSBML()
   :module: roadrunner

   Returns the original SBML model that was loaded into roadrunner.

   :rtype: str



.. method:: RoadRunner.getValue(sel)

   Returns the value for a given selection.

   :param sel: a selection that is either a string or a SelectionRecord that was
               obtained from createSelection
   :type sel: str or SelectionRecord



.. method:: RoadRunner.getSelectedValues()
   :module: roadrunner

   returns the values selected with SimulateOptions for the current model time / timestep

   :rtype: numpy.ndarray



.. method:: RoadRunner.getSimulationResult()
   :module: roadrunner

   get the simulation result in case one forgot to hold on to the simulate return value.

   :rtype: numpy.ndarray



.. attribute:: RoadRunner.selections
   :module: roadrunner



.. method:: RoadRunner.createSelection(sel)

   Create a new selection based on a selection string

   :rtype: roadrunner.SelectionRecord




.. staticmethod:: RoadRunner.__version__()
   :module: roadrunner

   Returns the current version of the RoadRunner library.


.. staticmethod:: RoadRunner.getlibSBMLVersion()
   :module: roadrunner

   Returns the version of the libSBML library that is currently being used.



.. method:: RoadRunner.isModelLoaded()
   :module: roadrunner

   Return True if model was loaded; False otherwise



.. method:: RoadRunner.load(uriOrDocument)
   :module: roadrunner

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



.. py:attribute:: RoadRunner.model
   :module: roadrunner
   :annotation: None

   Get the currently loaded model. The model object contains the entire state of the SBML model.


.. method:: RoadRunner.oneStep(startTime, stepSize)
   :module: roadrunner

   Carry out a one step integration of the model. The method takes two arguments,
   the current time and the step size to us in the integration. The method returns
   the new time which will be currentTime + StepSize::

       newTime = rr.oneStep (10, 0.5)


.. method:: RoadRunner.reset()
   :module: roadrunner

   This method resets all the floating species concentrations to their initial values.



.. method:: RoadRunner.setConfigurationXML(*args)
   :module: roadrunner

   given a xml document, which should have been returned from getConfigurationXML,
   this method recurses though all the child configurable elements and sets their
   configuration to the values specified in the document.

   :param str xml: the contents of an xml document.



.. attribute:: RoadRunner.conservedMoietyAnalysis
   :module: roadrunner

   Enables / Disables conserved moiety analysis (boolean). 

   If this is enabled, the SBML document (either current, or one about to be loaded) 
   is converted using the ConservedMoietyConverter. All of the linearly dependent 
   species are replaced with assignment rules and a new set of conserved moiety 
   parameters are introduced. 


.. method:: RoadRunner.simulate(*args)
   :module: roadrunner

   Simulate the current SBML model.

   There are a number of ways to call simulate.

   1. With no arguments. In this case, the current set of `SimulateOptions` will
      be used for the simulation. The current set may be changed either directly
      via setSimulateOptions() or with one of the two alternate ways of calling
      simulate.

   2: With single `SimulateOptions` argument. In this case, all of the settings
      in the given options are copied and will be used for the current and future
      simulations.

   3: With the three positions arguments, `timeStart`, `timeEnd`, `steps`. In this case
      these three values are copied and will be used for the current and future simulations.

   The options given in the 2nd and 3rd forms will remain in effect until changed. So, if
   one calls::

     rr.simulate (0, 3, 100)

   The start time of 0, end time of 3 and steps of 100 will remain in effect, so that if this
   is followed by a call to::

     rr.simulate()

   This simulation will use the previous values.

   :returns: a numpy array with each selected output time series being a
             column vector, and the 0'th column is the simulation time.
   :rtype: numpy.ndarray


.. py:attribute:: RoadRunner.simulateOptions
   :module: roadrunner
   :annotation: None

   Get the SimulateOptions object where simulation options may be set.




.. py:function:: RoadRunner_getCopyright()
   :module: roadrunner

   Returns the copyright string



.. py:function:: RoadRunner_getExtendedVersionInfo()
   :module: roadrunner

   getVersion plus info about dependent libs versions.



.. py:function:: RoadRunner_getParamPromotedSBML(*args)
   :module: roadrunner

   Takes an SBML document (in textual form) and changes all of the local parameters
   to be global parameters.

   :param str SBML: the contents of an SBML document
   :rtype: str

.. method:: RoadRunner.evalModel()
   :module: roadrunner

   Evaluates the current model, that is it updates the rates of change and any assignments in the model.
   It does *not* carry out an integration step.

   :returns: Returns true if successful


Steady State Sections
---------------------

.. attribute:: RoadRunner.steadyStateSelections

   A list of SelectionRecords which determine what values are used for 
   a steady state calculation. This list may be set by assigning a list
   of valid selection symbols::

     r.steadyStateSelections = ['S1', '[S2]', 'P1']


.. method:: RoadRunner.steadyState()
   :module: roadrunner

   Attempt to evaluate the steady state for the model. The method returns
   a value that indicates how close the solution is to the steady state.
   The smaller the value the better. Values less than 1E-6 usually indicate a
   steady state has been found. If necessary the method can be called a
   second time to improve the solution.

   :returns: the sum of squares of the steady state solution.

   :rtype: double


.. method:: RoadRunner.getSteadyStateValues()
   :module: roadrunner
    
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

   Get unscaled control coefficient with respect to a global parameter.

   :param variableId: must be either a reaction or floating species.

   :param parameterId: must be either a global parameter, boundary species, or
                       conserved sum.

.. method:: RoadRunner.getEE(reactionId, parameterId, steadyState=True)
   :module: roadrunner

   Retrieve a single elasticity coefficient with respect to a global parameter.
   
   For example::

     x = rr.getEE ('J1', 'Vmax')

   :param str variable: The dependent variable of the coefficient, for example a flux or
                        species concentration.
   :param str parameter: The independent parameter, for example a kinetic constant or boundary
                         species
   :param Boolean steadyState: should the steady state value be computed.


.. method:: RoadRunner.getuEE(reactionId, parameterId)

   Get unscaled elasticity coefficient with respect to a global parameter or species.


.. method:: RoadRunner.getEigenvalueIds()
   :module: roadrunner

   returns a list of selection symbols for the eigenvalues of the floating species. The eigen value
   selection symbol is ``eigen(XX)``, where ``XX`` is the floating species name. 

.. method:: RoadRunner.getEigenvalues()
   :module: roadrunner


   Calculates the eigen values of the Jacobian as a real matrix, first column real part, second
   column imaginary part.

   If moiety conservation is enables, the reduced Jacobian is used, otherwise the full Jacobian is
   used. 

   Note, only valid for pure reaction kinetics models (no rate rules, no floating species rules and
   time invariant stoichiometry). 

   :rtype: numpy.ndarray


.. method:: RoadRunner.getFullJacobian()
   :module: roadrunner

   Compute the full Jacobian at the current operating point.

   This is the Jacobian of ONLY the floating species.


.. method:: RoadRunner.getFullyReorderedStoichiometryMatrix()
   :module: roadrunner

   Returns the full reordered stoichiometry matrix for the currently loaded model.
   The rows will corresponds to the order of species in the call to getFloatinSpeciesIds(),
   the columns will corresponds to the order of reactions in the call to getReactionIds().



.. method:: RoadRunner.getReducedJacobian()
   :module: roadrunner

   Returns the *reduced* Jacobian for the independent species. This matrix will be non-singular
   for models that include moiety-conserved cycles.

   :rtype: numpy.ndarray




.. method:: RoadRunner.getScaledConcentrationControlCoefficientMatrix()
   :module: roadrunner

   Returns the m by n matrix of scaled concentration control coefficients where m is the number
   of floating species and n the number of reactions.

   :rtype: numpy.ndarray


.. method:: RoadRunner.getScaledFloatingSpeciesElasticity(reactionId, speciesId)
   :module: roadrunner

   Returns the scaled elasticity for a given reaction and given species.

   :param str reactionId: the SBML id of a reaction.
   :param str speciesId: the SBML id of a species.
   :rtype: double


.. method:: RoadRunner.getUnscaledConcentrationControlCoefficientMatrix()
   :module: roadrunner

   Returns the unscaled concentration control coefficient matrix.


.. method:: RoadRunner.getUnscaledElasticityMatrix()
   :module: roadrunner

   Returns the unscaled species elasticity matrix at the current operating point.


.. method:: RoadRunner.getUnscaledFluxControlCoefficientMatrix()
   :module: roadrunner

   Returns the unscaled flux control coefficient matrix.


.. method:: RoadRunner.getUnscaledSpeciesElasticity(reactionIndx, speciesIndx)
   :module: roadrunner

   Get a single species elasticity value. 


   :param int reactionIndx: index of reaction
   :param int speciesIndx: index of species.


.. method:: RoadRunner.getScaledFluxControlCoefficientMatrix()
   :module: roadrunner

   Returns the n by n matrix of scaled flux control coefficients where n is the number of reactions.

   :rtype: numpy.ndarray



.. method:: RoadRunner.getScaledElasticityMatrix()
   :module: roadrunner

   Returns the scaled elasticity matrix at the current operating point.

   :rtype: numpy.ndarray



.. method:: RoadRunner.getReorderedStoichiometryMatrix()
   :module: roadrunner

   Returns the reordered stoichiometry matrix where the tops rows represent the independent species of
   which there will be rank (N) and the bottom rows the dependent species.

   :rtype: numpy.ndarray


.. method:: RoadRunner.getConservationMatrix()
   :module: roadrunner

   Returns a conservation matrix :math:`\Gamma` which is a :math:`c \times m` matrix
   where :math:`c` is the number of conservation laws and :math:`m` the number of species.



.. method:: RoadRunner.getL0Matrix()
   :module: roadrunner

   Returns the L0 matrix for the current model. The L0 matrix is an (m-r) by r matrix that expresses
   the dependent reaction rates in terms of the independent rates. m is the number of floating species
   and r is the rank of the stoichiometry matrix.

   :rtype: numpy.ndarray



.. method:: RoadRunner.getLinkMatrix()
   :module: roadrunner

   Returns the full link matrix, L for the current model. The Link matrix is an m by r matrix where m
   is the number of floating species and r the rank of the stoichiometric matrix, N.

   :rtype: numpy.ndarray

.. method:: RoadRunner.getNrMatrix()
   :module: roadrunner

   Returns the reduced stoichiometry matrix, :math:`N_R`, which will have only r rows where r is the rank of
   the full stoichiometry matrix. The matrix will be reordered such that the rows of :math:`N_R` are independent.

   :rtype: numpy.ndarray

