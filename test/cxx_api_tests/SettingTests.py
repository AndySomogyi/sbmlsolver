import os
import unittest

import sys

thisDir = os.path.dirname(os.path.realpath(__file__))
rr_site_packages = os.path.dirname(os.path.dirname(thisDir))

sys.path += [
    r"D:\roadrunner\roadrunner\cmake-build-release-visual-studio---with-python\lib\site-packages",
    # rr_site_packages,
]

try:
    from roadrunner.roadrunner import (
        RoadRunner, Integrator, SteadyStateSolver,
        ExecutableModel, RoadRunnerOptions
    )
    import roadrunner
except ImportError:
    from roadrunner import (
        RoadRunner, Integrator, SteadyStateSolver,
        ExecutableModel, RoadRunnerOptions
    )
    import roadrunner

from roadrunner.testing import SettingTestsSwigAPI


class SettingTests(unittest.TestCase):
    """
    A Setting (aka Variant) is the roadrunner construct for
    containing multiple types and its used for storing
    roadrunner options. Since we deliberately convert
    Setting's to their corresponding type in Python,
    and this conversion should happen seamlessly,
    the Setting object itself is not exposed to the Python
    level. We must therefore test it indirectly.
    """

    def setUp(self):
        self.s = SettingTestsSwigAPI.SettingTestsSwigAPI()

    def testCxxToPy_char(self):
        self.assertEqual(self.s.getCharTypeAsSetting(), "c")

    def testCxxToPy_uchar(self):
        self.assertEqual(self.s.getUCharTypeAsSetting(), "d")

    def testCxxToPy_float(self):
        self.assertAlmostEqual(self.s.getFloatTypeAsSetting(), 0.1234)

    def testCxxToPy_double(self):
        self.assertAlmostEqual(self.s.getDoubleTypeAsSetting(), 0.5678)

    def testCxxToPy_int32(self):
        self.assertEqual(self.s.getInt32TypeAsSetting(), -1)

    def testCxxToPy_uint32(self):
        self.assertEqual(self.s.getUint32TypeAsSetting(), 1)

    def testCxxToPy_int64(self):
        self.assertEqual(self.s.getInt64TypeAsSetting(), -2)

    def testCxxToPy_uint64(self):
        self.assertEqual(self.s.getUint64TypeAsSetting(), 2)

    def testCxxToPy_doubleVector(self):
        self.assertEqual(self.s.getDoubleVectorTypeAsSetting(), [0.1, 0.2, 0.3])

    def testCxxToPy_doubleVectorIsList(self):
        """
        We might want to consider converting std::vector
        into a numpy array instead...
        :return:
        """
        self.assertIsInstance(self.s.getDoubleVectorTypeAsSetting(), list)

    def testCxxToPy_string(self):
        self.assertEqual(self.s.getStringTypeAsSetting(), "I'm a string")

    def testPyToCxx_char(self):
        self.s.setCharType("t")
        self.assertEqual(self.s.getCharTypeAsSetting(), "t")

    @unittest.skip("Not supported. Conversion from Python string to "
                   "C++ will always go to std::string, not char or uchar")
    def testPyToCxx_uchar(self):
        pass

    def testPyToCxx_string(self):
        self.s.setStringType("I'm a Python string")
        self.assertEqual(self.s.getStringTypeAsSetting(), "I'm a Python string")

    def testPyToCxx_bool(self):
        self.s.setBoolType(False)
        self.assertFalse(self.s.getBoolTypeAsSetting())

    def testPyToCxx_int32(self):
        self.s.setInt32Type(9876)
        self.assertEqual(self.s.getInt32TypeAsSetting(), 9876)

    def testPyToCxx_uint32(self):
        self.s.setUint32Type(8476)
        self.assertEqual(self.s.getUint32TypeAsSetting(), 8476)

    def testPyToCxx_int64(self):
        self.s.setInt64Type(9287646)
        self.assertEqual(self.s.getInt64TypeAsSetting(), 9287646)

    def testPyToCxx_uint64(self):
        self.s.setUint64Type(998763552)
        self.assertEqual(self.s.getUint64TypeAsSetting(), 998763552)

    def testPyToCxx_float(self):
        self.s.setFloatType(0.8576635)
        self.assertEqual(self.s.getFloatTypeAsSetting(), 0.8576635)

    def testPyToCxx_double(self):
        self.s.setDoubleType(8.776535)
        self.assertEqual(self.s.getDoubleTypeAsSetting(), 8.776535)

    def testPyToCxx_doubleVector(self):
        self.s.setDoubleVectorType([0.6, 8.3, 76.2])
        self.assertEqual(self.s.getDoubleVectorTypeAsSetting(), [0.6, 8.3, 76.2])
