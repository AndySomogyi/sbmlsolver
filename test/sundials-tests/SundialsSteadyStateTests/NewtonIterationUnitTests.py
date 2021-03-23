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
from roadrunner.testing.TestModelFactory import TestModelFactory, OpenLinearFlux

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
            print("Comparing ", speciesId, "expected result: " , expected, "actual result: " , actualResult)
            self.assertAlmostEqual(expected, actualResult)

    def test_newton_iteration_registered(self):
        self.assertIn("NewtonIteration", self.rr.getRegisteredSteadyStateSolverNames())

    def test_settings_map_is_dict(self):
        solver = NewtonIteration(self.rr.getModel())
        settings = solver.getSettingsMap()
        print(type(settings))
        print(settings.keys())
        print(settings["allow_presimulation"])
        self.assertIsInstance(settings, dict)

    def test_sovle_using_newton_iteration_directly(self):
        solver = NewtonIteration(self.rr.getModel())
        solver.allow_presimulation = True
        solver.solve()
        self.checkResults(self.rr.getFloatingSpeciesConcentrationsNamedArray())

    def test_change_then_reset_settings(self):
        solver = NewtonIteration(self.rr.getModel())
        solver.strategy = "linesearch" # default is "basic"
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





































