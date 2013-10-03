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

   Returns a vector of steady state values for the floating species. The order of 
   species in the vector is indicated by the order of species Ids in a call to 
   getFloatingSpeciesIds()
   
              
.. method:: RoadRunner.getCC(variable, parameter)

   Returns a scaled control coefficient with respect to a parameter. For example::
   
     r.getCC ('J1', 'Vmax')
     r.getCC ('S1', 'Xo')
     r.getCC ('S2', 'Km')
      
   The first returns a flux control coefficient with respect to flux J1. The second and third
   return concentration control coefficients with respect to speies S1 and S2.
   
   :param variable: The id of a dependent variable of the coefficient, for example a 
                    flux or species concentration.
      
   :param parameter: The id of the independent parameter, for example a kinetic constant 
                     or boundary species

   :returns: the value of the control coefficeint returned to the caller.

   :rtype: double
      
         
.. method:: RoadRunner.getCompiler()

   Return the compiler used to build the ExecutableModel.
      
      
   
.. method:: RoadRunner.getConfigurationXML()
   :module: roadrunner
   
   recurse through all of the child configurable objects that this
   class ownes and build an assemble all of thier configuration parameters
   into a single xml document which is returned as a string.
      
   The value of this result depends on what child objects are presently loaded.
      
      
   
.. method:: RoadRunner.getConservationAnalysis()
   :module: roadrunner
   
   is conservation analysis enabled. This is set NEEDS WORD
      
      
   
.. method:: RoadRunner.getConservationMatrix()
   :module: roadrunner
   
   TODO docs
      
      
   
.. staticmethod:: RoadRunner.getCopyright()
   :module: roadrunner
   
   Returns the copyright string
      
     
.. method:: RoadRunner.getEE(reactionId, parameterId, steadyState=True)
   :module: roadrunner
   
   Retireve a single elasticity coefficient. For example
   
   getEE ('J1', 'Vmax')
      
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
   
   Returns eigenvalues, first column real part, second column imaginary part.
      
   :rtype: numpy.ndarray
      
      
   
.. staticmethod:: RoadRunner.getExtendedVersionInfo()
   :module: roadrunner
   
   getVersion plus info about dependent libs versions.


   
.. method:: RoadRunner.getFloatingSpeciesAmountIds()
   :module: roadrunner
   
   Returns a list of the floating species Ids, but with theIids surrounded
   by square brackets, i.e. 'S1' -> '[S1]'
      
      
   
.. method:: RoadRunner.getFullJacobian()
   :module: roadrunner
   
   Compute the full Jacobian at the current operating point. 

   This is the Jacobian of ONLY the floating species.
      
      
   
.. method:: RoadRunner.getFullyReorderedStoichiometryMatrix()
   :module: roadrunner
   
   Returns the full reordered stoichiometry matrix for the currently loaded model.
   The rows will coresponds to the order of species in the call to getFloatinSpeciesIds(),
   the colunms will corresponds to the order of reactions in the call to getReactionIds().
      
      
   
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
   
   Returns the L0 matrix for the current model. The L0 matrix is an (m-r) by r matrix that expresses 
   the dependent reaction rates in terms of the indepdent rates. m is the number of floating species
   and r is the rank of the stoichiometry matrix.
      
   :rtype: numpy.ndarray
      
      
   
.. method:: RoadRunner.getLinkMatrix()
   :module: roadrunner
   
   Returns the full link matrix, L for the current model. The Link matrix is an m by r matrix where m
   is the number of floating species and r the rank of the stichiometric matrix, N.
      
   :rtype: numpy.ndarray
      
      
   
.. method:: RoadRunner.getModelGenerator()
   :module: roadrunner
   
   TODO docs
      
   :rtype: numpy.ndarray
      
      
   
.. method:: RoadRunner.getNrMatrix()
   :module: roadrunner
   
   Returns the reduced stoichiometry matrix, :math:`N_R`, which wil have only r rows where r is the rank of 
   the full stoichiometry matrix. The matrix will be reordered such that the rows of :math:`N_R` are independent.
      
   :rtype: numpy.ndarray
      
      
   
.. staticmethod:: RoadRunner.getParamPromotedSBML(*args)
   :module: roadrunner
   
   Takes an sbml document (in textual form) and changes all of the local parameters
   to be global parameters.
      
   :param str sbml: the contents of an sbml document
   :rtype: str
      
      
   
.. method:: RoadRunner.getReducedJacobian()
   :module: roadrunner
   
   Returns the *reduced* Jacobian for the independent species. This matrix will be non-singular
   for models that include moiety-concerved cycles. 
      
   :rtype: numpy.ndarray
      
      
   
.. method:: RoadRunner.getReorderedStoichiometryMatrix()
   :module: roadrunner
   
   Returns the reordered stoichiometry matrix where the tops rows represent the independent species of
   which there will be rank (N) and the bottom rows the dependent species.
      
   :rtype: numpy.ndarray
      

   
.. method:: RoadRunner.getCurrentSBML()
   :module: roadrunner
   
   Retuens the *current state* of the model in the form of an SBML string. That is the SBML
   will reflect the current state of the model and not the orginal SBML that was loaded
   into roadRunner.
      
   :rtype: str
   
   
.. method:: RoadRunner.getSBML()
   :module: roadrunner
   
   Returns the original SBML model that was loaded into roadRunner.
      
   :rtype: str
      
      
   
.. method:: RoadRunner.getScaledConcentrationControlCoefficientMatrix()
   :module: roadrunner
   
   Returns the m by n matrix of scaled conentration control coefficients where m is the number
   of floating species and n the number of reactiohs.
      
   :rtype: numpy.ndarray
      
   
.. method:: RoadRunner.getScaledFloatingSpeciesElasticity(reactionId, speciesId)
   :module: roadrunner
   
   Returns the scaled elasticity for a given reaction and given species. 
      
   :param str reactionId: the sbml id of a reaction.
   :param str speciesId: the sbml id of a species.
   :rtype: double
      
      
   
.. method:: RoadRunner.getScaledFluxControlCoefficientMatrix()
   :module: roadrunner
   
   Returns the n by n matrix of scaled flux control coefficients where n is the number of reactions.
      
   :rtype: numpy.ndarray
      
      
   
.. method:: RoadRunner.getScaledReorderedElasticityMatrix()
   :module: roadrunner
   
   Returns the unscaled elasticity matrix at the current operating point.
      
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
   
   Returns the current stoichiomentry matrix, N.
      
      
   
.. method:: RoadRunner.getTimeCourseSelectionList()
   :module: roadrunner
   
   TODO docs
      
      
   
.. method:: RoadRunner.getUnscaledConcentrationControlCoefficientMatrix()
   :module: roadrunner
   
   Returns the unscaled concentration control coefficent matrix.
      

.. method:: RoadRunner.getUnscaledElasticityMatrix()
   :module: roadrunner
   
   Returns the unscaled species elasticity matrix at the current operating point.
         
   
.. method:: RoadRunner.getUnscaledFluxControlCoefficientMatrix()
   :module: roadrunner
   
   Returns the unscaled flux control coefficient matrix.
      
   
.. method:: RoadRunner.getUnscaledSpeciesElasticity(reactionIndx, speciesIndx)
   :module: roadrunner
   
   Get a single species elasticity value.
   IMPORTANT:
   Assumes that the reaction rates have been precomputed at the operating point !!
      
   :param int reactionIndx: index of reactionl
   :param int speciesIndx: index of species.
      
      
   
.. staticmethod:: RoadRunner.getVersion()
   :module: roadrunner
   
   Returns the current version of the roadRunner library.
      
   
.. staticmethod:: RoadRunner.getlibSBMLVersion()
   :module: roadrunner

   Returns theversion of the libSBML library that is currently being used.
      
      
   
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
   
   Carry out a one step integration of the model. The method takes two arguments,
   the current time and the step size to us in the integration. The method returns
   the new time which will be currentTime + StepSize
      
     newTime = r.oneStep (10, 0.5)
      
   
.. method:: RoadRunner.reset()
   :module: roadrunner
   
   This method resets all the floating species concentrations to their initial values.
      
      
   
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

   Returns the copyright string
   
   

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
