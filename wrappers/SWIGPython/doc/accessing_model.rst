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
   ExecutableModel.setBoundarySpeciesByIndex
   ExecutableModel.getBoundarySpeciesByIndex
   ExecutableModel.setBoundarySpeciesConcentrations
   ExecutableModel.getNumberOfBoundarySpecies
   ExecutableModel.getBoundarySpeciesIds


Floating Species Group
----------------------

.. autosummary::

   ExecutableModel.getFloatingSpeciesConcentrations
   ExecutableModel.setFloatingSpeciesByIndex
   ExecutableModel.getFloatingSpeciesByIndex
   ExecutableModel.setFloatingSpeciesConcentrations
   ExecutableModel.getNumberOfFloatingSpecies
   ExecutableModel.getFloatingSpeciesIds


Initial Conditions Group
------------------------

.. autosummary::

Reaction Group
--------------

.. autosummary::

   ExecutableModel.getNumberOfReactions
   ExecutableModel.getReactionRate
   ExecutableModel.getReactionRates
   ExecutableModel.getReactionRatesEx
   ExecutableModel.getReactionIds


Rates Of Change Group
---------------------

.. autosummary::


Parameters Group
----------------

.. autosummary::

Current State of the System Group
--------------------------------

.. autosummary::
