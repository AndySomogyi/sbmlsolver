Accessing the SBML Model Variables
__________________________________

All of the SBML model variables are accessed via the ``RoadRunner.model`` object. This is an instance of
the ExecutableModel class described here. One always access the model object that belongs to the top
RoadRunner object, i.e. ``r.model``, where ``r`` is an instance of the ``RoadRunner`` class. 

The ExecutableModel class also implements the Python dictionary protocol, so that it can be used as
a dictionary. The dictionary keys are all of the symbols specified in the original model as well as
a number of selection strings described in the Selections section. 

.. method:: ExecutableModel.keys()
   :module: roadrunner

   Get a list of all the keys that this model has. This is a very good way of looking at all the
   available symbols and selection strings:

   >>>  r.model.keys()
   [ 'S1', 'S2', '[S1]', '[S2]', 'compartment', 'k1', 'cm0',  
     'reaction1',  'init([S1])',  'init([S2])', 'init(S1)',  
     'init(S2)',  "S1'"]


.. method:: ExecutableModel.items()
   :module: roadrunner

   Get a list of key / value pairs of all the selections / values in this model. 

.. method:: ExecutableModel.__getitem__
   :module: roadrunner

   Implements the python ``[]`` indexing operator, so the model values can be accessed like::

     >>> r.model["S1"]
     0.0

.. method:: ExecutableModel.__setitem__
   :module: roadrunner

   Implements the python ``[]`` indexing operator for setting values::

     >>> r.model["S1"] = 12.3

   Note, some keys are read only such as values defined by rules, or calculated values such as
   species amount rates or reaction rates. If one attempts to set the value of a read-only symbol,
   an exception is raised indicating the error, and no harm done. 



Floating Species
----------------

.. method:: ExecutableModel.getNumFloatingSpecies()
   :module: roadrunner

   Returns the number of floating species in the model.


.. method:: ExecutableModel.getFloatingSpeciesIds()

   Return a list of floating species sbml ids.


.. method:: ExecutableModel.getFloatingSpeciesAmountRates([index])

   Return a vector of the amount rate of change of the floating species.

   The units of amount rates is amount / time.

   :param numpy.ndarray index: (optional) an index array indicating which items to return.
   :returns: an array of the rates of change of the floating species amounts.
   :rtype: numpy.ndarray


.. method:: ExecutableModel.getFloatingSpeciesAmounts([index])
   :module: roadrunner

   Get the list of floating species amounts. If no arguments are given, this
   returns all floating species amounts.

   :param index: an optional array of desired floating species indices. i.e. if this model has
                 4 floating species and we want the amounts for the last and first, we
                 would use [3,0] for the index array.
   :type name: numpy.ndarray

   get all the amounts::

     >>> e.getFloatingSpeciesAmounts()
     [15,2,3,20]

   get amounts 3 and 0::

     >>> getFloatingSpeciesAmounts([3,0])
     [10,15]


.. method:: ExecutableModel.setFloatingSpeciesAmounts([index], values)
   :module: roadrunner

   Use this to set the entire set of floating species amounts in one call.
   The order of species is given by the order of Ids returned by getFloatingSpeciesIds()

   :param numpy.ndarray index: (optional) an index array indicating which items to set,
                               or if no index array is given, the first param should be
                               an array of all the values to set.
   :param numpy.ndarray values: the values to set.



.. method:: ExecutableModel.setFloatingSpeciesConcentrations([index], values)
   :module: roadrunner

   Use this to set the entire set of floating species concentrations in one call.
   The order of species is given by the order of Ids returned by getFloatingSpeciesIds()


   :param numpy.ndarray index: (optional) an index array indicating which items to set,
                               or if no index array is given, the first param should be an
                               array of all the  values to set.
   :param numpy.ndarray values: the values to set.



.. method:: ExecutableModel.getFloatingSpeciesConcentrations([index])

   Returns a vector of floating species concentrations. The order of species is
   given by the order of Ids returned by getFloatingSpeciesIds()

   :param numpy.ndarray index: (optional) an index array indicating which items to return.
   :returns: an array of floating species concentrations.
   :rtype: numpy.ndarray

Floating Species Initial Conditions
-----------------------------------
RoadRunner stores all initial conditions separately from the model state variables. This means that
you can update the initial conditions at any time, and it does not affect the current state of the
model. To reset the model, that is, reset it to its original state, or a new original state where
what has changed the initial conditions use the :meth:`~ExecutableModel.reset()` method. 

The following methods allow access to the floating species initial condition values:

.. method:: ExecutableModel.getFloatingSpeciesInitAmountIds()
   :module: roadrunner

   get a list of the floating species amount initial amount selection symbols.

.. method:: ExecutableModel.getFloatingSpeciesInitConcentrationIds()
   :module: roadrunner

   get a list of the floating species amount initial concentration selection symbols.


.. method:: ExecutableModel.getFloatingSpeciesInitConcentrations([index])    
   :module: roadrunner

   Returns a vector of floating species initial concentrations. The order of species is
   given by the order of Ids returned by getFloatingSpeciesInitialConcentrationIds()

   :param numpy.ndarray index: (optional) an index array indicating which items to return.
   :returns: an array of floating species initial concentrations.
   :rtype: numpy.ndarray

.. method:: ExecutableModel.setFloatingSpeciesInitConcentrations([index], values)
   :module: roadrunner

   Set a vector of floating species initial concentrations. The order of species is
   given by the order of Ids returned by getFloatingSpeciesInitialAmountIds()

   :param numpy.ndarray index: (optional) an index array indicating which items to return.


.. method:: ExecutableModel.getFloatingSpeciesInitAmounts([index])    
   :module: roadrunner

   Returns a vector of floating species initial amounts. The order of species is
   given by the order of Ids returned by getFloatingSpeciesInitialConcentrationIds()

   :param numpy.ndarray index: (optional) an index array indicating which items to return.
   :returns: an array of floating species initial amounts.
   :rtype: numpy.ndarray


.. method:: ExecutableModel.setFloatingSpeciesInitAmounts([index], values)
   :module: roadrunner

   Set a vector of floating species initial amounts. The order of species is
   given by the order of Ids returned by getFloatingSpeciesInitialAmountIds()

   :param numpy.ndarray index: (optional) an index array indicating which items to return.




Boundary Species
----------------

.. method:: ExecutableModel.getBoundarySpeciesAmounts([index])
   :module: roadrunner

   Returns a vector of boundary species amounts. The order of species is
   given by the order of Ids returned by getBoundarySpeciesIds()

   :param numpy.ndarray index: (optional) an index array indicating which items to return.
   :returns: an array of the boundary species amounts.
   :rtype: numpy.ndarray.

   given by the order of Ids returned by getBoundarySpeciesIds()

   :param numpy.ndarray index: (optional) an index array indicating which items to return.
   :returns: an array of the boundary species amounts.
   :rtype: numpy.ndarray.


.. method:: ExecutableModel.getBoundarySpeciesConcentrations([index])
   :module: roadrunner

   Returns a vector of boundary species concentrations. The order of species is
   given by the order of Ids returned by getBoundarySpeciesIds()

   :param numpy.ndarray index: (optional) an index array indicating which items to return.
   :returns: an array of the boundary species concentrations.
   :rtype: numpy.ndarray.

   given by the order of Ids returned by getBoundarySpeciesIds()

   :param numpy.ndarray index: (optional) an index array indicating which items to return.
   :returns: an array of the boundary species concentrations.
   :rtype: numpy.ndarray.



.. method:: ExecutableModel.getBoundarySpeciesIds()
   :module: roadrunner

   Returns a vector of boundary species Ids.

   :param numpy.ndarray index: (optional) an index array indicating which items to return.
   :returns: a list of boundary species ids.


.. method:: ExecutableModel.getNumBoundarySpecies()
   :module: roadrunner

   Returns the number of boundary species in the model.


.. method:: ExecutableModel.setBoundarySpeciesConcentrations([index], values)
   :module: roadrunner

   Use this to set the entire set of boundary species concentrations in one call.
   The order of species is given by the order of Ids returned by getBoundarySpeciesIds()

   :param numpy.ndarray index: (optional) an index array indicating which items to set,
                               or if no index array is given, the first param should be an
                               array of all the  values to set.

   :param numpy.ndarray values: the values to set.

Compartments
------------

.. method:: ExecutableModel.getCompartmentIds([index])

   Returns a vector of compartment identifier symbols.

   :param index: A array of compartment indices indicating which compartment ids to return.
   :type index: None or numpy.ndarray
   :returns: a list of compartment ids.

.. method:: ExecutableModel.getCompartmentVolumes([index])

   Returns a vector of compartment volumes. The order of volumes is
   given by the order of Ids returned by getCompartmentIds()

   :param numpy.ndarray index: (optional) an index array indicating which items to return.
   :returns: an array of compartment volumes.
   :rtype: numpy.ndarray.

.. method:: ExecutableModel.getNumCompartments()

   Returns the number of compartments in the model.

   :rtype: int


.. method:: ExecutableModel.setCompartmentVolumes([index], values)

   Sets a vector of compartment volumes.

   If the index vector is not give, then the values vector treated as a vector of all
   compartment volumes to set. If index is given, then  values should have the same
   length as index.

   :param numpy.ndarray index: (optional) an index array indicating which items to set,
                               or if no index array is given, the first param should be an
                               array of all the  values to set.

   :param numpy.ndarray values: the values to set.



Global Parameters
-----------------


.. method:: ExecutableModel.getGlobalParameterValues([index])
   :module: roadrunner

   Return a vector of global parameter values. The order of species is
   given by the order of Ids returned by getGlobalParameterIds()

   :param numpy.ndarray index: (optional) an index array indicating which items to return.
   :returns: an array of global parameter values.
   :rtype: numpy.ndarray.


.. method:: ExecutableModel.getNumGlobalParameters()
   :module: roadrunner


   Returns the number of global parameters in the model.


.. method:: ExecutableModel.setGlobalParameterValues([index], values)
   :module: roadrunner

   Use this to set the entire set of global parameters in one call.
   The order of parameters is given by the order of Ids returned by getGlobalParameterIds()


   :param numpy.ndarray index: (optional) an index array indicating which items to set,
                               or if no index array is given, the first param should be an
                               array of all the values to set.
   :param numpy.ndarray values: the values to set.

Reactions
---------

.. method:: ExecutableModel.getNumReactions()
   :module: roadrunner

   Returns the number of reactions in the model.


.. method:: ExecutableModel.getReactionIds()
   :module: roadrunner

   Returns a vector of reaction Ids.

   :param numpy.ndarray index: (optional) an index array indicating which items to return.
   :returns: a list of reaction ids.



.. method:: ExecutableModel.getReactionRates([index])
   :module: roadrunner

   Returns a vector of reaction rates (reaction velocity) for the current state of the model. The
   order of reaction rates is given by the order of Ids returned by getReactionIds()

   :param numpy.ndarray index: (optional) an index array indicating which items to return.
   :returns: an array of reaction rates.
   :rtype: numpy.ndarray



Stoichiometry
-------------

.. method:: ExecutableModel.getStoichiometry(speciesIndex, reactionIndex)

   Returns the stochiometric coefficient for the given species index and reaction index. 

   Frequently one does not need the full stochiometrix matrix, particularly if the system is 
   large and only a single coefficient is needed. 


   :param speciesIndex: a floating species index from :meth:`getFloatingSpeciesIds`
   :param reactionIndex: a reaction index from :meth:`getReactionIds`



State Vector
------------

.. method:: ExecutableModel.getStateVector([stateVector])
            
   Returns a vector of all the variables that represent the state of the system. The state is
   considered all values which change with the dynamics of the model. This would include all species
   which are produced or consumed by reactions, and all variables which are defined by rate rules. 

   Variables such as global parameters, compartments, or boundary species which do not change with
   the model dynamics are considered parameters and are thus not part of the state. 

   In performance critical applications, the optional stateVector array should be provided where the
   output variables will be written to. 


   :param numpy.ndarray stateVector: an optional numpy array where the state vector variables will be written. If
                       no state vector array is given, a new one will be constructed and returned. 

                       This should be the same length as the model state vector. 
   :rtype: numpy.ndarray

               
.. method:: ExecutableModel.getStateVectorId(index)

   Get the id (symbolic name) of a state vector item. 

   :param int index: the index of the desired state vector item
   :rtype: str

.. method:: ExecutableModel.getStateVectorIds()

   Returns a list of all state vector ids

   :rtype: list

.. method:: ExecutableModel.getStateVectorRate(time, [stateVector], [stateVectorRate])

   Calculates the rate of change of all state vector variables. 

   Note, the rate of change of species returned by this method is always in units of amount /
   time. 


   :param double time: the model time at which the calculation should be performed. 
   :param numpy.ndarray: (optional) the model state at which the calculation should be performed. If
                         this is not give, the current state is used. 
   :param numpy.ndarray: (optional) an output array where the rates of change will be written to. If
                         this is not given, a new array is allocated and returned. 

   :returns: an array of the rates of change of all state vector variables.
   :rtype: numpy.ndarray


Conserved Moieties
------------------


.. method:: ExecutableModel.getNumConservedMoieties()

   Returns the number of conserved moieties in the model.

   :rtype: int


.. method:: ExecutableModel.getConservedMoietyIds([index])

   Returns a vector of conserved moiety identifier symbols.


   :param index: A array of compartment indices indicating which compartment ids to return.
   :type index: None or numpy.ndarray
   :returns: a list of compartment ids.

.. method:: ExecutableModel.getConservedMoietyValues([index])

   Returns a vector of conserved moiety volumes. The order of values is
   given by the order of Ids returned by getConservedMoietyIds()

   :param numpy.ndarray index: (optional) an index array indicating which items to return.
   :returns: an array of conserved moiety values.
   :rtype: numpy.ndarray.

.. method:: ExecutableModel.setConservedMoietyValues([index], values)

   Sets a vector of conserved moiety values.

   *Note* This method currently only updates the conserved moiety values, it does 
   not update the initial species condition from which the values were calculated. 
   
   If the index vector is not given, then the values vector treated as a vector of all
   values to set. If index is given, then  values should have the same
   length as index.

   :param numpy.ndarray index: (optional) an index array indicating which items to set,
                               or if no index array is given, the first param should be an
                               array of all the  values to set.
   :param numpy.ndarray values: the values to set.


Misc
----


.. method:: ExecutableModel.getInfo()
   :module: roadrunner

   get various info about the model.

.. method:: ExecutableModel.getModelName()
   :module: roadrunner

   Get the model name specified in the SBML.


.. method:: ExecutableModel.getTime()
   :module: roadrunner

   Get the model time. The model originally start at time t=0 and is advaced forward in time by the
   integrator. So, if one ran a simulation from time = 0 to time = 10, the model will then have it's
   time = 10. 


.. method:: ExecutableModel.reset()
   :module: roadrunner

   Reset the floating species concentration to their initial conditions.



.. method:: ExecutableModel.setTime(time)
   :module: roadrunner

   Set the model time variable. 

   :param time: time the time value to set.




