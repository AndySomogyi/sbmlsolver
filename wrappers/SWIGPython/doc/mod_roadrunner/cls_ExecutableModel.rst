.. py:class:: ExecutableModel(*args, **kwargs)
   :module: roadrunner

   The sbml model.
   
   
   
   .. py:method:: ExecutableModel.evalInitialConditions()
      :module: roadrunner
   
              
      
   
   .. py:method:: ExecutableModel.getBoundarySpeciesAmounts(*args)
      :module: roadrunner
   
      :param numpy.ndarray index: (optional) an index array indicating which items to return.
      :returns: an array of the boundary species amounts.
      :rtype: numpy.ndarray.
      
      
   
   .. py:method:: ExecutableModel.getBoundarySpeciesConcentrations(*args)
      :module: roadrunner
   
      :param numpy.ndarray index: (optional) an index array indicating which items to return.
      :returns: an array of the boundary species concentrations. 
      :rtype: numpy.ndarray.
      
      
   
   .. py:method:: ExecutableModel.getBoundarySpeciesIds()
      :module: roadrunner
   
      :param numpy.ndarray index: (optional) an index array indicating which items to return.
      :returns: a list of boundary species ids.
      
      
   
   .. py:method:: ExecutableModel.getCompartmentIds()
      :module: roadrunner
   
      :param numpy.ndarray index: (optional) an index array indicating which items to return.
      :retuns: a list of compartment ids.
      
      
   
   .. py:method:: ExecutableModel.getCompartmentVolumes(*args)
      :module: roadrunner
   
      :param numpy.ndarray index: (optional) an index array indicating which items to return.
      :returns: an array of compartment volumes.
      :rtype: numpy.ndarray.
      
      
   
   .. py:method:: ExecutableModel.getConservedSumChanged()
      :module: roadrunner
   
              
      
   
   .. py:method:: ExecutableModel.getConservedSums(*args)
      :module: roadrunner
   
      :param numpy.ndarray index: (optional) an index array indicating which items to return.
      :returns: an array of conserved sums.
      :rtype: numpy.ndarray.
      
      
   
   .. py:method:: ExecutableModel.getFloatingSpeciesAmountRates(*args)
      :module: roadrunner
   
      :param numpy.ndarray index: (optional) an index array indicating which items to return.
      :returns: an array of the rates of change of the floating species amounts.
      :rtype: numpy.ndarray
      
      
   
   .. py:method:: ExecutableModel.getFloatingSpeciesAmounts(*args)
      :module: roadrunner
   
      Get the list of floating species amounts. If no arguments are given, this
      returns all floating species amounts.
      
      :param index: an optional array of desired floating species indices. i.e. if this model has
              4 floating species and we want the amounts for the last and first, we
              would use [3,0] for the index array.
      :type name: numpy.ndarray
      
      get all the amounts:
      
      >>> e.getFloatingSpeciesAmounts()
      [15,2,3,20]
      
      get amounts 3 and 0:
      
      >>> getFloatingSpeciesAmounts([3,0])
      [10,15]
      
      
      
   
   .. py:method:: ExecutableModel.getFloatingSpeciesConcentrations(*args)
      :module: roadrunner
   
      :param numpy.ndarray index: (optional) an index array indicating which items to return.
      :returns: an array of floating species concentrations.
      :rtype: numpy.ndarray
      
      
   
   .. py:method:: ExecutableModel.getGlobalParameterValues(*args)
      :module: roadrunner
   
      :param numpy.ndarray index: (optional) an index array indicating which items to return.
      :returns: an array of global parameter values.
      :rtype: numpy.ndarray.
      
      
   
   .. py:method:: ExecutableModel.getInfo()
      :module: roadrunner
   
              
      
   
   .. py:method:: ExecutableModel.getModelName()
      :module: roadrunner
   
      Get the model name specified in the sbml
      
      
   
   .. py:method:: ExecutableModel.getNumBoundarySpecies()
      :module: roadrunner
   
              
      
   
   .. py:method:: ExecutableModel.getNumCompartments()
      :module: roadrunner
   
              
      
   
   .. py:method:: ExecutableModel.getNumConservedSums()
      :module: roadrunner
   
              
      
   
   .. py:method:: ExecutableModel.getNumDependentSpecies()
      :module: roadrunner
   
              
      
   
   .. py:method:: ExecutableModel.getNumFloatingSpecies()
      :module: roadrunner
   
              
      
   
   .. py:method:: ExecutableModel.getNumGlobalParameters()
      :module: roadrunner
   
              
      
   
   .. py:method:: ExecutableModel.getNumIndependentSpecies()
      :module: roadrunner
   
              
      
   
   .. py:method:: ExecutableModel.getNumReactions()
      :module: roadrunner
   
              
      
   
   .. py:method:: ExecutableModel.getNumRules()
      :module: roadrunner
   
              
      
   
   .. py:method:: ExecutableModel.getReactionIds()
      :module: roadrunner
   
      :param numpy.ndarray index: (optional) an index array indicating which items to return.
      :returns: a list of reaction ids.
      
      
   
   .. py:method:: ExecutableModel.getReactionRates(*args)
      :module: roadrunner
   
      :param numpy.ndarray index: (optional) an index array indicating which items to return.
      :returns: an array of reaction rates.
      :rtype: numpy.ndarray
      
      
   
   .. py:method:: ExecutableModel.getTime()
      :module: roadrunner
   
              
      
   
   .. py:method:: ExecutableModel.reset()
      :module: roadrunner
   
              
      
   
   .. py:method:: ExecutableModel.setBoundarySpeciesConcentrations(*args)
      :module: roadrunner
   
      :param numpy.ndarray index: (optional) an index array indicating which items to set, 
              or if no index array is given, the first param should be an array of all the 
              values to set.
      :param numpy.ndarray values' the values to set.
      
      
      
   
   .. py:method:: ExecutableModel.setCompartmentVolumes(*args)
      :module: roadrunner
   
      :param numpy.ndarray index: (optional) an index array indicating which items to set, 
              or if no index array is given, the first param should be an array of all the 
              values to set.
      :param numpy.ndarray values' the values to set.
      
      
      
   
   .. py:method:: ExecutableModel.setConservedSumChanged(*args)
      :module: roadrunner
   
              
      
   
   .. py:method:: ExecutableModel.setConservedSums(*args)
      :module: roadrunner
   
      :param numpy.ndarray index: (optional) an index array indicating which items to set, 
              or if no index array is given, the first param should be an array of all the 
              values to set.
      :param numpy.ndarray values' the values to set.
      
      
      
   
   .. py:method:: ExecutableModel.setFloatingSpeciesAmounts(*args)
      :module: roadrunner
   
      :param numpy.ndarray index: (optional) an index array indicating which items to set, 
              or if no index array is given, the first param should be an array of all the 
              values to set.
      :param numpy.ndarray values' the values to set.
      
      
   
   .. py:method:: ExecutableModel.setFloatingSpeciesConcentrations(*args)
      :module: roadrunner
   
      :param numpy.ndarray index: (optional) an index array indicating which items to set, 
              or if no index array is given, the first param should be an array of all the 
              values to set.
      :param numpy.ndarray values' the values to set.
      
      
   
   .. py:method:: ExecutableModel.setGlobalParameterValues(*args)
      :module: roadrunner
   
      :param numpy.ndarray index: (optional) an index array indicating which items to set, 
              or if no index array is given, the first param should be an array of all the 
              values to set.
      :param numpy.ndarray values' the values to set.
      
      
   
   .. py:method:: ExecutableModel.setTime(*args)
      :module: roadrunner
   
      Set the model time variable.
      
      :param time: time the time value to set.
      :returns: None
