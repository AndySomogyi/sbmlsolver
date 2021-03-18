# Ensure bindings from TestModelFactory working correctly
#
# The data container classes inside TestModelFactory.h have been
# exposed to Python using swig (see TestModelFactory.i).
#
# This file resides within "<roadrunner_root>/test/sundials-tests" until
# the project is installed. Only once it is installed along side the TestModelFactory.py
# and _TestModelFactory.pyd files can this file be executed.

import unittest

import TestModelFactory as tmf


class testTestModelFactory(unittest.TestCase):

    def setUp(self) -> None:
        pass

    def tearDown(self) -> None:
        pass

    def testDoublePair(self):
        dp = tmf.DoublePair(5.4, 4.3)
        self.assertIsInstance(dp, tmf.DoublePair)
        self.assertEqual(dp[0], 5.4)

    def testSimpleFluxStr(self):
        sf = tmf.SimpleFlux()
        self.assertTrue(sf.str().startswith(r"<?xml"))

    def testSimpleFluxModelName(self):
        self.assertEqual("SimpleFlux", tmf.SimpleFlux().modelName())

    def testSimpleFluxSteadyState(self):
        sf = tmf.SimpleFlux()
        methods = [
            "modelName",
            "stateVectorAtT10",
            "steadyState",
            "str",
            "settings",
        ]
        for i in methods:
            m = getattr(sf, i)
            self.assertTrue(callable(m))

    def testNamesOfClassesExist(self):
        classNames = [
            "SimpleFlux",
            "SimpleFluxManuallyReduced",
            "OpenLinearFlux",
            "Model269",
            "Model28",
            "CeilInRateLaw",
            "FactorialInRateLaw",
            "Venkatraman2010",
            "Brown2004",
        ]
        for clsName in classNames:
            self.assertIn(clsName, dir(tmf))
