import os
import sys

thisDir = os.path.dirname(os.path.realpath(__file__))
rr_site_packages = os.path.dirname(os.path.dirname(thisDir))

sys.path += [
    rr_site_packages,
]
from roadrunner.roadrunner import RoadRunner
from roadrunner.testing.TestModelFactory import TestModelFactory

import unittest

class CVODEIntegratorTests(unittest.TestCase):
    maxDiff = None

    def setUp(self) -> None:

        self.testModel = TestModelFactory("OpenLinearFlux")
        self.rr = RoadRunner(self.testModel.str())
        # self.integrator = self.rr.getIntegrator()

    # def tearDown(self) -> None:
    #     pass
    #
    def test_relative_tolerance(self):
        print(self.testModel)
    #     self.integrator.setValue("relative_tolerance", 1e-07)
    #     self.assertEqual(1e-07, self.integrator.getValue("relative_tolerance"))
    #
    # def test_absolute_tolerance(self):
    #     self.integrator.setValue("absolute_tolerance", 1e-13)
    #     self.assertEqual(1e-13, self.integrator.getValue("absolute_tolerance"))
    #
    # def test_stiff(self):
    #     self.integrator.setValue("stiff", False)
    #     self.assertFalse(self.integrator.getValue("stiff"))
    #
    # def test_set_maximum_bdf_order(self):
    #     self.integrator.setValue("maximum_bdf_order", 3)
    #     self.assertEqual(3, self.integrator.getValue("maximum_bdf_order"))
    #
    # def test_set_maximum_adams_order(self):
    #     self.integrator.setValue("maximum_adams_order", 2)
    #     self.assertEqual(2, self.integrator.getValue("maximum_adams_order"))
    #
    # def test_set_maximum_num_steps(self):
    #     self.integrator.setValue("maximum_num_steps", 500)
    #     self.assertEqual(500, self.integrator.getValue("maximum_num_steps"))
    #
    # def test_set_maximum_time_step(self):
    #     self.integrator.setValue("maximum_time_step", 5)
    #     self.assertEqual(5, self.integrator.getValue("maximum_time_step"))
    #
    # def test_set_minimum_time_step(self):
    #     self.integrator.setValue("minimum_time_step", 0.1)
    #     self.assertEqual(0.1, self.integrator.getValue("minimum_time_step"))
    #
    # def test_set_initial_time_step(self):
    #     self.integrator.setValue("initial_time_step", 3.56)
    #     self.assertEqual(3.56, self.integrator.getValue("initial_time_step"))
    #
    # def test_set_multiple_steps(self):
    #     self.integrator.setValue("multiple_steps", True)
    #     self.assertTrue(self.integrator.getValue("multiple_steps"))
    #
    # def test_set_variable_step_size(self):
    #     self.integrator.setValue("variable_step_size", True)
    #     self.assertTrue(self.integrator.getValue("variable_step_size"))
    #
    # def test_set_max_output_rows(self):
    #     self.integrator.setValue("max_output_rows", 50)
    #     self.assertEqual(50, self.integrator.getValue("max_output_rows"))
    #
    # def test_set_concentration_tolerance(self):
    #     self.integrator.setConcentrationTolerance(1e-8)


