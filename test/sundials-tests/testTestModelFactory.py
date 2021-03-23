# Ensure bindings from TestModelFactory working correctly
#
# The data container classes inside TestModelFactory.h have been
# exposed to Python using swig (see TestModelFactory.i).

import os
import sys
import unittest

thisDir = os.path.dirname(os.path.realpath(__file__))
rr_site_packages = os.path.dirname(os.path.dirname(thisDir))
print("thisDir", thisDir)
print("rr_site_packages", rr_site_packages)

sys.path += [
    rr_site_packages,
    r"D:\roadrunner\roadrunner\install-msvc2019-rel-swig-4.0.2\site-packages",  # todo delete me
]
from roadrunner.testing import TestModelFactory as tmf


class testTestModelFactory(unittest.TestCase):

    def setUp(self) -> None:
        pass

    def tearDown(self) -> None:
        pass

    def testDoublePair(self):
        x = tmf._testDoublePair(3.4, 5.6)
        self.assertIsInstance(x, tuple)
        self.assertEqual(x[0], 3.4)

    def testUseVariant(self):
        x = tmf._useVariant()
        print(x,type(x))
        self.assertIsInstance(x, float)

    def testVariantMap(self):
        x = tmf._testVariantMap()
        print(x,type(x))
        self.assertIsInstance(x, dict)
        self.assertIsInstance(list(x.keys())[0], str)
        self.assertIsInstance(list(x.values())[0], int)

    def testResultMap(self):
        x = tmf._testResultMap()
        self.assertIsInstance(x, dict)

    def testGetSettings(self):
        open_linear_flux = tmf.OpenLinearFlux()
        expected = {'allow_presimulation': True, 'presimulation_time': 5}
        self.assertEqual(expected, open_linear_flux.settings())

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

    def test_polymorphic_factory(self):
        testModel = tmf.TestModelFactory("OpenLinearFlux")
        self.assertIsInstance(testModel, tmf.OpenLinearFlux) # fails if is type TestModel
