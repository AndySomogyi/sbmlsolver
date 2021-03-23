import os
import sys

thisDir = os.path.dirname(os.path.realpath(__file__))
rr_site_packages = os.path.dirname(os.path.dirname(thisDir))
print("thisDir", thisDir)
print("rr_site_packages", rr_site_packages)

sys.path += [
    rr_site_packages,
    r"D:\roadrunner\roadrunner\install-msvc2019-rel-swig-4.0.2\site-packages",
]
from roadrunner.roadrunner import RoadRunner, NewtonIteration
from roadrunner.testing.TestModelFactory import TestModelFactory

import unittest



class NewtonIterationUnitTests(unittest.TestCase):

    def setUp(self) -> None:
        # we choose the easiest model for the unit tests.
        # see integration tests for solving other models.
        self.testModel = TestModelFactory("SimpleFluxManuallyReduced")
        self.rr = RoadRunner(self.testModel.str())

    def checkResults(self, results):
        for i in range(len(results.colnames)):
            speciesId = results.colnames[i]
            actualResult = results[0][i]
            expected = self.testModel.steadyState()[speciesId][1]
            print("Comparing ", speciesId, "expected result: ", expected, "actual result: ", actualResult)
            self.assertAlmostEqual(expected, actualResult)

    def test_newton_iteration_registered(self):
        self.assertIn("NewtonIteration", self.rr.getRegisteredSteadyStateSolverNames())

    def test_settings_map_is_dict(self):
        solver = NewtonIteration(self.rr.getModel())
        settings = solver.getSettingsMap()
        self.assertIsInstance(settings, dict)

    def test_sovle_using_newton_iteration_directly(self):
        solver = NewtonIteration(self.rr.getModel())
        solver.allow_presimulation = True
        solver.solve()
        self.checkResults(self.rr.getFloatingSpeciesConcentrationsNamedArray())

    def test_change_then_reset_settings(self):
        solver = NewtonIteration(self.rr.getModel())
        solver.strategy = "linesearch"  # default is "basic"
        # ensure we've actually changed something
        self.assertEqual("linesearch", solver.strategy)
        solver.resetSettings()
        self.assertEqual("basic", solver.strategy)

    def test_regenerate_model_before_creating_solver(self):
        self.rr.regenerate()
        solver = NewtonIteration(self.rr.getModel())
        solver.solve()
        self.checkResults(self.rr.getFloatingSpeciesConcentrationsNamedArray())

    def test_regenerate_model_after_creating_solver(self):
        solver = NewtonIteration(self.rr.getModel())
        self.rr.regenerate()
        # after regeneration, the pointer to the model is different so we must sync with model before solving.
        solver.syncWithModel(self.rr.getModel())
        solver.solve()
        self.checkResults(self.rr.getFloatingSpeciesConcentrationsNamedArray())


class NewtonIterationIntegrationTests(unittest.TestCase):

    # defined by subclasses
    strategy = None

    def setUp(self) -> None:
        pass

    def tearDown(self) -> None:
        pass

    def checkSteadyState(self, model_name):
        # grab the test model
        test_model = TestModelFactory(model_name)

        # load it into roadrunner
        rr = RoadRunner(test_model.str())

        # collect reference results
        expected_results = test_model.steadyState()

        # set starting values
        for species_name, (starting_val, expected_ss_val) in expected_results.items():
            setattr(rr, species_name, starting_val)

        # regenerate (but might not need to?)
        rr.regenerate()

        # set the steady state solver
        rr.setSteadyStateSolver("NewtonIteration")

        # get options for model
        settings = test_model.settings()

        # apply settings
        for setting, value in settings.items():
            rr.getSteadyStateSolver().setValue(setting, value)

        # apply strategy
        if not self.strategy:
            raise NotImplementedError("self.strategy parameter not implemented. "
                                      "You are probably using the NewtonIterationIntegrationTests base class "
                                      "instead of a subtype")
        rr.getSteadyStateSolver().setValue("strategy", self.strategy)

        # do steady state calculation
        rr.steadyState()

        actual_results = rr.getFloatingSpeciesConcentrationsNamedArray()
        for species_name, (starting_val, expected_ss_val) in expected_results.items():
            actual = actual_results[species_name][0]
            print("Comparing reference value : ", expected_ss_val, "with actual value: ", actual)
            self.assertAlmostEqual(expected_ss_val, actual, places=5)



class BasicNewtonIterationTests(NewtonIterationIntegrationTests):
    strategy = "basic"
    def testOpenLinearFlux(self):
        self.checkSteadyState("OpenLinearFlux")

    def testSimpleFluxManuallyReduced(self):
        self.checkSteadyState("SimpleFluxManuallyReduced")

    def testSimpleFlux(self):
        self.checkSteadyState("SimpleFlux")

    def testVenkatraman2010(self):
        self.checkSteadyState("Venkatraman2010")

    @unittest.skip("Steady state for Brown2004 model does not compute because the "
                   "moiety conservation analysis returns a singular matrix. This is a known "
                   "issue and will be resolved in time")
    def testBrown2004(self):
        self.checkSteadyState("Brown2004")

class LineSearchNewtonIterationTests(NewtonIterationIntegrationTests):
    strategy = "linesearch"
    def testOpenLinearFlux(self):
        self.checkSteadyState("OpenLinearFlux")

    def testSimpleFluxManuallyReduced(self):
        self.checkSteadyState("SimpleFluxManuallyReduced")

    def testSimpleFlux(self):
        self.checkSteadyState("SimpleFlux")

    def testVenkatraman2010(self):
        self.checkSteadyState("Venkatraman2010")

    @unittest.skip("Steady state for Brown2004 model does not compute because the "
                   "moiety conservation analysis returns a singular matrix. This is a known "
                   "issue and will be resolved in time")
    def testBrown2004(self):
        self.checkSteadyState("Brown2004")
