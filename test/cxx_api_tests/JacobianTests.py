import os
import sys

thisDir = os.path.dirname(os.path.realpath(__file__))
rr_site_packages = os.path.dirname(os.path.dirname(thisDir))

sys.path += [
    r"D:\roadrunner\roadrunner\cmake-build-release-visual-studio---with-python\lib\site-packages",
    rr_site_packages,
]

try:
    from roadrunner.roadrunner import (
        RoadRunner, Integrator, SteadyStateSolver,
        ExecutableModel, RoadRunnerOptions, Config
    )
    import roadrunner
except ImportError:
    from roadrunner import (
        RoadRunner, Integrator, SteadyStateSolver,
        ExecutableModel, RoadRunnerOptions
    )
    import roadrunner

from roadrunner.testing import TestModelFactory as tmf
from roadrunner.testing.RoadRunnerTest import RoadRunnerTest


class JacobianTests(RoadRunnerTest):

    def setUp(self) -> None:
        pass

    def tearDown(self) -> None:
        pass

    def loadTestModel(self, modelName: str):
        """Instantiate instance of TestModel called modelName"""
        self.checkValidTestModelName(modelName)
        testModel = tmf.TestModelFactory(modelName)
        self.checkTestModelImplements(testModel, tmf.JacobianResult)
        return testModel

    def _checkJac(self, modelName: str, tol: float, rrJacMode: Config.ROADRUNNER_JACOBIAN_MODE, full: bool):
        """
        check the values of TestModel jacobian interface against those computed by the current model

        If full is set to false, then we turn on moiety conservation

        :param modelName: A valid test model name (See TestModelFactory::getAvailableTestModelNames()
        :param tol:  degree of tolerance, allowed error
        :param rrJacMode: either Config::ROADRUNNER_JACOBIAN_MODE_AMOUNTS or Config::ROADRUNNER_JACOBIAN_MODE_CONCENTRATIONS
        :param full: true if we are looking at full jacobian, false if were looking at reduced.
        """
        testModel = self.loadTestModel(modelName)
        rr = RoadRunner(testModel.str())
        Config.setValue(Config.ROADRUNNER_JACOBIAN_MODE, rrJacMode)

        expectedJac = None
        if full and rrJacMode == Config.ROADRUNNER_JACOBIAN_MODE_CONCENTRATIONS:
            expectedJac = testModel.fullJacobianConc()
        elif full and rrJacMode == Config.ROADRUNNER_JACOBIAN_MODE_AMOUNTS:
            expectedJac = testModel.fullJacobianAmt()
        elif not full and rrJacMode == Config.ROADRUNNER_JACOBIAN_MODE_CONCENTRATIONS:
            expectedJac = testModel.reducedJacobianConc()
        elif not full and rrJacMode == Config.ROADRUNNER_JACOBIAN_MODE_AMOUNTS:
            expectedJac = testModel.reducedJacobianAmt()
        else:
            raise ValueError("Should never reach here")

        actualJac = None
        if full:
            actualJac = rr.getFullJacobian()
        else:
            rr.conservedMoietyAnalysis = True
            if rr.getModel().getNumConservedMoieties() == 0:
                rr.conservedMoietyAnalysis = False
            actualJac = rr.getReducedJacobian()

        self.checkMatricesEqual(expectedJac, actualJac)

    def test_BiomolecularEnd_FullAmt(self):
        self._checkJac("BiomolecularEnd", 4, Config.ROADRUNNER_JACOBIAN_MODE_AMOUNTS, True)

    def test_BiomolecularEnd_FullConc(self):
        self._checkJac("BiomolecularEnd", 4, Config.ROADRUNNER_JACOBIAN_MODE_CONCENTRATIONS, True)

    def test_BiomolecularEnd_ReducedAmt(self):
        self._checkJac("BiomolecularEnd", 4, Config.ROADRUNNER_JACOBIAN_MODE_AMOUNTS, False)

    def test_BiomolecularEnd_ReducedConc(self):
        self._checkJac("BiomolecularEnd", 4, Config.ROADRUNNER_JACOBIAN_MODE_CONCENTRATIONS, False)
