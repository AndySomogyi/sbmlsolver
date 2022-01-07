import os
import sys

thisDir = os.path.dirname(os.path.realpath(__file__))
rr_site_packages = os.path.dirname(os.path.dirname(thisDir))

sys.path += [
    rr_site_packages,
]
from roadrunner.roadrunner import RoadRunner, BasicNewtonIteration
from roadrunner.tests.TestModelFactory import TestModelFactory

import unittest


class AutomaticMoietyConservationTests(unittest.TestCase):
    """Test the automatic moiety analysis option, which is present in superclass
    of all steady state solvers.
    Three components to these tests:
     1) The model can either need moiety conservation analysis or not.
     2) The auto_moiety_analysis option can be on or off
     3) The RoadRunner moiety_conservation_analysis option can be on or off
    Test are named using this convention:
     (Needed or not)(auto_moiety_analysis)(moiety_conservation_analysis option)
     i.e. (Needed)(On)(ON) --> OnNeededOn
     There are 3 options each with 2 possibilities --> 2^3 = 8 possibilities.
     Two different models are used:
         1) OpenLinearFlux, which does not require moiety conservation analysis
         1) SimpleFlux, which requires moiety conservation analysis
    """
    def setUp(self) -> None:
        pass

    def check(self, needed:bool, auto_moiety_option: bool, moiety_option:bool):
        test_model = TestModelFactory("SimpleFlux" if needed else "OpenLinearFlux")

        rr = RoadRunner(test_model.str())
        rr.conservedMoietyAnalysis = moiety_option

        solver = rr.getSteadyStateSolver()
        solver.setValue("auto_moiety_analysis", auto_moiety_option)

        # if moieties are needed, and both autoMoiety and moiety options are false
        # then we expect an error
        if test_model.modelName() == "SimpleFlux" and not auto_moiety_option and not moiety_option:
            with self.assertRaises(Exception):
                rr.steadyState()
        else:
            rr.steadyState()
            expected = test_model.steadyState()
            actual = rr.getFloatingSpeciesConcentrationsNamedArray()
            for species_name, steady_state_val in expected.items():
                print("Comparing expected steady state value: ", expected[species_name],
                      "with actual steady state value", actual[species_name][0])
                self.assertAlmostEqual(expected[species_name], actual[species_name][0])

    def test_needed_on_on(self):
        self.check(True, True, True)

    def test_needed_on_off(self):
        self.check(True, True, False)

    def test_needed_off_on(self):
        self.check(True, False, True)

    def test_needed_off_off(self):
        """Errors out"""
        self.check(True, False, False)

    def test_not_needed_on_on(self):
        self.check(False, True, True)

    def test_not_needed_on_off(self):
        self.check(False, True, False)

    def test_not_needed_off_on(self):
        self.check(False, False, True)

    def test_not_needed_off_off(self):
        """Errors out"""
        self.check(False, False, False)


if __name__ == "__main__":
    unittest.main()


