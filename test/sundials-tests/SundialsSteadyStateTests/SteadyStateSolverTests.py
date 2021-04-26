import os
import sys

thisDir = os.path.dirname(os.path.realpath(__file__))
rr_site_packages = os.path.dirname(os.path.dirname(thisDir))

sys.path += [
    rr_site_packages,
]
from roadrunner.roadrunner import RoadRunner, BasicNewtonIteration
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

            expected = self.testModel.steadyState()[speciesId]
            print("Comparing ", speciesId, "expected result: ", expected, "actual result: ", actualResult)
            self.assertAlmostEqual(expected, actualResult)

    def test_newton_iteration_registered(self):
        self.assertIn("newton", self.rr.getRegisteredSteadyStateSolverNames())

    def test_newton_linesearch_registered(self):
        self.assertIn("newton_linesearch", self.rr.getRegisteredSteadyStateSolverNames())

    def test_settings_map_is_dict(self):
        solver = BasicNewtonIteration(self.rr.getModel())
        settings = solver.getSettingsMap()
        self.assertIsInstance(settings, dict)

    def test_solve_using_newton_iteration_directly(self):
        solver = BasicNewtonIteration(self.rr.getModel())
        solver.allow_presimulation = True
        solver.solve()
        self.checkResults(self.rr.getFloatingSpeciesConcentrationsNamedArray())

    def test_change_then_reset_settings(self):
        solver = BasicNewtonIteration(self.rr.getModel())
        solver.presimulation_maximum_steps = 400
        # ensure we've actually changed something
        self.assertEqual(400, solver.presimulation_maximum_steps)
        solver.resetSettings()
        self.assertEqual(500, solver.presimulation_maximum_steps)

    def test_regenerate_model_before_creating_solver(self):
        self.rr.regenerateModel()
        solver = BasicNewtonIteration(self.rr.getModel())
        solver.solve()
        self.checkResults(self.rr.getFloatingSpeciesConcentrationsNamedArray())

    def test_regenerate_model_after_creating_solver(self):
        solver = BasicNewtonIteration(self.rr.getModel())
        self.rr.regenerateModel()
        # after regeneration, the pointer to the model is different so we must sync with model before solving.
        solver.syncWithModel(self.rr.getModel())
        solver.solve()
        self.checkResults(self.rr.getFloatingSpeciesConcentrationsNamedArray())

    def test_presimulation_times(self):
        solver = BasicNewtonIteration(self.rr.getModel())
        self.assertIsInstance(solver.presimulation_times, list)
        self.assertListEqual([0.1, 1.0, 10.0, 100.0, 1000.0, 10000.0], solver.presimulation_times)


class SteadyStateSolverIntegrationTests(unittest.TestCase):
    # defined by subclasses
    strategy = None

    def setUp(self) -> None:
        pass

    def tearDown(self) -> None:
        pass

    def checkSteadyState(self, model_name: str, solver_name: str):
        # grab the test model
        test_model = TestModelFactory(model_name)

        # load it into roadrunner
        rr = RoadRunner(test_model.str())

        # collect reference results
        expected_results = test_model.steadyState()

        # set the steady state solver
        rr.setSteadyStateSolver(solver_name)

        # get settings for this problem
        settings = test_model.steadyStateSettings()

        # apply settings
        for setting, value in settings.items():
            if setting == "moiety_conservation":
                rr.conservedMoietyAnalysis = True
            else:
                try:
                    rr.getSteadyStateSolver().setValue(setting, value)
                except Exception:
                    # its okay for a solver to not have a particular options
                    continue

        # do steady state calculation
        rr.steadyState()

        actual_results = rr.getFloatingSpeciesConcentrationsNamedArray()
        for species_name, expected_ss_val in expected_results.items():
            actual = actual_results[species_name][0]
            print("Comparing reference value : ", expected_ss_val, "with actual value: ", actual)
            self.assertAlmostEqual(expected_ss_val, actual, places=5)


class BasicNewtonIterationTests(SteadyStateSolverIntegrationTests):

    def testOpenLinearFlux(self):
        self.checkSteadyState("OpenLinearFlux", "newton")

    def testSimpleFluxManuallyReduced(self):
        self.checkSteadyState("SimpleFluxManuallyReduced", "newton")

    def testSimpleFlux(self):
        self.checkSteadyState("SimpleFlux", "newton")

    def testVenkatraman2010(self):
        self.checkSteadyState("Venkatraman2010", "newton")

    def testBrown2004(self):
        self.checkSteadyState("Brown2004", "newton")


class LineSearchNewtonIterationTests(SteadyStateSolverIntegrationTests):
    def testOpenLinearFlux(self):
        self.checkSteadyState("OpenLinearFlux", "newton_linesearch")

    def testSimpleFluxManuallyReduced(self):
        self.checkSteadyState("SimpleFluxManuallyReduced", "newton_linesearch")

    def testSimpleFlux(self):
        self.checkSteadyState("SimpleFlux", "newton_linesearch")

    def testVenkatraman2010(self):
        self.checkSteadyState("Venkatraman2010", "newton_linesearch")

    def testBrown2004(self):
        self.checkSteadyState("Brown2004", "newton_linesearch")


class NLEQ1IntegrationTests(SteadyStateSolverIntegrationTests):
    def testOpenLinearFlux(self):
        self.checkSteadyState("OpenLinearFlux", "nleq1")

    def testSimpleFluxManuallyReduced(self):
        self.checkSteadyState("SimpleFluxManuallyReduced", "nleq1")

    def testSimpleFlux(self):
        self.checkSteadyState("SimpleFlux", "nleq1")

    def testVenkatraman2010(self):
        self.checkSteadyState("Venkatraman2010", "nleq1")

    def testBrown2004(self):
        self.checkSteadyState("Brown2004", "nleq1")


class NLEQ2IntegrationTests(SteadyStateSolverIntegrationTests):
    def testOpenLinearFlux(self):
        self.checkSteadyState("OpenLinearFlux", "nleq2")

    def testSimpleFluxManuallyReduced(self):
        self.checkSteadyState("SimpleFluxManuallyReduced", "nleq2")

    def testSimpleFlux(self):
        self.checkSteadyState("SimpleFlux", "nleq2")

    def testVenkatraman2010(self):
        self.checkSteadyState("Venkatraman2010", "nleq2")

    def testBrown2004(self):
        self.checkSteadyState("Brown2004", "nleq2")
