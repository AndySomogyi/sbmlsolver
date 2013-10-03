**********************************
Accessing the SBML Model Variables
**********************************

.. currentmodule:: roadrunner

The following methods allow users to obtain information on the model


Compartment Group
-----------------

.. autosummary::

   ExecutableModel.getCompartmentVolumes
   ExecutableModel.getNumCompartments
   ExecutableModel.getCompartmentIds
   ExecutableModel.setCompartmentVolumes

   
Boundary Species Group
----------------------

.. autosummary::

   ExecutableModel.getBoundarySpeciesConcentrations
   ExecutableModel.setBoundarySpeciesConcentrations
   ExecutableModel.getNumBoundarySpecies
   ExecutableModel.getBoundarySpeciesIds


Floating Species Group
----------------------

.. autosummary::

   ExecutableModel.getFloatingSpeciesConcentrations
   ExecutableModel.setFloatingSpeciesConcentrations
   ExecutableModel.getNumFloatingSpecies
   ExecutableModel.getFloatingSpeciesIds


Initial Conditions Group
------------------------

.. autosummary::

Reaction Group
--------------

.. autosummary::

   ExecutableModel.getNumReactions
   ExecutableModel.getReactionRates
   ExecutableModel.getReactionIds


Rates Of Change Group
---------------------

.. autosummary::

   ExecutableModel.getFloatingSpeciesAmountRates
   ExecutableModel.getReactionRates
   ExecutableModel.getNumReactions
   ExecutableModel.getReactionIds


Parameters Group
----------------

.. autosummary::
   ExecutableModel.setGlobalParameterValues
   ExecutableModel.getGlobalParameterValues
   ExecutableModel.getNumGlobalParameters
   ExecutableModel.getNumGlobalParameters
   
Current State of the System Group
---------------------------------

When using the LLVM back end, all model state calculation are automatically 
performed using a techinque called lazy evaluation. If one sets the concentration 
of a species, the amount of of that species is automatically available without 
having to perform any addition operations. Similarly with any other value in the model. 

If an SBML parameter is defined by an assigment rule or function and its value then 
depends on a number of other values, simply setting to other values automatically 
cause the value of the most dependent variable to be set. 

This is identical how one operates in a spredsheet such as Microsoft Excel. For 
example, if one has a cell with an equation that depends on other cell, and those
other cell depend on other values, setting the value of any upstream cell automatically
causes that value to cascade down to the terminal cells. The LLVM back end roadruner
function identically. 

