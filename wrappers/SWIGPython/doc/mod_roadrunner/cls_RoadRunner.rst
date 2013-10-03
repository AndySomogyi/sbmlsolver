.. class:: RoadRunner
   The main RoadRunner class.

      
.. method:: RoadRunner.test()

   this method does some stuff

.. method:: RoadRunner.__init__(compiler='', tempDir='', supportCodeDir='')

   
   All three of the RoadRunner options default to the empty string, in this
   case, the default values are used.

    
   :param compiler: if LLVM build is enabled, the compiler defaults to LLVM.
   :type name: str (optional)
   
   :param tempDir: (string) typically ignored, only used by the old C RoadRunner.
   :type name: str (optional)
   
   :param supportCodeDir: typically ignored, only used by the old C RoadRunner
   :type name: str (optional)

   and some more stuff.
   
   
.. method:: RoadRunner.computeSteadyStateValue(*args)

   
              
.. method:: RoadRunner.getCC(variable, parameter)

      Get scaled a single control coefficient with respect to a global parameter.
      
      :param variable: The id of a dependent variable of the coefficient, for example a 
                       flux or species concentration.
      
      :param parameter: The id of the independent parameter, for example a kinetic constant 
                        or boundary species

      :returns: the value of the control coefficeint returned to the caller.

      :rtype: double
      
         
.. method:: RoadRunner.getCompiler()

   get the compiler used to build the ExecutableModel.
      
      
   
.. method:: RoadRunner.getConfigurationXML()
   :module: roadrunner
   
      recurse through all of the child configurable objects that this
      class ownes and build an assemble all of thier configuration parameters
      into a single xml document which is returned as a string.
      
      The value of this result depends on what child objects are presently loaded.
      
      
   
.. method:: RoadRunner.getConservationAnalysis()
   :module: roadrunner
   
   is conservation analysis enabled. This is set
      
      
   
.. method:: RoadRunner.getConservationMatrix()
   :module: roadrunner
   
   TODO docs
      
      
   
.. staticmethod:: RoadRunner.getCopyright()
   :module: roadrunner
   
   get the copyright string
      
      
   
.. method:: RoadRunner.getEE(reactionId, parameterId, steadyState=True)
   :module: roadrunner
   
   Retireve a single elasticity coefficient.
      
   :param str variable: The dependent variable of the coefficient, for example a flux or 
                        species concentration.
   :param str parameter: The independent parameter, for example a kinetic constant or boundary
                         species
   :param Boolean steadyState: should the steady state value be computed.
      
      
.. method:: RoadRunner.getEigenvalueIds()
   :module: roadrunner
   
   returns a list of floating species ids with thier names
   prefixed with 'eigen_'. For example, if the model contained
   the floating species 'S1' and 'S2', this would return a list
   containing ['eigen_S1', 'eigen_S2'].
      
  
.. method:: RoadRunner.getEigenvalues()
   :module: roadrunner
   
   Returns eigenvalues, first column real part, second column imaginary part
      
   :rtype: numpy.ndarray
      
      
   
.. staticmethod:: RoadRunner.getExtendedVersionInfo()
   :module: roadrunner
   
   getVersion plus info about dependent libs versions.


   
.. method:: RoadRunner.getFloatingSpeciesAmountIds()
   :module: roadrunner
   
   gets a list of the floating species ids, but with the ids surrounded
   by square brackets, i.e. 'S1' -> '[S1]'
      
      
   
.. method:: RoadRunner.getFullJacobian()
   :module: roadrunner
   
   compute the full Jacobian at the current operating point. 

   This is the Jacobian of ONLY the floating species.
      
      
   
.. method:: RoadRunner.getFullyReorderedStoichiometryMatrix()
   :module: roadrunner
   
   Returns the stoichiometry matrix for the currently loaded model
      
      
   
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
      
      
   
.. method:: RoadRunner.getL0Matrix()
   :module: roadrunner
   
   TODO docs
      
   :rtype: numpy.ndarray
      
      
   
.. method:: RoadRunner.getLinkMatrix()
   :module: roadrunner
   
   TODO docs
      
   :rtype: numpy.ndarray
      
      
   
.. method:: RoadRunner.getModelGenerator()
   :module: roadrunner
   
   TODO docs
      
   :rtype: numpy.ndarray
      
      
   
.. method:: RoadRunner.getNrMatrix()
   :module: roadrunner
   
   TODO docs
      
      :rtype: numpy.ndarray
      
      
   
.. staticmethod:: RoadRunner.getParamPromotedSBML(*args)
   :module: roadrunner
   
   Takes an sbml document (in textual form) and changes all of the local parameters
   to be global parameters.
      
   :param str sbml: the contents of an sbml document
   :rtype: str
      
      
   
.. method:: RoadRunner.getReducedJacobian()
   :module: roadrunner
   
   get the *reduced* Jacobian for the independent species. 
      
   :rtype: numpy.ndarray
      
      
   
.. method:: RoadRunner.getReorderedStoichiometryMatrix()
   :module: roadrunner
   
   TODO docs
      
   :rtype: numpy.ndarray
      
      
   
.. method:: RoadRunner.getSBML()
   :module: roadrunner
   
   return the currently loaded sbml document as a string.
      
   :rtype: str
      
      
   
.. method:: RoadRunner.getScaledConcentrationControlCoefficientMatrix()
   :module: roadrunner
   
   TODO docs
      
   :rtype: numpy.ndarray
      
      
   
.. method:: RoadRunner.getScaledFloatingSpeciesElasticity(*args)
   :module: roadrunner
   
   Compute the scaled elasticity for a given reaction and given species. 
      
   :param str reactionId: the sbml id of a reaction.
   :param str speciesId: the sbml id of a species.
   :rtype: double
      
      
   
.. method:: RoadRunner.getScaledFluxControlCoefficientMatrix()
   :module: roadrunner
   
   Calculate the the matrix of scaled flux control coefficients. The first column 
   will contain real values and the second column the imaginary values.
      
   :rtype: numpy.ndarray
      
      
   
.. method:: RoadRunner.getScaledReorderedElasticityMatrix()
   :module: roadrunner
   
   Compute the unscaled elasticity matrix at the current operating point
      
   :rtype: numpy.ndarray
      
      
   
.. method:: RoadRunner.getSelectedValues()
   :module: roadrunner
   
   returns the values selected with SimulateOptions for the current model time / timestep
      
   :rtype: numpy.ndarray
      
      
   
.. method:: RoadRunner.getSimulationResult()
   :module: roadrunner
   
   get the simulation result in case one forgot to hold on to the simulate return value.
      
   :rtype: numpy.ndarray
      
      
   
.. method:: RoadRunner.getSteadyStateSelectionList()
   :module: roadrunner
   
              
      
   
.. method:: RoadRunner.getStoichiometryMatrix()
   :module: roadrunner
   
   TODO docs
      
      
   
.. method:: RoadRunner.getTimeCourseSelectionList()
   :module: roadrunner
   
   TODO docs
      
      
   
.. method:: RoadRunner.getUnscaledConcentrationControlCoefficientMatrix()
   :module: roadrunner
   
   TODO docs
      
      
   
.. method:: RoadRunner.getUnscaledElasticityMatrix()
   :module: roadrunner
   
   TODO docs
      
      
   
.. method:: RoadRunner.getUnscaledFluxControlCoefficientMatrix()
   :module: roadrunner
   
   TODO docs
      
      
   
.. method:: RoadRunner.getUnscaledSpeciesElasticity(*args)
   :module: roadrunner
   
   Get a single species elasticity value
   IMPORTANT:
   Assumes that the reaction rates have been precomputed at the operating point !!
      
   :param int reactionIndx: index of reactionl
   :param int speciesIndx: index of species.
      
      
   
.. staticmethod:: RoadRunner.getVersion()
   :module: roadrunner
   
   TODO docs
      
      
   
.. staticmethod:: RoadRunner.getlibSBMLVersion()
   :module: roadrunner

   TODO docs
      
      
   
.. method:: RoadRunner.isModelLoaded()
   :module: roadrunner
   
   TODO docs
      
      
   
.. method:: RoadRunner.loadSBML(*args)
   :module: roadrunner
   
   TODO docs
      
      
   
.. method:: RoadRunner.loadSBMLFromFile(*args)
   :module: roadrunner
   
   Load an sbml document from disk.
      
   :param str path: path of an sbml document.
   :returns: True if successfull, False if failed.
      
      
   
.. py:attribute:: RoadRunner.model
   :module: roadrunner
   :annotation: None
   
   TODO docs
      
   
.. method:: RoadRunner.oneStep(*args)
   :module: roadrunner
   
   TODO docs
      
      
   
.. method:: RoadRunner.reset()
   :module: roadrunner
   
   TODO docs
      
      
   
.. method:: RoadRunner.setConfigurationXML(*args)
   :module: roadrunner
   
   given a xml document, which should have been returned from getConfigurationXML,
   this method recurses though all the child configurable elements and sets thier
   configuration to the values specified in the document.
      
   :param str xml: the contents of an xml document.
      
      
   
.. method:: RoadRunner.setConservationAnalysis(*args)
   :module: roadrunner
   
   TODO docs
      
      
   
.. method:: RoadRunner.setSteadyStateSelectionList(*args)
   :module: roadrunner
   
   :param list(str) selectionList: a list of selection identifiers.
      
      
   
.. method:: RoadRunner.setTimeCourseSelectionList(*args)
   :module: roadrunner
   
   :param list(str) selectionList: a list of selection identifiers.
      
      
   
.. method:: RoadRunner.simulate(*args)
   :module: roadrunner
   
   simulate the current SBML model.
      
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
      
     r.simulate(0, 3.14, 100)
      
   The start time of 0, end time of 3.14 and steps of 100 will remain in effect, so that if this
   is followed by a call to::
      
     r.simulate()
      
   This simulation will use the previous values. 
      
   :returns: a numpy array with each selected output timeseries being a
             column vector, and the 0'th column is the simulation time.
   :rtype: numpy.ndarray
      
   
.. py:attribute:: RoadRunner.simulateOptions
   :module: roadrunner
   :annotation: None
   
   Get the SimulateOptions object where simulation options may be set. 
      
   
.. method:: RoadRunner.steadyState()
   :module: roadrunner
   
   Attempt to evaluate the steady state for the model. The method returns
   a value that indicates how close the solution is to the steady state. 
   The smaller the value the better. Values less than 1E-6 usually indicate a
   steady state has been found. If necessary the method can be called a 
   second time to improve the solution.
      
      

.. py:function:: RoadRunner_getCopyright()
   :module: roadrunner

   get the copyright string
   
   

.. py:function:: RoadRunner_getExtendedVersionInfo()
   :module: roadrunner

   getVersion plus info about dependent libs versions.
   
   

.. py:function:: RoadRunner_getParamPromotedSBML(*args)
   :module: roadrunner

   Takes an sbml document (in textual form) and changes all of the local parameters
   to be global parameters.
   
   :param str sbml: the contents of an sbml document
   :rtype: str
   
   

.. py:function:: RoadRunner_getVersion()
   :module: roadrunner

   TODO docs
   
   

.. py:function:: RoadRunner_getlibSBMLVersion()
   :module: roadrunner

   TODO docs
