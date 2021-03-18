import os
import sys



sys.path += [
    r"D:\roadrunner\roadrunner\install-msvc2019-rel\site-packages",
    r"D:\roadrunner\roadrunner\install-msvc2019-rel\site-packages\roadrunner\testing"
             ]
from roadrunner.roadrunner import RoadRunner
from TestModelFactory import TestModelFactory

import unittest


class SteadyStateSolverTests(unittest.TestCase):

    def setUp(self) -> None:
        self.testModel = TestModelFactory("OpenLinearFlux")
        self.rr = RoadRunner(self.testModel.str())

        # collect

        print([i for i in dir(self.rr) if "float" in i.lower()])

    def checkSteadyStateValues(self, solverName:str, modelName:str):
        """
        Compare computed steady state results with reference results. This
        method is semantically equivalent to NewtonIterationIntegrationTests::testSteadyState<ModelType>.

        :param solverName: name of solver to use (see rr.getRegisteredSteadyStateSolverNames())
        :param modelName: Name of the model to load. Choices are
            SimpleFlux, SimpleFluxManuallyReduced, OpenLinearFlux,
            Model269, Model28, CeilInRateLaw, FactorialInRateLaw,
            Venkatraman2010, Brown2004. See TestModelFactory.h for full
            class definitions.
        """
        # collect model containing sbml and steady state data
        testModel = TestModelFactory(modelName)

        # collect reference results which has the structure
        expectedResult = testModel.steadyState()
        print(expectedResult)

        # load model into roadrunner
        rr = RoadRunner(testModel.str())

        # change to solverName
        rr.setSteadyStateSolver(solverName)

        # insert starting values


    def test_newton_iteration_registered(self):
        self.assertIn("NewtonIteration", self.rr.getRegisteredSteadyStateSolverNames())

    def test_newton_iteration(self):
        rr = RoadRunner(self.testModel.str())

        self.checkSteadyStateValues("NewtonIteration", "OpenLinearFlux")

        # rr.setSteadyStateSolver("NewtonIteration")
        # rr.steadyState()
        # print(self.testModel.steadyState())
        # print(rr.getFloatingSpeciesConcentrationsNamedArray())

























