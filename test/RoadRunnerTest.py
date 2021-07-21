import os
import sys
import unittest

import numpy as np

thisDir = os.path.dirname(os.path.realpath(__file__))
rr_site_packages = os.path.dirname(os.path.dirname(thisDir))

sys.path += [
    rr_site_packages,
]
from roadrunner.testing import TestModelFactory as tmf


class RoadRunnerTest(unittest.TestCase):
    """A set of utilities used in Python tests"""

    def checkValidTestModelName(self, model_name: str):
        """Check that model_name is a valid TestModel name"""
        if model_name not in tmf.getAvailableTestModels():
            raise ValueError(f"The requested test model_name \"{model_name}\""
                             f"is not one of the available test models: {tmf.getAvailableTestModels()}")

    def checkTestModelImplements(self, test_model : tmf.TestModel, implementsResultType):
        """Check that test_model implements the interface implementsResultType"""

        if not issubclass(type(test_model), implementsResultType):
            raise ValueError("This test model does not implement the "
                             "SteadyStateResult interface")

    def checkMatricesEqual(self, expected: np.ndarray, actual: np.ndarray):
        passed = (expected == actual).all()
        if not passed:
            print("actual")
            print(actual)
            print("expected")
            print(expected)
        self.assertTrue(passed)