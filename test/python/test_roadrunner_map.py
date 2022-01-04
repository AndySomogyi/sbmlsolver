"""Unit test style tests for RoadRunnerMap API"""

import os
import sys
import unittest

thisDir = os.path.dirname(os.path.realpath(__file__))
rr_site_packages = os.path.dirname(os.path.dirname(thisDir))

sys.path += [
    rr_site_packages,
    r"D:\roadrunner\roadrunner\cmake-build-release\lib\site-packages"
]
import roadrunner as rr
from roadrunner import RoadRunnerMap, RoadRunner
import roadrunner.tests.TestModelFactory as tmf


class RoadRunnerMapTests(unittest.TestCase):

    def setUp(self) -> None:
        self.sbmlStrings = [
            tmf.OpenLinearFlux().str(),
            tmf.SimpleFlux().str(),
            tmf.Brown2004().str(),
            tmf.FactorialInRateLaw().str(),
            tmf.Venkatraman2010().str()
        ]
        self.N = len(self.sbmlStrings)

    def test_ctor1(self):
        rrm = RoadRunnerMap()
        self.assertEqual(0, len(rrm))

    def test_ctor2(self):
        rrm = RoadRunnerMap(self.sbmlStrings, 2)
        self.assertEqual(len(self.sbmlStrings), len(rrm))

    def test_keys(self):
        rrm = RoadRunnerMap(self.sbmlStrings, 2)
        expected = ('SimpleFlux', 'x', 'FactorialModel1', '__main', 'Brown2004_NGF_EGF_signaling')
        self.assertEqual(sorted(expected), sorted(rrm.keys()))

    def test_size_1thread(self):
        rrm = RoadRunnerMap(self.sbmlStrings, 1)
        self.assertEqual(self.N, len(rrm))

    def test_size_2thread(self):
        rrm = RoadRunnerMap(self.sbmlStrings, 2)
        self.assertEqual(self.N, len(rrm))

    def test_values(self):
        rrm = RoadRunnerMap(self.sbmlStrings, 2)
        values = rrm.values()
        self.assertIsInstance(values, list)
        self.assertTrue(all([isinstance(i, RoadRunner) for i in values]))

    def test_items(self):
        rrm = RoadRunnerMap(self.sbmlStrings, 2)
        items = rrm.items()
        self.assertIsInstance(items, list)
        for name, rrModel in rrm.items():
            self.assertIsInstance(name, str)
            self.assertIsInstance(rrModel, RoadRunner)

    def test_insert_from_empty(self):
        rrm = RoadRunnerMap()
        rrm.insert(self.sbmlStrings[0])
        self.assertEqual(1, len(rrm))
        self.assertEqual(rrm.keys()[0], "x")

    def test_insert_from_empty_with_custom_name(self):
        rrm = RoadRunnerMap()
        rrm.insert("NewName", self.sbmlStrings[0])
        self.assertEqual(1, len(rrm))
        self.assertEqual(rrm.keys()[0], "NewName")

    def test_insert_from_populated(self):
        rrm = RoadRunnerMap(self.sbmlStrings[:2], 2)
        rrm.insert(self.sbmlStrings[-1])
        self.assertEqual(3, len(rrm))

    def test_insert_from_populated_custom_name(self):
        rrm = RoadRunnerMap(self.sbmlStrings[:2], 2)
        rrm.insert("NewModel", self.sbmlStrings[-1])
        self.assertEqual(3, len(rrm))
        self.assertTrue("NewModel" in rrm)

    def test_insert_from_list_of_strings(self):
        n = len(self.sbmlStrings[:2]) + len(self.sbmlStrings[2:])
        rrm = RoadRunnerMap(self.sbmlStrings[:2], 2)
        rrm.insert(self.sbmlStrings[2:])
        self.assertEqual(n, len(rrm))

    def test___setitem__(self):
        rrm = RoadRunnerMap(self.sbmlStrings[:-1], 2)
        last = self.sbmlStrings[-1]
        rrm["last"] = last
        self.assertEqual(len(self.sbmlStrings), len(rrm))

    def test___setitem__exception(self):
        """
        This is explicitely prohibited to prevent users from looping over a list of
        sbml paths/strings and entering them one by one into the map. This situation
        should instead be handled by passing in the list of strings.
        """
        rrm = RoadRunnerMap(self.sbmlStrings[:-1], 2)
        last = self.sbmlStrings[-1]
        with self.assertRaises(TypeError):
            rrm["last"] = RoadRunner(last)

    def test___getitem__(self):
        rrm = RoadRunnerMap(self.sbmlStrings, 2)
        simpleFlux = rrm["SimpleFlux"]
        self.assertIsInstance(simpleFlux, RoadRunner)

    def test___delitem__(self):
        rrm = RoadRunnerMap(self.sbmlStrings, 2)
        self.assertEqual(len(self.sbmlStrings), len(rrm))
        del rrm["SimpleFlux"]
        self.assertEqual(len(self.sbmlStrings) - 1, len(rrm))

    def test_empty_when_true(self):
        rrm = RoadRunnerMap()
        self.assertTrue(rrm.empty)

    def test_empty_when_false(self):
        rrm = RoadRunnerMap(self.sbmlStrings, 2)
        self.assertFalse(rrm.empty())

    def test___len__(self):
        rrm = RoadRunnerMap(self.sbmlStrings, 2)
        self.assertEqual(len(self.sbmlStrings), len(rrm))

    def test_clear(self):
        rrm = RoadRunnerMap(self.sbmlStrings, 2)
        self.assertEqual(len(self.sbmlStrings), len(rrm))
        self.assertFalse(rrm.empty())
        rrm.clear()
        self.assertTrue(rrm.empty())

    def test_contains_positive(self):
        rrm = RoadRunnerMap(self.sbmlStrings, 2)
        self.assertTrue("x" in rrm)

    def test_contains_negative(self):
        rrm = RoadRunnerMap(self.sbmlStrings, 2)
        self.assertFalse("x" not in rrm)

    def test_numThreads(self):
        rrm = RoadRunnerMap(self.sbmlStrings, 2)
        self.assertEqual(2, rrm.getNumThreads())
        rrm.setNumThreads(4)
        self.assertEqual(4, rrm.getNumThreads())

    def test_iterators(self):
        rrm = RoadRunnerMap(self.sbmlStrings, 2)
        expected = [
            "Brown2004_NGF_EGF_signaling",
            "SimpleFlux",
            "FactorialModel1",
            "__main",
            "x"
        ]
        k = []
        for i in rrm:
            k.append(i)
        self.assertEqual(sorted(k), sorted(expected))


    def test_iterators_items(self):
        rrm = RoadRunnerMap(self.sbmlStrings, 2)
        expected = [
            "Brown2004_NGF_EGF_signaling",
            "SimpleFlux",
            "FactorialModel1",
            "__main",
            "x"
        ]
        k = []
        for name, rr in rrm.items():
            k.append(name)
        self.assertEqual(sorted(k), sorted(expected))


if __name__ == "__main__":
    unittest.main()
