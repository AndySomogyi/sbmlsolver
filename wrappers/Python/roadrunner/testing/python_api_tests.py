"""Unit test style tests for roadrunner Python API

The integration tests are adequately handled by tester.py,
though it's a shame unittest is not used. These tests are
not about simulation accuracy, but rather checking that each
little part of the API works in isolation.

"""

import sys
import unittest

sys.path += [
    r"D:\roadrunner\roadrunner\install-msvc2019-rel-swig3.0.0\site-packages",
    # r"D:\roadrunner\roadrunner\install-msvc2019-rel-swig-4.0.2\site-packages",
]

from roadrunner.roadrunner import RoadRunner

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


class RoadRunnerTests(unittest.TestCase):
    maxDiff = None

    def setUp(self) -> None:
        self.rr = RoadRunner(sbml)

    def tearDown(self) -> None:
        pass

    def checkMatricesEqual(self, expected, actual):
        if len(expected) != len(actual):
            raise ValueError("len(expected) != len(actual)")

        if len(expected[0]) != len(actual[0]):
            raise ValueError("len(expected[0]) != len(actual[0])")

        for i in range(len(expected)):
            for j in range(len(expected[0])):
                self.assertAlmostEqual(expected[i][j], actual[i][j])

    def test_property_list(self):
        expected_property_list = ['S1', 'S1_amt', 'S1_conc', 'S2', 'S2_amt', 'S2_conc', 'S3', 'S3_amt', 'S3_conc',
                                  '_J0', '_J1', '_RoadRunner__simulateOptions', 'conservedMoietyAnalysis',
                                  'default_compartment', 'diffstep', 'integrator', 'k1', 'k2', 'model', 'options',
                                  'selections', 'steadyStateSelections', 'steadyStateSolver', 'steadyStateThresh',
                                  'timeCourseSelections']
        actual_property_list = [i for i in dir(RoadRunner) if isinstance(getattr(RoadRunner, i), property)]
        self.assertListEqual(expected_property_list, actual_property_list)

        def test_species_attribute(self):
            self.assertEqual(self.rr.S1, 10)

        def test_species_attribute_amount(self):
            self.assertEqual(self.rr.S1_amt, 10)

        def test_species_attribute_conc(self):
            self.assertEqual(self.rr.S1_conc, 10)

        def test_reaction_flux(self):
            self.assertEqual(self.rr._J0, 1)

        def test___class__(self):
            """
            Warning: brittle test that will break as soon
            as we update something in roadruner. Useful for
            updating swig though. If this test has broken,
            your build, just delete it.
            """
            class_contents = ['S1', 'S1_amt', 'S1_conc', 'S2', 'S2_amt', 'S2_conc', 'S3', 'S3_amt', 'S3_conc', '_J0',
                              '_J1',
                              '_RoadRunner__simulateOld', '_RoadRunner__simulateOptions', '__class__', '__del__',
                              '__delattr__', '__dict__', '__dir__', '__doc__', '__eq__', '__format__', '__ge__',
                              '__getattr__', '__getattribute__', '__getitem__', '__gt__', '__hash__', '__init__',
                              '__init_subclass__', '__le__', '__len__', '__lt__', '__module__', '__ne__', '__new__',
                              '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__setitem__', '__sizeof__',
                              '__str__', '__subclasshook__', '__swig_destroy__', '__swig_getmethods__',
                              '__swig_setmethods__', '__weakref__', '_addCompartment', '_addParameter', '_addReaction',
                              '_addSpecies', '_diffstep_getter', '_diffstep_stter', '_getConservedMoietyAnalysis',
                              '_getDependentFloatingSpeciesIds', '_getIndependentFloatingSpeciesIds', '_getModel',
                              '_getSelections', '_getSteadyStateSelections', '_getValue', '_load', '_makeProperties',
                              '_new_init', '_properties', '_setConservedMoietyAnalysis',
                              '_setConservedMoietyAnalysisProxy',
                              '_setSelections', '_setSteadyStateSelections', '_simulate', '_steadyStateThresh_getter',
                              '_steadyStateThresh_setter', '_swig_init', 'addAssignmentRule', 'addCompartment',
                              'addDelay',
                              'addEvent', 'addEventAssignment', 'addInitialAssignment', 'addParameter', 'addPriority',
                              'addRateRule', 'addReaction', 'addSpecies', 'addTrigger', 'clearModel',
                              'conservedMoietyAnalysis', 'createSelection', 'default_compartment', 'diffstep',
                              'ensureSolversRegistered', 'getAvailableIntegrators',
                              'getBoundarySpeciesAmountsNamedArray',
                              'getBoundarySpeciesConcentrationsNamedArray', 'getCC', 'getCompiler',
                              'getConservationMatrix',
                              'getConservedMoietyIds', 'getConservedMoietyValues', 'getCurrentSBML',
                              'getDependentFloatingSpeciesIds', 'getDependentRatesOfChange',
                              'getDependentRatesOfChangeNamedArray', 'getDiffStepSize', 'getEE', 'getEigenValueIds',
                              'getExistingIntegratorNames', 'getExtendedStoichiometryMatrix', 'getExtendedVersionInfo',
                              'getFloatingSpeciesAmountsNamedArray', 'getFloatingSpeciesConcentrationsNamedArray',
                              'getFloatingSpeciesInitialConcentrationIds', 'getFrequencyResponse', 'getFullEigenValues',
                              'getFullJacobian', 'getFullStoichiometryMatrix', 'getGlobalParameterByName', 'getIds',
                              'getIndependentFloatingSpeciesIds', 'getIndependentRatesOfChange',
                              'getIndependentRatesOfChangeNamedArray', 'getInfo', 'getInstanceCount', 'getInstanceID',
                              'getIntegrator', 'getIntegratorByName', 'getKMatrix', 'getKineticLaw', 'getL0Matrix',
                              'getLinkMatrix', 'getModel', 'getNrMatrix', 'getParamPromotedSBML', 'getRatesOfChange',
                              'getRatesOfChangeNamedArray', 'getReactionRates', 'getReducedEigenValues',
                              'getReducedJacobian', 'getReducedStoichiometryMatrix', 'getRegisteredIntegratorNames',
                              'getRegisteredSteadyStateSolverNames', 'getSBML',
                              'getScaledConcentrationControlCoefficientMatrix', 'getScaledElasticityMatrix',
                              'getScaledFloatingSpeciesElasticity', 'getScaledFluxControlCoefficientMatrix',
                              'getSelectedValues', 'getSimulationData', 'getSteadyStateSelectionStrings',
                              'getSteadyStateSolver', 'getSteadyStateThreshold', 'getSteadyStateValues',
                              'getSteadyStateValuesNamedArray', 'getSupportedIdTypes',
                              'getUnscaledConcentrationControlCoefficientMatrix', 'getUnscaledElasticityMatrix',
                              'getUnscaledFluxControlCoefficientMatrix', 'getUnscaledParameterElasticity',
                              'getUnscaledSpeciesElasticity', 'getValue', 'getuCC', 'getuEE', 'integrator',
                              'integratorExists', 'internalOneStep', 'isModelLoaded', 'items', 'iteritems', 'iterkeys',
                              'itervalues', 'k1', 'k2', 'keys', 'load', 'loadState', 'makeIntegrator', 'mcaSteadyState',
                              'model', 'oneStep', 'options', 'plot', 'plotLegend', 'regenerate', 'removeCompartment',
                              'removeEvent', 'removeEventAssignments', 'removeInitialAssignment', 'removeParameter',
                              'removeReaction', 'removeRules', 'removeSpecies', 'reset', 'resetAll', 'resetParameter',
                              'resetSelectionLists', 'resetToOrigin', 'saveState', 'selections', 'setBoundary',
                              'setConstant', 'setDiffStepSize', 'setGlobalParameterByName', 'setHasOnlySubstanceUnits',
                              'setInitAmount', 'setInitConcentration', 'setIntegrator', 'setIntegratorSetting',
                              'setKineticLaw', 'setOptions', 'setPersistent', 'setReversible', 'setSteadyStateSolver',
                              'setSteadyStateThreshold', 'setTriggerInitialValue', 'setValue', 'setValues', 'showPlot',
                              'simulate', 'steadyState', 'steadyStateNamedArray', 'steadyStateSelections',
                              'steadyStateSolver', 'steadyStateSolverExists', 'steadyStateThresh',
                              'timeCourseSelections',
                              'validateCurrentSBML', 'values']
            self.assertListEqual(class_contents, dir(self.rr.__class__))

        def test_attribute_access_protocols_setattr(self):
            self.rr.__setattr__("cheese", 6)
            self.assertTrue(hasattr(self.rr, "cheese"))
            self.assertEqual(getattr(self.rr, "cheese"), 6)

        def test_attribute_access_protocols_getattr(self):
            self.assertEqual(self.rr.__getattr__("S1"), 10)

        def test___dict__(self):
            self.assertEqual(["this"], list(self.rr.__dict__.keys()))

        def test___dir__(self):
            """
            Warning: brittle test that will break as soon
            as we update something in roadruner. Useful for
            updating swig though. If this test has broken,
            your build, just delete it.
            """
            rr_dir = ['S1', 'S1_amt', 'S1_conc', 'S2', 'S2_amt', 'S2_conc', 'S3', 'S3_amt', 'S3_conc', '_J0', '_J1',
                      '_RoadRunner__simulateOld', '_RoadRunner__simulateOptions', '__class__', '__del__', '__delattr__',
                      '__dict__', '__dir__', '__doc__', '__eq__', '__format__', '__ge__', '__getattr__',
                      '__getattribute__',
                      '__getitem__', '__gt__', '__hash__', '__init__', '__init_subclass__', '__le__', '__len__',
                      '__lt__',
                      '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__',
                      '__setitem__', '__sizeof__', '__str__', '__subclasshook__', '__swig_destroy__',
                      '__swig_getmethods__',
                      '__swig_setmethods__', '__weakref__', '_addCompartment', '_addParameter', '_addReaction',
                      '_addSpecies', '_diffstep_getter', '_diffstep_stter', '_getConservedMoietyAnalysis',
                      '_getDependentFloatingSpeciesIds', '_getIndependentFloatingSpeciesIds', '_getModel',
                      '_getSelections',
                      '_getSteadyStateSelections', '_getValue', '_load', '_makeProperties', '_new_init', '_properties',
                      '_setConservedMoietyAnalysis', '_setConservedMoietyAnalysisProxy', '_setSelections',
                      '_setSteadyStateSelections', '_simulate', '_steadyStateThresh_getter',
                      '_steadyStateThresh_setter',
                      '_swig_init', 'addAssignmentRule', 'addCompartment', 'addDelay', 'addEvent', 'addEventAssignment',
                      'addInitialAssignment', 'addParameter', 'addPriority', 'addRateRule', 'addReaction', 'addSpecies',
                      'addTrigger', 'clearModel', 'conservedMoietyAnalysis', 'createSelection', 'default_compartment',
                      'diffstep', 'ensureSolversRegistered', 'getAvailableIntegrators',
                      'getBoundarySpeciesAmountsNamedArray', 'getBoundarySpeciesConcentrationsNamedArray', 'getCC',
                      'getCompiler', 'getConservationMatrix', 'getConservedMoietyIds', 'getConservedMoietyValues',
                      'getCurrentSBML', 'getDependentFloatingSpeciesIds', 'getDependentRatesOfChange',
                      'getDependentRatesOfChangeNamedArray', 'getDiffStepSize', 'getEE', 'getEigenValueIds',
                      'getExistingIntegratorNames', 'getExtendedStoichiometryMatrix', 'getExtendedVersionInfo',
                      'getFloatingSpeciesAmountsNamedArray', 'getFloatingSpeciesConcentrationsNamedArray',
                      'getFloatingSpeciesInitialConcentrationIds', 'getFrequencyResponse', 'getFullEigenValues',
                      'getFullJacobian', 'getFullStoichiometryMatrix', 'getGlobalParameterByName', 'getIds',
                      'getIndependentFloatingSpeciesIds', 'getIndependentRatesOfChange',
                      'getIndependentRatesOfChangeNamedArray', 'getInfo', 'getInstanceCount', 'getInstanceID',
                      'getIntegrator', 'getIntegratorByName', 'getKMatrix', 'getKineticLaw', 'getL0Matrix',
                      'getLinkMatrix',
                      'getModel', 'getNrMatrix', 'getParamPromotedSBML', 'getRatesOfChange',
                      'getRatesOfChangeNamedArray',
                      'getReactionRates', 'getReducedEigenValues', 'getReducedJacobian',
                      'getReducedStoichiometryMatrix',
                      'getRegisteredIntegratorNames', 'getRegisteredSteadyStateSolverNames', 'getSBML',
                      'getScaledConcentrationControlCoefficientMatrix', 'getScaledElasticityMatrix',
                      'getScaledFloatingSpeciesElasticity', 'getScaledFluxControlCoefficientMatrix',
                      'getSelectedValues',
                      'getSimulationData', 'getSteadyStateSelectionStrings', 'getSteadyStateSolver',
                      'getSteadyStateThreshold', 'getSteadyStateValues', 'getSteadyStateValuesNamedArray',
                      'getSupportedIdTypes', 'getUnscaledConcentrationControlCoefficientMatrix',
                      'getUnscaledElasticityMatrix', 'getUnscaledFluxControlCoefficientMatrix',
                      'getUnscaledParameterElasticity', 'getUnscaledSpeciesElasticity', 'getValue', 'getuCC', 'getuEE',
                      'integrator', 'integratorExists', 'internalOneStep', 'isModelLoaded', 'items', 'iteritems',
                      'iterkeys', 'itervalues', 'k1', 'k2', 'keys', 'load', 'loadState', 'makeIntegrator',
                      'mcaSteadyState',
                      'model', 'oneStep', 'options', 'plot', 'plotLegend', 'regenerate', 'removeCompartment',
                      'removeEvent',
                      'removeEventAssignments', 'removeInitialAssignment', 'removeParameter', 'removeReaction',
                      'removeRules', 'removeSpecies', 'reset', 'resetAll', 'resetParameter', 'resetSelectionLists',
                      'resetToOrigin', 'saveState', 'selections', 'setBoundary', 'setConstant', 'setDiffStepSize',
                      'setGlobalParameterByName', 'setHasOnlySubstanceUnits', 'setInitAmount', 'setInitConcentration',
                      'setIntegrator', 'setIntegratorSetting', 'setKineticLaw', 'setOptions', 'setPersistent',
                      'setReversible', 'setSteadyStateSolver', 'setSteadyStateThreshold', 'setTriggerInitialValue',
                      'setValue', 'setValues', 'showPlot', 'simulate', 'steadyState', 'steadyStateNamedArray',
                      'steadyStateSelections', 'steadyStateSolver', 'steadyStateSolverExists', 'steadyStateThresh',
                      'this',
                      'timeCourseSelections', 'validateCurrentSBML', 'values']
            self.assertEqual(rr_dir, dir(self.rr))

        @unittest.skip("__eq__ may be implemented, may not. If not we should do")
        def test___eq__(self):
            pass

        def test___getitem__(self):
            self.assertEqual(self.rr.__getitem__("S1"), 10)

        def test___len__(self):
            self.assertEqual(len(self.rr), 17)  # not sure what this is enumerating though. Species, reactions?

        def test___repr__(self):
            l = repr([self.rr])
            self.assertIn("roadrunner.RoadRunner()", l)

        def test___setitem__failes_when_key_not_exist(self):
            with self.assertRaises(RuntimeError):
                self.rr.__setitem__("five", 5)

        def test___setitem__failes_when_key_exists(self):
            self.rr.__setitem__("S1", 12)
            self.assertEqual(self.rr.__getitem__("S1"), 12)

        def test___sizeof__(self):
            self.assertEqual(32, self.rr.__sizeof__())

        def test___str__(self):
            s = str(self.rr)
            self.assertIn("roadrunner", s)

        @unittest.skip("Method not implemented yet (but should be)")
        def test_getCompartmentIds(self):
            """This method is missing from the rr api"""
            pass

        def testAddCompartment(self):
            """The rr._addCompartment() method is called by the
            rr.addCompartment() method. The difference, is the
            latter adds the compartment to the __dict__ as an
            attribute.
            """
            self.rr._addCompartment("NewComp", 1, True)
            with self.assertRaises(AttributeError):
                self.rr.C1

        def test_addCompartment(self):
            """The rr._addCompartment() method is called by the
            rr.addCompartment() method. The difference, is the
            latter adds the compartment to the __dict__ as an
            attribute.
            """
            self.rr.addCompartment("C1", 1, True)
            self.assertEqual(self.rr.C1, 1)

        # def test_addParameter(self):
        #     self.rr._addParameter("k_new", 25, True)
        #     with self.assertRaises(AttributeError):
        #         print(self.rr.k_new)
        #
        # def testAddParameter(self):
        #     self.rr.addParameter("k_new", 25, True)
        #     self.assertEqual(25, self.rr.k_new)
        #
        # def test_addReaction(self):
        #     self.rr._addReaction("NewReaction", ["S1"], ["S2"], "k1*S1", True)
        #     with self.assertRaises(AttributeError):
        #         print(self.rr.NewReaction)
        #
        # def testAddReaction(self):
        #     self.rr.addReaction("NewReaction", ["S1"], ["S2"], "k1*S1", True)
        #     self.assertEqual(self.rr.NewReaction, 1.0)
        #
        # def test_addSpecies(self):
        #     self.rr._addSpecies("NewSpecies", "default_compartment", 123.3, False, False, "", True)
        #     with self.assertRaises(AttributeError):
        #         print(self.rr.NewSpecies)
        #
        # def testAddSpecies(self):
        #     self.rr.addSpecies("NewSpecies", "default_compartment", 123.3, False, False, "", True)
        #     self.assertEqual(self.rr.NewSpecies, 123.3)
        #
        # @unittest.skip
        # def test_removeInitialAssignment(self):
        #     print(self.rr.removeInitialAssignment())
        #     self.assertEqual(
        #         self.rr.removeInitialAssignment(),
        #     )
        #
        # @unittest.skip
        # def test_removeParameter(self):
        #     print(self.rr.removeParameter())
        #     self.assertEqual(
        #         self.rr.removeParameter(), )
        #
        # @unittest.skip
        # def test_removeReaction(self):
        #     print(self.rr.removeReaction())
        #     self.assertEqual(
        #         self.rr.removeReaction(),
        #     )
        #
        # @unittest.skip
        # def test_removeRules(self):
        #     print(self.rr.removeRules())
        #     self.assertEqual(
        #         self.rr.removeRules(),
        #
        #     )
        #
        # @unittest.skip
        # def test_removeSpecies(self):
        #     print(self.rr.removeSpecies())
        #     self.assertEqual(
        #         self.rr.removeSpecies(),
        #
        #     )
        #
        # def test_diffstep_getter(self):
        #     self.assertEqual(self.rr._diffstep_getter(), 0.05)
        #
        # def test_diffstep_stter(self):
        #     self.rr._diffstep_stter(0.01)
        #     self.assertEqual(self.rr._diffstep_getter(), 0.01)
        #
        # def test_getConservedMoietyAnalysis(self):
        #     self.assertFalse(self.rr._getConservedMoietyAnalysis())
        #
        # def test_setConservedMoietyAnalysis(self):
        #     self.rr.conservedMoietyAnalysis = True
        #     self.assertTrue(self.rr._getConservedMoietyAnalysis())
        #
        # def test_addAssignmentRule(self):
        #     self.rr.addSpecies("SNew", "default_compartment", 2.5, False, True, "", True)
        #     self.rr.addAssignmentRule("SNew", "S1+5", True)
        #     self.assertEqual(self.rr.SNew, 15)
        #
        # @unittest.skip("Not sure how to test this right now")
        # def test_addDelay(self):
        #     pass
        #
        # @unittest.skip("Not sure how to test this right now")
        # def test_addEvent(self):
        #     pass
        #
        # @unittest.skip("Not sure how to test this right now")
        # def test_addEventAssignment(self):
        #     pass
        #
        # @unittest.skip("Not sure how to test this right now")
        # def test_addInitialAssignment(self):
        #     self.rr.addSpecies("NewSpec", "default_compartment", 13.3, False, True, "", False)
        #     self.rr.addInitialAssignment("NewSpec", "S1+S2", True)
        #
        # @unittest.skip("Not sure how to test this right now")
        # def test_addPriority(self):
        #     pass
        #
        # @unittest.skip("Not sure how to test this right now")
        # def test_addRateRule(self):
        #     pass
        #
        # @unittest.skip("Not sure how to test this right now")
        # def test_addTrigger(self):
        #     pass
        #
        # def test_clearModel(self):
        #     self.assertTrue(self.rr.clearModel())
        #
        # @unittest.skip("Not sure how to test this right now. Docs are not clear")
        # def test_createSelection(self):
        #     pass
        #
        # def test_default_compartment(self):
        #     self.assertEqual(self.rr.default_compartment, 1)
        #
        # def test_diffstep(self):
        #     self.assertEqual(self.rr.diffstep, 0.05)
        #
        # @unittest.skip("This API is unclear. ")
        # def test_ensureSolversRegistered(self):
        #     self.rr.ensureSolversRegistered()
        #
        # def test_getAvailableIntegrators(self):
        #     self.assertEqual(self.rr.getAvailableIntegrators(), ("cvode",))
        #
        # def test_getBoundarySpeciesAmountsNamedArray(self):
        #     bs = self.rr.getBoundarySpeciesAmountsNamedArray()
        #     self.assertEqual(bs["S1"], 10)
        #     self.assertEqual(bs["S3"], 0)
        #
        # def test_getBoundarySpeciesConcentrationsNamedArray(self):
        #     bs = self.rr.getBoundarySpeciesConcentrationsNamedArray()
        #     self.assertEqual(bs["S1"], 10)
        #     self.assertEqual(bs["S3"], 0)
        #
        # def test_getCC(self):
        #     self.assertAlmostEqual(self.rr.getCC("_J0", "k1"), 1.0000000000000024)
        #
        # def test_getCompiler(self):
        #     compiler_string = str(self.rr.getCompiler())
        #     self.assertIn("Compiler", compiler_string)
        #
        # def test_getConservationMatrix(self):
        #     """This mode has no conserved moieties so poor model for testing this feature"""
        #     self.rr.conservedMoietyAnalysis = True
        #     self.assertEqual(self.rr.getConservationMatrix().size, 0)
        #
        # def test_getConservedMoietyIds(self):
        #     """This mode has no conserved moieties so poor model for testing this feature"""
        #     self.rr.conservedMoietyAnalysis = True
        #     self.assertEqual(self.rr.getConservedMoietyIds(), ())
        #
        # def test_getConservedMoietyValues(self):
        #     """This mode has no conserved moieties so poor model for testing this feature"""
        #     self.rr.conservedMoietyAnalysis = True
        #     self.assertEqual(len(self.rr.getConservedMoietyValues()), 0)
        #
        # def test_getCurrentSBML(self):
        #     self.assertIsInstance(self.rr.getCurrentSBML(), str)
        #
        # def test_getDependentFloatingSpeciesIds(self):
        #     """No dependents in this model"""
        #     self.assertTrue(callable(self.rr.getDependentFloatingSpeciesIds))
        #
        # def test_getDependentRatesOfChange(self):
        #     """No dependents in this model so we'll just check callable"""
        #     self.assertTrue(callable(self.rr.getDependentRatesOfChange))
        #
        # def test_getDependentRatesOfChangeNamedArray(self):
        #     """No dependents in this model"""
        #     self.assertTrue(callable(self.rr.getDependentRatesOfChangeNamedArray))
        #
        # def test_getDiffStepSize(self):
        #     self.assertEqual(0.05, self.rr.getDiffStepSize())
        #
        # def test_getEE(self):
        #     self.assertAlmostEqual(self.rr.getEE("_J0", "k1"), 1.0000000000000024)
        #
        # def test_getEigenValueIds(self):
        #     expected = ('eigen(S2)', 'eigenReal(S2)', 'eigenImag(S2)')
        #     self.assertEqual(self.rr.getEigenValueIds(), expected)
        #
        # def test_getExistingIntegratorNames(self):
        #     expected = ('cvode',)
        #     self.assertEqual(expected, self.rr.getExistingIntegratorNames())
        #
        # def test_getExtendedStoichiometryMatrix(self):
        #     expected = [[1, -1],
        #                 [-1, 0],
        #                 [0, 1]]
        #     # todo implement proper equality operators
        #     for i in range(len(expected)):
        #         for j in range(len(expected[0])):
        #             self.assertEqual(expected[i][j], self.rr.getExtendedStoichiometryMatrix()[i][j])
        #
        # def test_getFloatingSpeciesAmountsNamedArray(self):
        #     x = self.rr.getFloatingSpeciesAmountsNamedArray()
        #     self.assertEqual(x["S2"], 0)
        #
        # def test_getFloatingSpeciesConcentrationsNamedArray(self):
        #     x = self.rr.getFloatingSpeciesConcentrationsNamedArray()
        #     self.assertEqual(x["S2"], 0)
        #
        # def test_getFloatingSpeciesInitialConcentrationIds(self):
        #     x = self.rr.getFloatingSpeciesInitialConcentrationIds()
        #     self.assertEqual(x, ("init([S2])",))
        #
        # @unittest.skip("figure out how to use")
        # def test_getFrequencyResponse(self):
        #     pass
        #
        # def test_getFullEigenValues(self):
        #     self.assertEqual([-0.2], self.rr.getFullEigenValues())
        #
        # def test_getFullJacobian(self):
        #     self.assertEqual(-0.2, self.rr.getFullJacobian()["S2"])
        #
        # def test_getFullStoichiometryMatrix(self):
        #     expected = [[1, -1]]
        #     self.checkMatricesEqual(expected, self.rr.getFullStoichiometryMatrix())
        #
        # def test_getGlobalParameterByName(self):
        #     self.assertEqual(self.rr.getGlobalParameterByName("k1"), 0.1)
        #
        # @unittest.skip("No way to use this method from Python - it "
        #                "requires an out parameter as input")
        # def test_getIds(self):
        #     pass
        #
        # def test_getIndependentFloatingSpeciesIds(self):
        #     self.assertEqual(self.rr.getIndependentFloatingSpeciesIds(), ["S2"])
        #
        # def test_getIndependentRatesOfChange(self):
        #     self.assertEqual(1, self.rr.getIndependentRatesOfChange())
        #
        # def test_getIndependentRatesOfChangeNamedArray(self):
        #     print(self.rr.getIndependentRatesOfChangeNamedArray())
        #     self.assertEqual(
        #         self.rr.getIndependentRatesOfChangeNamedArray(),
        #         [[1]]
        #     )
        #
        # def test_getInfo(self):
        #     print(self.rr.getInfo())
        #     self.assertIn(
        #         "jacobianStepSize",
        #         self.rr.getInfo(),
        #     )
        #
        # def test_getIntegrator(self):
        #     print(self.rr.getIntegrator())
        #     self.assertIsInstance(
        #         self.rr.getIntegrator(),
        #         roadrunner.roadrunner.Integrator
        #     )
        #
        # def test_getIntegratorByName(self):
        #     print(self.rr.getIntegratorByName("cvode"))
        #     self.assertIsInstance(
        #         self.rr.getIntegratorByName("cvode"),
        #         roadrunner.roadrunner.Integrator
        #     )
        #
        # def test_getKMatrix(self):
        #     expected = [[1], [1]]
        #     print(self.rr.getKMatrix())
        #     self.checkMatricesEqual(
        #         expected,
        #         self.rr.getKMatrix(),
        #     )
        #
        # def test_getKineticLaw(self):
        #     print(self.rr.getKineticLaw("_J0"))
        #     self.assertEqual(
        #         self.rr.getKineticLaw("_J0"),
        #         "k1 * S1"
        #     )
        #
        # def test_getL0Matrix(self):
        #     print(self.rr.getL0Matrix())
        #     self.assertEqual(
        #         self.rr.getL0Matrix().size, 0
        #     )
        #
        # def test_getLinkMatrix(self):
        #     print(self.rr.getLinkMatrix())
        #     expected = [[1]]
        #     self.checkMatricesEqual(
        #         expected,
        #         self.rr.getLinkMatrix(),
        #     )
        #
        # def test_getModel(self):
        #     print(self.rr.getModel())
        #     self.assertIsInstance(
        #         self.rr.getModel(),
        #         roadrunner.roadrunner.ExecutableModel
        #     )
        #
        # def test_getNrMatrix(self):
        #     print(self.rr.getNrMatrix())
        #     expected = [[1, -1]]
        #     self.checkMatricesEqual(
        #         expected,
        #         self.rr.getNrMatrix(),
        #     )
        #
        # @unittest.skip("No idea what the argument for this method should be?")
        # def test_getParamPromotedSBML(self):
        #     print(self.rr.getParamPromotedSBML())
        #     self.assertEqual(
        #         self.rr.getParamPromotedSBML(),
        #
        #     )
        #
        # def test_getRatesOfChange(self):
        #     print(self.rr.getRatesOfChange())
        #     self.assertEqual(
        #         self.rr.getRatesOfChange(),
        #         [1.0]
        #     )
        #
        # def test_getRatesOfChangeNamedArray(self):
        #     print(self.rr.getRatesOfChangeNamedArray())
        #     self.assertEqual(
        #         self.rr.getRatesOfChangeNamedArray(),
        #         [[1.0]]
        #     )
        #
        # def test_getReactionRates(self):
        #     print(self.rr.getReactionRates())
        #     self.assertEqual(1.0, self.rr.getReactionRates()[0])
        #     self.assertEqual(0.0, self.rr.getReactionRates()[1])
        #
        # def test_getReducedEigenValues(self):
        #     print(self.rr.getReducedEigenValues())
        #     self.assertAlmostEqual(
        #         self.rr.getReducedEigenValues()[0],
        #         -0.2
        #     )
        #
        # def test_getReducedJacobian(self):
        #     print(self.rr.getReducedJacobian())
        #     self.assertAlmostEqual(
        #         -0.2,
        #         self.rr.getReducedJacobian()["S2"][0],
        #     )
        #
        # def test_getReducedStoichiometryMatrix(self):
        #     print(self.rr.getReducedStoichiometryMatrix())
        #     expected = [[1, -1]]
        #     self.checkMatricesEqual(
        #         expected,
        #         self.rr.getReducedStoichiometryMatrix(),
        #     )
        #
        # def test_getRegisteredIntegratorNames(self):
        #     print(self.rr.getRegisteredIntegratorNames())
        #     self.assertEqual(
        #         ('cvode', 'gillespie', 'rk4', 'rk45', 'euler'),
        #         self.rr.getRegisteredIntegratorNames(),
        #     )
        #
        # def test_getRegisteredSteadyStateSolverNames(self):
        #     print(self.rr.getRegisteredSteadyStateSolverNames())
        #     self.assertEqual(
        #         ('nleq1', 'nleq2', 'NewtonIteration'),
        #         self.rr.getRegisteredSteadyStateSolverNames(),
        #     )
        #
        # def test_getSBML(self):
        #     """todo what is the difference between getSBML and getCurrentSBML?"""
        #     print(self.rr.getSBML())
        #     self.assertIsInstance(
        #         self.rr.getSBML(),
        #         str
        #     )
        #
        # def test_getScaledConcentrationControlCoefficientMatrix(self):
        #     print(self.rr.getScaledConcentrationControlCoefficientMatrix())
        #     expected = [[1, -1]]
        #     self.checkMatricesEqual(
        #         expected,
        #         self.rr.getScaledConcentrationControlCoefficientMatrix(),
        #     )
        #
        # def test_getScaledElasticityMatrix(self):
        #     print(self.rr.getScaledElasticityMatrix())
        #     self.assertEqual(
        #         0,
        #         self.rr.getScaledElasticityMatrix()[0],
        #     )
        #
        # def test_getScaledFloatingSpeciesElasticity(self):
        #     print(self.rr.getScaledFloatingSpeciesElasticity("_J0", "S2"))
        #     self.assertEqual(
        #         0.0,
        #         self.rr.getScaledFloatingSpeciesElasticity("_J0", "S2"),
        #     )
        #
        # def test_getScaledFluxControlCoefficientMatrix(self):
        #     print(self.rr.getScaledFluxControlCoefficientMatrix())
        #     expected = [[1, 0],
        #                 [1, -2.88658e-15]]
        #     self.checkMatricesEqual(
        #         expected,
        #         self.rr.getScaledFluxControlCoefficientMatrix(),
        #     )
        #
        # def test_getSelectedValues(self):
        #     print(self.rr.getSelectedValues())
        #     self.assertEqual(self.rr.getSelectedValues()[0], 0)
        #     self.assertEqual(self.rr.getSelectedValues()[1], 0)
        #
        # def test_getSimulationData(self):
        #     print(self.rr.getSimulationData())
        #     self.assertTrue(
        #         self.rr.getSimulationData().size == 0
        #     )
        #
        # def test_getSteadyStateSelectionStrings(self):
        #     print(self.rr.getSteadyStateSelectionStrings())
        #     self.assertEqual(
        #         self.rr.getSteadyStateSelectionStrings(),
        #         ("[S2]",)
        #     )
        #
        # def test_getSteadyStateSolver(self):
        #     print(self.rr.getSteadyStateSolver())
        #     self.assertIsInstance(
        #         self.rr.getSteadyStateSolver(),
        #         roadrunner.roadrunner.SteadyStateSolver
        #     )
        #
        # def test_getSteadyStateThreshold(self):
        #     print(self.rr.getSteadyStateThreshold())
        #     self.assertEqual(
        #         self.rr.getSteadyStateThreshold(),
        #         0.01
        #     )
        #
        # def test_getSteadyStateValues(self):
        #     print(self.rr.getSteadyStateValues())
        #     self.assertEqual(
        #         self.rr.getSteadyStateValues(),
        #         [5.0]
        #     )
        #
        # def test_getSteadyStateValuesNamedArray(self):
        #     print(self.rr.getSteadyStateValuesNamedArray())
        #     self.assertEqual(
        #         self.rr.getSteadyStateValuesNamedArray()[0],
        #         [5.0]
        #     )
        #
        # @unittest.skip("This method should be not be visible in Python")
        # def test_getSupportedIdTypes(self):
        #     """returns a bit field of the ids that this class supports."""
        #     print(self.rr.getSupportedIdTypes())
        #
        # def test_getUnscaledConcentrationControlCoefficientMatrix(self):
        #     print(self.rr.getUnscaledConcentrationControlCoefficientMatrix())
        #     expected = [[5, -5]]
        #     self.checkMatricesEqual(
        #         expected,
        #         self.rr.getUnscaledConcentrationControlCoefficientMatrix(),
        #     )
        #
        # def test_getUnscaledElasticityMatrix(self):
        #     print(self.rr.getUnscaledElasticityMatrix())
        #     expected = [[0],
        #                 [0.2]]
        #     self.checkMatricesEqual(
        #         expected,
        #         self.rr.getUnscaledElasticityMatrix(),
        #     )
        #
        # def test_getUnscaledFluxControlCoefficientMatrix(self):
        #     print(self.rr.getUnscaledFluxControlCoefficientMatrix())
        #     expected = [[1, 0],
        #                 [1, -2.88658e-15]]
        #     self.checkMatricesEqual(
        #         expected,
        #         self.rr.getUnscaledFluxControlCoefficientMatrix(),
        #     )
        #
        # def test_getUnscaledParameterElasticity(self):
        #     print(self.rr.getUnscaledParameterElasticity("_J0", "k1"))
        #     self.assertAlmostEqual(
        #         10,
        #         self.rr.getUnscaledParameterElasticity("_J0", "k1"),
        #     )
        #
        # def test_getUnscaledSpeciesElasticity(self):
        #     print(self.rr.getUnscaledSpeciesElasticity(0, 0))
        #     self.assertEqual(
        #         0.0,
        #         self.rr.getUnscaledSpeciesElasticity(0, 0),
        #
        #     )
        #
        # def test_getValue(self):
        #     print(self.rr.getValue("k1"))
        #     self.assertEqual(
        #         self.rr.getValue("k1"),
        #         0.1
        #     )
        #
        # def test_getuCC(self):
        #     print(self.rr.getuCC("_J0", "k1"))
        #     self.assertAlmostEqual(
        #         self.rr.getuCC("_J0", "k1"),
        #         10
        #     )
        #
        # def test_getuEE(self):
        #     print(self.rr.getuEE("_J0", "k1"))
        #     self.assertAlmostEqual(
        #         self.rr.getuEE("_J0", "k1"),
        #         10
        #     )
        #
        # def test_integrator(self):
        #     print(self.rr.integrator)
        #     self.assertIsInstance(
        #         self.rr.integrator,
        #         roadrunner.roadrunner.Integrator
        #     )
        #
        # def test_integratorExists(self):
        #     self.assertTrue(
        #         self.rr.integratorExists("cvode"),
        #     )
        #
        # def test_internalOneStep(self):
        #     before = self.rr.S2
        #     self.rr.internalOneStep(0, 10)
        #     after = self.rr.S2
        #     self.assertNotEqual(before, after)
        #
        # def test_isModelLoaded(self):
        #     print(self.rr.isModelLoaded())
        #     self.assertTrue(
        #         self.rr.isModelLoaded(),
        #     )
        #
        # def test_items(self):
        #     print(self.rr.items())
        #     expected = [('S2', 0.0), ('S1', 10.0), ('S3', 0.0),
        #                 ('[S2]', 0.0), ('[S1]', 10.0), ('[S3]', 0.0),
        #                 ('default_compartment', 1.0), ('k1', 0.1),
        #                 ('k2', 0.2), ('_J0', 1.0), ('_J1', 0.0),
        #                 ('init([S2])', 0.0), ('init(S2)', 0.0),
        #                 ("S2'", 1.0), ('eigen(S2)', (-0.2 + 0j)),
        #                 ('eigenReal(S2)', -0.2), ('eigenImag(S2)', 0.0)]
        #     self.assertEqual(
        #         expected,
        #         self.rr.items(),
        #     )
        #
        # def test_iteritems(self):
        #     print(self.rr.iteritems())
        #     working_iterator = False
        #     for i in self.rr.iteritems():
        #         working_iterator = True
        #     self.assertTrue(working_iterator)
        #
        # def test_iterkeys(self):
        #     print(self.rr.iterkeys())
        #     working_iterator = False
        #     for i in self.rr.iterkeys():
        #         working_iterator = True
        #     self.assertTrue(working_iterator)
        #
        # def test_itervalues(self):
        #     print(self.rr.itervalues())
        #     working_iterator = False
        #     for i in self.rr.itervalues():
        #         working_iterator = True
        #     self.assertTrue(working_iterator)
        #
        # def test_parameter_value(self):
        #     self.assertEqual(
        #         self.rr.k1,
        #         0.1
        #     )
        #
        # def test_keys(self):
        #     print(self.rr.keys())
        #     expected = ['S2', 'S1', 'S3', '[S2]', '[S1]', '[S3]', 'default_compartment', 'k1', 'k2', '_J0', '_J1',
        #                 'init([S2])', 'init(S2)', "S2'", 'eigen(S2)', 'eigenReal(S2)', 'eigenImag(S2)']
        #     self.assertEqual(
        #         expected,
        #         self.rr.keys(),
        #     )
        #
        # def test_load(self):
        #     print(self.rr.load(sbml))
        #     self.rr.load(sbml),
        #     self.assertTrue(self.rr.isModelLoaded())
        #
        # def test_loadState(self):
        #     this_dir = os.path.dirname(__file__)
        #     fname = os.path.join(this_dir, "roadrunner_state.rr")
        #     self.rr.saveState(fname)
        #     self.assertTrue(os.path.isfile(fname))
        #     self.rr.loadState(fname)
        #     self.assertTrue(self.rr.isModelLoaded())
        #     os.remove(fname)
        #
        # def test_makeIntegrator(self):
        #     integrator = self.rr.makeIntegrator("gillespie")
        #     self.assertIsInstance(
        #         integrator,
        #         roadrunner.roadrunner.Integrator
        #     )
        #
        # def test_mcaSteadyState(self):
        #     print(self.rr.mcaSteadyState())
        #     self.assertEqual(
        #         self.rr.mcaSteadyState(),
        #         0
        #     )
        #
        # def test_model(self):
        #     print(self.rr.model)
        #     self.assertIsInstance(
        #         self.rr.model,
        #         roadrunner.roadrunner.ExecutableModel
        #     )
        #
        # def test_oneStep(self):
        #     before = self.rr.S2
        #     self.rr.internalOneStep(0, 10)
        #     after = self.rr.S2
        #     self.assertNotEqual(before, after)
        #
        # def test_options(self):
        #     print(self.rr.options)
        #     self.assertIsInstance(
        #         self.rr.options,
        #         roadrunner.roadrunner.RoadRunnerOptions
        #     )
        #
        # def test_regenerate(self):
        #     self.rr.regenerate()
        #     self.assertTrue(self.rr.isModelLoaded())
        #
        # @unittest.skip("unclear on how to use this method")
        # def test_removeCompartment(self):
        #     print(self.rr.keys())
        #     self.rr.removeCompartment("default_compartment", True)
        #     with self.assertRaises(AttributeError):
        #         self.rr.default_compartment
        #
        # @unittest.skip("no events in model - add one ")
        # def test_removeEvent(self):
        #     print(self.rr.removeEvent())
        #
        # @unittest.skip("no events in model - add one ")
        # def test_removeEventAssignments(self):
        #     print(self.rr.removeEventAssignments())
        #
        # def test_reset(self):
        #     self.rr.reset()
        #     self.assertTrue(
        #         self.rr.isModelLoaded()
        #     )
        #
        # def test_resetAll(self):
        #     """difference between reset and resetAll?"""
        #     self.rr.resetAll()
        #     self.assertTrue(
        #         self.rr.isModelLoaded()
        #     )
        #
        # def test_resetParameter(self):
        #     self.rr.k1 = 15
        #     print(self.rr.resetParameter())
        #     self.assertEqual(
        #         self.rr.k1, 0.1
        #     )
        #
        # def test_resetSelectionLists(self):
        #     self.rr.selections = ["[S2]"]
        #     print(self.rr.resetSelectionLists())
        #     self.assertEqual(
        #         self.rr.selections,
        #         ['time', '[S2]']
        #     )
        #
        # def test_resetToOrigin(self):
        #     self.k1 = 16
        #     self.rr.resetToOrigin()
        #     self.assertEqual(
        #         self.rr.k1, 0.1
        #
        #     )
        #
        # def test_selections(self):
        #     print(self.rr.selections)
        #     self.assertEqual(
        #         self.rr.selections,
        #         ['time', '[S2]']
        #     )
        #
        # def test_setBoundary(self):
        #     self.rr.setBoundary("S1", False)
        #     self.assertEqual(1, self.rr.getBoundarySpeciesAmountsNamedArray().size)
        #     self.rr.setBoundary("S1", True)
        #     self.assertEqual(2, self.rr.getBoundarySpeciesAmountsNamedArray().size)
        #
        # @unittest.skip("We can setConstant but how to getConstant?")
        # def test_setConstant(self):
        #     self.rr.setConstant("S1", False)
        #
        # def test_setDiffStepSize(self):
        #     self.rr.setDiffStepSize(1e-6)
        #     self.assertEqual(
        #         self.rr.getDiffStepSize(),
        #         1e-6
        #     )
        #
        # def test_setGlobalParameterByName(self):
        #     self.rr.setGlobalParameterByName("k1", 6),
        #     self.assertEqual(
        #         self.rr.k1, 6
        #     )
        #
        # @unittest.skip("How to getHasOnlySubstanceUnits?")
        # def test_setHasOnlySubstanceUnits(self):
        #     self.rr.setHasOnlySubstanceUnits("S1", True)
        #
        # @unittest.skip("This function looks broken?")
        # def test_setInitAmount(self):
        #     self.rr.setInitAmount("S1", 45.6, True)
        #     self.assertEqual(
        #         self.rr.S1, 45.6
        #     )
        #
        # @unittest.skip("This function looks broken?")
        # def test_setInitConcentration(self):
        #     self.rr.setInitConcentration("S1", 45.6, True)
        #     self.assertEqual(
        #         self.rr.S1, 45.6
        #     )
        #
        # def test_setIntegrator(self):
        #     self.rr.setIntegrator("gillespie")
        #     self.assertIsInstance(
        #         self.rr.integrator,
        #         roadrunner.roadrunner.Integrator
        #     )
        #
        # def test_setIntegratorSetting(self):
        #     print(self.rr.setIntegratorSetting("cvode", "stiff", True))
        #     self.assertTrue(
        #         self.rr.integrator.getValue("stiff")
        #     )
        #
        # def test_setKineticLaw(self):
        #     self.rr.setKineticLaw("_J0", "1+2+3+4+5")
        #     self.assertEqual(
        #         self.rr.getKineticLaw("_J0"),
        #         "1 + 2 + 3 + 4 + 5"
        #     )
        #
        # @unittest.skip("What should go in RoadRunnerOptions?")
        # def test_setOptions(self):
        #     opt = roadrunner.roadrunner.RoadRunnerOptions()
        #     print(self.rr.setOptions())
        #     self.assertEqual(
        #         self.rr.setOptions(),
        #     )
        #
        # @unittest.skip("how to use?")
        # def test_setPersistent(self):
        #     print(self.rr.setPersistent())
        #     self.assertEqual(
        #         self.rr.setPersistent(),
        #
        #     )
        #
        # @unittest.skip("How to get the reversible flag back after we've set it?")
        # def test_setReversible(self):
        #     print(self.rr.setReversible("_J0", True, True))
        #
        # def test_setSteadyStateSolver(self):
        #     print(self.rr.setSteadyStateSolver("NewtonIteration"))
        #     self.assertEqual(
        #         self.rr.getSteadyStateSolver().getName(),
        #         "NewtonIteration"
        #     )
        #
        # def test_setSteadyStateThreshold(self):
        #     print(self.rr.setSteadyStateThreshold(6))
        #     self.assertEqual(
        #         self.rr.getSteadyStateThreshold(),
        #         6
        #
        #     )
        #
        # @unittest.skip("how to use?")
        # def test_setTriggerInitialValue(self):
        #     print(self.rr.setTriggerInitialValue())
        #     self.assertEqual(
        #         self.rr.setTriggerInitialValue(),
        #     )
        #
        # @unittest.skip("What values are set on a RoadRunner instance?")
        # def test_setValue(self):
        #     print(self.rr.setValue())
        #     self.assertEqual(
        #         self.rr.setValue(),
        #     )
        #
        # def test_simulate(self):
        #     before = self.rr.getFloatingSpeciesAmountsNamedArray()
        #     self.rr.simulate(0, 10, 11)
        #     after = self.rr.getFloatingSpeciesAmountsNamedArray()
        #     self.assertNotEqual(before, after)
        #
        # def test_steadyState(self):
        #     before = self.rr.getFloatingSpeciesAmountsNamedArray()
        #     print(self.rr.steadyState())
        #     after = self.rr.getFloatingSpeciesAmountsNamedArray()
        #     self.assertNotEqual(before, after)
        #
        # def test_steadyStateNamedArray(self):
        #     print(self.rr.steadyStateNamedArray())
        #     self.assertEqual(
        #         self.rr.steadyStateNamedArray(),
        #         5
        #     )
        #
        # def test_steadyStateSelections(self):
        #     print(self.rr.steadyStateSelections)
        #     self.assertEqual(
        #         self.rr.steadyStateSelections,
        #         ["[S2]"]
        #
        #     )
        #
        # def test_steadyStateSolver(self):
        #     print(self.rr.steadyStateSolver)
        #     self.assertIsInstance(
        #         self.rr.steadyStateSolver,
        #         roadrunner.roadrunner.SteadyStateSolver
        #     )
        #
        # def test_steadyStateSolverExists(self):
        #     self.assertTrue(self.rr.steadyStateSolverExists("nleq2"))
        #     self.assertFalse(self.rr.steadyStateSolverExists("NewtonIteration"))  # not instantiated yet
        #
        # def test_steadyStateThresh(self):
        #     print(self.rr.steadyStateThresh)
        #     self.assertEqual(
        #         self.rr.steadyStateThresh,
        #         0.01
        #     )
        #
        # def test_timeCourseSelections(self):
        #     print(self.rr.timeCourseSelections)
        #     self.assertEqual(
        #         self.rr.timeCourseSelections,
        #         ['time', '[S2]']
        #     )
        #
        # @unittest.skip("unclear how to test. Should this method be private?")
        # def test_validateCurrentSBML(self):
        #     self.assertTrue(self.rr.validateCurrentSBML())
