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
# from roadrunner.roadrunner import RoadRunner, NewtonIteration
from roadrunner.testing.TestModelFactory import TestModelFactory, OpenLinearFlux
# import roadrunner
import unittest



class NewtonIterationIntegrationTests(unittest.TestCase):

    def setUp(self) -> None:
        pass

    def checkSteadyState(self, model_name: str):
        testModel = TestModelFactory(model_name)
        assert testModel != None, "Test model is None"

        # # load roadrunner model
        # rr = RoadRunner(testModel.str())
        #
        # # collet reference results
        #
        # expected_results = testModel.steadyState()
        #
        # # insert starting values into the model
        # for speciesName, (startingVal, ss) in expected_results.items():
        #     setattr(rr, speciesName, startingVal)
        # # ensure we've updated the model (might not be needed)
        # rr.regenerate()
        #
        # # use NewtonIteration
        # rr.setSteadyStateSolver("NewtonIteration")

        settings = testModel.settings()
        print(settings, type(settings))

        # apply the solver settings
        # for settingName, settingValue in testModel.settings().items():
        #     print(settingName, settingValue)
            # rr.solver.setValue(settingName, settingValue)
        #
        # rr.steadyState()
        #
        # actualResults = rr.getFloatingSpeciesConcentrationsNamedArray()
        # print(actualResults)
        # print(actualResults.items())
        # for speciesName in actualResults.colnames:
        #     actualResult = actualResults[0][speciesName]

    def test_open_linear_flux(self):
        self.checkSteadyState("OpenLinearFlux")




































