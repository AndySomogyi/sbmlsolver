"""Unit test style tests for RoadRunnerMap API"""

import os
import sys
import unittest

thisDir = os.path.dirname(os.path.realpath(__file__))
rr_site_packages = os.path.dirname(os.path.dirname(thisDir))

sys.path += [
    rr_site_packages,
]
from roadrunner import RoadRunnerMap
import roadrunner.tests.TestModelFactory as tmf

class RoadRunnerMapTests(unittest.TestCase):

    def __init__(self):
        self.sbmlStrings = [

        ]
