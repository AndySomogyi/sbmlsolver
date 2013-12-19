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
   


