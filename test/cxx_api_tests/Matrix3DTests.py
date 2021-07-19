import os
import sys

import numpy as np

thisDir = os.path.dirname(os.path.realpath(__file__))
rr_site_packages = os.path.dirname(os.path.dirname(thisDir))

sys.path += [
    rr_site_packages,
]
from roadrunner.roadrunner import RoadRunner, ForwardSensitivitySolver
import roadrunner.testing.TestModelFactory as tmf

import unittest


class Matrix3DTests(unittest.TestCase):
    maxDiff = None

    def setUp(self) -> None:

        self.testModel = tmf.TestModelFactory("OpenLinearFlux")
        self.rr = RoadRunner(self.testModel.str())
        self.model = self.rr.getModel()
        print(self.model)
        self.sens = ForwardSensitivitySolver(self.model)

    def checkMatricesEqual(self, expected, actual, places=7):
        for i in range(expected.shape[0]):
            for j in range(expected.shape[1]):
                self.assertAlmostEqual(expected[i, j], actual[i, j], places=places)


    def test(self):
        pass
