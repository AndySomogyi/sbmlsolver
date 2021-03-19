"""Unit test style tests for roadrunner Python API

The integration tests are adequately handled by tester.py,
though it's a shame unittest is not used. These tests are
not about simulation accuracy, but rather checking that each
little part of the API works in isolation.

"""

import unittest
import sys

sys.path += [
    r"D:\roadrunner\roadrunner\install-msvc2019-rel\site-packages"
]


import roadrunner
from roadrunner.roadrunner import RoadRunner


# tests for _RoadRunner__simulateOptions


class RoadRunnerTests(unittest.TestCase):
    sbml = """<?xml version="1.0" encoding="UTF-8"?>
<!-- Created by libAntimony version v2.5 on 2014-08-04 19:56 with libSBML version 5.9.1. -->
<sbml xmlns="http://www.sbml.org/sbml/level3/version1/core" level="3" version="1">
  <model id="__main" name="__main">
    <listOfCompartments>
      <compartment sboTerm="SBO:0000410" id="default_compartment" spatialDimensions="3" size="1" constant="true"/>
    </listOfCompartments>
    <listOfSpecies>
      <species id="S1" compartment="default_compartment" initialConcentration="10" hasOnlySubstanceUnits="false" boundaryCondition="true" constant="false"/>
      <species id="S2" compartment="default_compartment" initialConcentration="0" hasOnlySubstanceUnits="false" boundaryCondition="false" constant="false"/>
      <species id="S3" compartment="default_compartment" initialConcentration="0" hasOnlySubstanceUnits="false" boundaryCondition="true" constant="false"/>
    </listOfSpecies>
    <listOfParameters>
      <parameter id="k1" value="0.1" constant="true"/>
      <parameter id="k2" value="0.2" constant="true"/>
    </listOfParameters>
    <listOfReactions>
      <reaction id="_J0" reversible="true" fast="false">
        <listOfReactants>
          <speciesReference species="S1" stoichiometry="1" constant="true"/>
        </listOfReactants>
        <listOfProducts>
          <speciesReference species="S2" stoichiometry="1" constant="true"/>
        </listOfProducts>
        <kineticLaw>
          <math xmlns="http://www.w3.org/1998/Math/MathML">
            <apply>
              <times/>
              <ci> k1 </ci>
              <ci> S1 </ci>
            </apply>
          </math>
        </kineticLaw>
      </reaction>
      <reaction id="_J1" reversible="true" fast="false">
        <listOfReactants>
          <speciesReference species="S2" stoichiometry="1" constant="true"/>
        </listOfReactants>
        <listOfProducts>
          <speciesReference species="S3" stoichiometry="1" constant="true"/>
        </listOfProducts>
        <kineticLaw>
          <math xmlns="http://www.w3.org/1998/Math/MathML">
            <apply>
              <times/>
              <ci> k2 </ci>
              <ci> S2 </ci>
            </apply>
          </math>
        </kineticLaw>
      </reaction>
    </listOfReactions>
  </model>
</sbml>
"""

    def setUp(self) -> None:
        self.rr = RoadRunner(self.sbml)

    def tearDown(self) -> None:
        pass

    def test_create_model(self):
        for i in sorted(dir(self.rr)):
            print(i)

    def test_species_attribute(self):
        self.assertEqual(self.rr.S1, 10)

    def test_species_attribute_amount(self):
        self.assertEqual(self.rr.S1_amt, 10)

    def test_species_attribute_conc(self):
        self.assertEqual(self.rr.S1_conc, 10)

    def test_reaction_flux(self):
        self.assertEqual(self.rr._J0, 1)

    def test__RoadRunner__simulateOptions(self):
        print([i for i in dir(self.rr) if "sim" in i.lower()])
        print(self.rr._RoadRunner__simulateOptions)

    def test___class__(self):
        self.rr

    def test___delattr__(self):
        self.rr

    def test___dict__(self):
        self.rr

    def test___dir__(self):
        self.rr

    def test___doc__(self):
        self.rr

    def test___eq__(self):
        self.rr

    def test___format__(self):
        self.rr

    def test___ge__(self):
        self.rr

    def test___getattribute__(self):
        self.rr

    def test___getitem__(self):
        self.rr

    def test___gt__(self):
        self.rr

    def test___hash__(self):
        self.rr

    def test___init__(self):
        self.rr

    def test___init_subclass__(self):
        self.rr

    def test___le__(self):
        self.rr

    def test___len__(self):
        self.rr

    def test___lt__(self):
        self.rr

    def test___module__(self):
        self.rr

    def test___ne__(self):
        self.rr

    def test___new__(self):
        self.rr

    def test___reduce__(self):
        self.rr

    def test___reduce_ex__(self):
        self.rr

    def test___repr__(self):
        self.rr

    def test___setattr__(self):
        self.rr

    def test___setitem__(self):
        self.rr

    def test___sizeof__(self):
        self.rr

    def test___str__(self):
        self.rr

    def test___subclasshook__(self):
        self.rr

    def test___swig_destroy__(self):
        self.rr

    def test___weakref__(self):
        self.rr

    def test__addCompartment(self):
        self.rr

    def test__addParameter(self):
        self.rr

    def test__addReaction(self):
        self.rr

    def test__addSpecies(self):
        self.rr

    def test__diffstep_getter(self):
        self.rr

    def test__diffstep_stter(self):
        self.rr

    def test__getConservedMoietyAnalysis(self):
        self.rr

    def test__getDependentFloatingSpeciesIds(self):
        self.rr

    def test__getIndependentFloatingSpeciesIds(self):
        self.rr

    def test__getModel(self):
        self.rr

    def test__getSelections(self):
        self.rr

    def test__getSteadyStateSelections(self):
        self.rr

    def test__getValue(self):
        self.rr

    def test__listProperties(self):
        self.rr

    def test__load(self):
        self.rr

    def test__makeProperties(self):
        self.rr

    def test__new_init(self):
        self.rr

    def test__properties(self):
        self.rr

    def test__setConservedMoietyAnalysis(self):
        self.rr

    def test__setConservedMoietyAnalysisProxy(self):
        self.rr

    def test__setSelections(self):
        self.rr

    def test__setSteadyStateSelections(self):
        self.rr

    def test__simulate(self):
        self.rr

    def test__steadyStateThresh_getter(self):
        self.rr

    def test__steadyStateThresh_setter(self):
        self.rr

    def test__swig_init(self):
        self.rr

    def test_addAssignmentRule(self):
        self.rr

    def test_addCompartment(self):
        self.rr

    def test_addDelay(self):
        self.rr

    def test_addEvent(self):
        self.rr

    def test_addEventAssignment(self):
        self.rr

    def test_addInitialAssignment(self):
        self.rr

    def test_addParameter(self):
        self.rr

    def test_addPriority(self):
        self.rr

    def test_addRateRule(self):
        self.rr

    def test_addReaction(self):
        self.rr

    def test_addSpecies(self):
        self.rr

    def test_addTrigger(self):
        self.rr

    def test_clearModel(self):
        self.rr

    def test_conservedMoietyAnalysis(self):
        self.rr

    def test_createSelection(self):
        self.rr

    def test_default_compartment(self):
        self.rr

    def test_diffstep(self):
        self.rr

    def test_ensureSolversRegistered(self):
        self.rr

    def test_getAvailableIntegrators(self):
        self.rr

    def test_getBoundarySpeciesAmountsNamedArray(self):
        self.rr

    def test_getBoundarySpeciesConcentrationsNamedArray(self):
        self.rr

    def test_getCC(self):
        self.rr

    def test_getCompiler(self):
        self.rr

    def test_getConservationMatrix(self):
        self.rr

    def test_getConservedMoietyIds(self):
        self.rr

    def test_getConservedMoietyValues(self):
        self.rr

    def test_getCurrentSBML(self):
        self.rr

    def test_getDependentFloatingSpeciesIds(self):
        self.rr

    def test_getDependentRatesOfChange(self):
        self.rr

    def test_getDependentRatesOfChangeNamedArray(self):
        self.rr

    def test_getDiffStepSize(self):
        self.rr

    def test_getEE(self):
        self.rr

    def test_getEigenValueIds(self):
        self.rr

    def test_getExistingIntegratorNames(self):
        self.rr

    def test_getExtendedStoichiometryMatrix(self):
        self.rr

    def test_getExtendedVersionInfo(self):
        self.rr

    def test_getFloatingSpeciesAmountsNamedArray(self):
        self.rr

    def test_getFloatingSpeciesConcentrationsNamedArray(self):
        self.rr

    def test_getFloatingSpeciesInitialConcentrationIds(self):
        self.rr

    def test_getFrequencyResponse(self):
        self.rr

    def test_getFullEigenValues(self):
        self.rr

    def test_getFullJacobian(self):
        self.rr

    def test_getFullStoichiometryMatrix(self):
        self.rr

    def test_getGlobalParameterByName(self):
        self.rr

    def test_getIds(self):
        self.rr

    def test_getIndependentFloatingSpeciesIds(self):
        self.rr

    def test_getIndependentRatesOfChange(self):
        self.rr

    def test_getIndependentRatesOfChangeNamedArray(self):
        self.rr

    def test_getInfo(self):
        self.rr

    def test_getInstanceCount(self):
        self.rr

    def test_getInstanceID(self):
        self.rr

    def test_getIntegrator(self):
        self.rr

    def test_getIntegratorByName(self):
        self.rr

    def test_getKMatrix(self):
        self.rr

    def test_getKineticLaw(self):
        self.rr

    def test_getL0Matrix(self):
        self.rr

    def test_getLinkMatrix(self):
        self.rr

    def test_getModel(self):
        self.rr

    def test_getNrMatrix(self):
        self.rr

    def test_getParamPromotedSBML(self):
        self.rr

    def test_getRatesOfChange(self):
        self.rr

    def test_getRatesOfChangeNamedArray(self):
        self.rr

    def test_getReactionRates(self):
        self.rr

    def test_getReducedEigenValues(self):
        self.rr

    def test_getReducedJacobian(self):
        self.rr

    def test_getReducedStoichiometryMatrix(self):
        self.rr

    def test_getRegisteredIntegratorNames(self):
        self.rr

    def test_getRegisteredSteadyStateSolverNames(self):
        self.rr

    def test_getSBML(self):
        self.rr

    def test_getScaledConcentrationControlCoefficientMatrix(self):
        self.rr

    def test_getScaledElasticityMatrix(self):
        self.rr

    def test_getScaledFloatingSpeciesElasticity(self):
        self.rr

    def test_getScaledFluxControlCoefficientMatrix(self):
        self.rr

    def test_getSelectedValues(self):
        self.rr

    def test_getSimulationData(self):
        self.rr

    def test_getSteadyStateSelectionStrings(self):
        self.rr

    def test_getSteadyStateSolver(self):
        self.rr

    def test_getSteadyStateThreshold(self):
        self.rr

    def test_getSteadyStateValues(self):
        self.rr

    def test_getSteadyStateValuesNamedArray(self):
        self.rr

    def test_getSupportedIdTypes(self):
        self.rr

    def test_getUnscaledConcentrationControlCoefficientMatrix(self):
        self.rr

    def test_getUnscaledElasticityMatrix(self):
        self.rr

    def test_getUnscaledFluxControlCoefficientMatrix(self):
        self.rr

    def test_getUnscaledParameterElasticity(self):
        self.rr

    def test_getUnscaledSpeciesElasticity(self):
        self.rr

    def test_getValue(self):
        self.rr

    def test_getuCC(self):
        self.rr

    def test_getuEE(self):
        self.rr

    def test_integrator(self):
        self.rr

    def test_integratorExists(self):
        self.rr

    def test_internalOneStep(self):
        self.rr

    def test_isModelLoaded(self):
        self.rr

    def test_items(self):
        self.rr

    def test_iteritems(self):
        self.rr

    def test_iterkeys(self):
        self.rr

    def test_itervalues(self):
        self.rr

    def test_k1(self):
        self.rr

    def test_k2(self):
        self.rr

    def test_keys(self):
        self.rr

    def test_load(self):
        self.rr

    def test_loadState(self):
        self.rr

    def test_makeIntegrator(self):
        self.rr

    def test_mcaSteadyState(self):
        self.rr

    def test_model(self):
        self.rr

    def test_oneStep(self):
        self.rr

    def test_options(self):
        self.rr

    def test_plot(self):
        self.rr

    def test_plotLegend(self):
        self.rr

    def test_regenerate(self):
        self.rr

    def test_removeCompartment(self):
        self.rr

    def test_removeEvent(self):
        self.rr

    def test_removeEventAssignments(self):
        self.rr

    def test_removeInitialAssignment(self):
        self.rr

    def test_removeParameter(self):
        self.rr

    def test_removeReaction(self):
        self.rr

    def test_removeRules(self):
        self.rr

    def test_removeSpecies(self):
        self.rr

    def test_reset(self):
        self.rr

    def test_resetAll(self):
        self.rr

    def test_resetParameter(self):
        self.rr

    def test_resetSelectionLists(self):
        self.rr

    def test_resetToOrigin(self):
        self.rr

    def test_saveState(self):
        self.rr

    def test_selections(self):
        self.rr

    def test_setBoundary(self):
        self.rr

    def test_setConstant(self):
        self.rr

    def test_setDiffStepSize(self):
        self.rr

    def test_setGlobalParameterByName(self):
        self.rr

    def test_setHasOnlySubstanceUnits(self):
        self.rr

    def test_setInitAmount(self):
        self.rr

    def test_setInitConcentration(self):
        self.rr

    def test_setIntegrator(self):
        self.rr

    def test_setIntegratorSetting(self):
        self.rr

    def test_setKineticLaw(self):
        self.rr

    def test_setOptions(self):
        self.rr

    def test_setPersistent(self):
        self.rr

    def test_setReversible(self):
        self.rr

    def test_setSteadyStateSolver(self):
        self.rr

    def test_setSteadyStateThreshold(self):
        self.rr

    def test_setTriggerInitialValue(self):
        self.rr

    def test_setValue(self):
        self.rr

    def test_setValues(self):
        self.rr

    def test_showPlot(self):
        self.rr

    def test_simulate(self):
        self.rr

    def test_steadyState(self):
        self.rr

    def test_steadyStateNamedArray(self):
        self.rr

    def test_steadyStateSelections(self):
        self.rr

    def test_steadyStateSolver(self):
        self.rr

    def test_steadyStateSolverExists(self):
        self.rr

    def test_steadyStateThresh(self):
        self.rr

    def test_this(self):
        self.rr

    def test_thisown(self):
        self.rr

    def test_timeCourseSelections(self):
        self.rr

    def test_validateCurrentSBML(self):
        self.rr

    def test_values(self):
        self.rr




#
#
#





