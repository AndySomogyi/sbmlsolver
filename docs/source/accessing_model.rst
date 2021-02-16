**********************************
Accessing the SBML Model Variables
**********************************

.. currentmodule:: RoadRunner

The following methods allow users to obtain information on the model


Compartments
------------

   * :cpp:class:`rr::ExecutableModel::getCompartmentVolumes`
   * :cpp:class:`rr::ExecutableModel::getNumCompartments`
   * :cpp:class:`rr::ExecutableModel::getCompartmentIds`
   * :cpp:class:`rr::ExecutableModel::setCompartmentVolumes`

   
Boundary Species
----------------

   * :cpp:class:`rr::ExecutableModel::getBoundarySpeciesConcentrations`
   * :cpp:class:`rr::ExecutableModel::setBoundarySpeciesConcentrations`
   * :cpp:class:`rr::ExecutableModel::getNumBoundarySpecies`
   * :cpp:class:`rr::ExecutableModel::getBoundarySpeciesIds`


Floating Species
----------------

   * :cpp:class:`rr::ExecutableModel::getFloatingSpeciesConcentrations`
   * :cpp:class:`rr::ExecutableModel::setFloatingSpeciesConcentrations`
   * :cpp:class:`rr::ExecutableModel::getNumFloatingSpecies`
   * :cpp:class:`rr::ExecutableModel::getFloatingSpeciesIds`


Initial Conditions
------------------

   * :cpp:class:`rr::ExecutableModel::getFloatingSpeciesInitAmountIds`
   * :cpp:class:`rr::ExecutableModel::getFloatingSpeciesInitConcentrationIds`
   * :cpp:class:`rr::ExecutableModel::getFloatingSpeciesInitAmounts`
   * :cpp:class:`rr::ExecutableModel::setFloatingSpeciesInitAmounts`
   * :cpp:class:`rr::ExecutableModel::getFloatingSpeciesInitConcentrations`
   * :cpp:class:`rr::ExecutableModel::setFloatingSpeciesInitConcentrations`

Reactions
---------

   * :cpp:class:`rr::ExecutableModel::getNumReactions`
   * :cpp:class:`rr::ExecutableModel::getReactionRates`
   * :cpp:class:`rr::ExecutableModel::getReactionIds`


Reaction Rates
---------------

   * :cpp:class:`rr::ExecutableModel::getFloatingSpeciesAmountRates`
   * :cpp:class:`rr::ExecutableModel::getReactionRates`
   * :cpp:class:`rr::ExecutableModel::getNumReactions`
   * :cpp:class:`rr::ExecutableModel::getReactionIds`


Global Parameters
-----------------

   * :cpp:class:`rr::ExecutableModel::setGlobalParameterValues`
   * :cpp:class:`rr::ExecutableModel::getGlobalParameterValues`
   * :cpp:class:`rr::ExecutableModel::getNumGlobalParameters`
   * :cpp:class:`rr::ExecutableModel::getNumGlobalParameters`
   


