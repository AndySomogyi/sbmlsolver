from threading import Thread
from multiprocessing import Queue
from multiprocessing.pool import ThreadPool
import numpy as np
import unittest
import pandas as pd

import sys
import pickle
import os
from os.path import dirname, exists, join

import roadrunner
from roadrunner import *
import copy

sys.path += [
    # r"D:\roadrunner\roadrunner\install-msvc2019-rel\site-packages",
    r"D:\roadrunner\roadrunner\cmake-build-release-visual-studio\lib\site-packages"
]

import roadrunner.testing.TestModelFactory as tmf
from roadrunner.roadrunner import RoadRunner


def simulate_return_None(r):
    print(r.simulate(0, 10, 11))


def simulate_return_dataframe(r):
    data = r.simulate(0, 10, 11)
    df = pd.DataFrame(data, columns=data.colnames)
    return df


def simulate_return_NamedArray(r):
    return r.simulate(0, 10, 11)


class RoadRunnerPickleTests(unittest.TestCase):

    def setUp(self):
        rr = RoadRunner(tmf.SimpleFlux().str())
        rr.setIntegrator("gillespie")
        gillespie = rr.getIntegrator()
        gillespie.seed = 123
        self.rr = rr

    def tearDown(self):
        pass

    def test_to_pickle_and_back(self):
        pfile = os.path.join(os.path.dirname(__file__), "pkl.pickle")
        with open(pfile, 'wb') as f:
            pickle.dump(self.rr, f)

        self.assertTrue(os.path.isfile(pfile))

        with open(pfile, "rb") as f:
            rr = pickle.load(f)
        try:
            print(rr.simulate(0, 10, 11))
        except Exception:
            self.fail("Cannot simulate a pickle loaded model")

        if os.path.isfile(pfile):
            os.remove(pfile)

    def test_we_can_copy_rr(self):
        from copy import copy
        rr2 = copy(self.rr)
        self.assertNotEqual(hex(id(self.rr)), hex(id(rr2)))

    def test_pool_returns_None(self):
        import numpy as np
        from multiprocessing import Pool
        p = Pool(processes=4)
        p.map(simulate_return_None, [self.rr for i in range(10)])
        p.close()

    def test_pool_returns_DataFrame(self):
        import numpy as np
        from multiprocessing import Pool
        p = Pool(processes=4)
        dfs = p.map(simulate_return_dataframe, [self.rr for i in range(10)])
        p.close()
        self.assertEqual(len(dfs), 10)

    @unittest.skip("This test does not pass because "
                   "the NamedArray object is not yet "
                   "picklable")
    def test_pool_returns_NamedArray(self):
        import numpy as np
        from multiprocessing import Pool
        p = Pool(processes=4)
        dfs = p.map(simulate_return_NamedArray, [self.rr for i in range(10)])
        p.close()
        print(dfs)


class NamedArrayPickleTests(unittest.TestCase):
    rr = RoadRunner(tmf.SimpleFlux().str())

    def setUp(self) -> None:
        self.data = self.rr.simulate(0, 10, 11)
        # set some rownames for the sake of testing
        self.data.rownames = [i for i in range(11)]

    def tearDown(self) -> None:
        pass

    def test_getstate_isdict(self):
        self.assertIsInstance(self.data.__getstate__(), dict)

    def test_getstate_rownames(self):
        state = self.data.__getstate__()
        self.assertEqual(state['rownames'], [i for i in range(11)])

    def test_getstate_colnames(self):
        state = self.data.__getstate__()
        print(state['colnames'])
        self.assertEqual(state['colnames'], ['time', '[S1]', '[S2]'])

    def test_getstate_array(self):
        state = self.data.__getstate__()
        self.assertIsInstance(state['array'], bytes)

    def test_reduce_callable(self):
        c, args, state = self.data.__reduce__()
        self.assertTrue(callable(c))

    def test_reduce_args(self):
        c, args, state = self.data.__reduce__()
        self.assertEqual(args, (11, 3))

    def test_reduce_state(self):
        c, args, state = self.data.__reduce__()
        self.assertEqual(
            list(state.keys()),
            ['array', 'nDims', 'dim1',
             'dim2', 'rownames', 'colnames',
             '_pickle_version'])

    def test_reduce_args_work_with_callable(self):
        c, args, state = self.data.__reduce__()
        initialized = c(args)
        self.assertIsInstance(initialized, NamedArray)
        self.assertEqual(initialized.shape, (11, 3))

    def test_reduce_and_setstate(self):
        c, args, state = self.data.__reduce__()
        newArray = c(args)
        newArray.__setstate__(state)
        # make sure we still have a NamedArray after setstate
        self.assertIsInstance(newArray, NamedArray)
        self.assertEqual(newArray.shape, (11, 3))
        self.assertTrue((self.data == newArray).all())

    def test_getstate_and_setstate(self):
        pickled = NamedArray(self.data.shape)
        self.assertNotEqual(id(self.data), id(pickled))
        state = self.data.__getstate__()
        self.assertIsInstance(state, dict)
        pickled.__setstate__(state)
        self.assertAlmostEqual(self.data['[S1]'][1], 9.062504129616269)

    def testx(self):
        state = self.data.__getstate__()
        arr = state['array']
        print(arr)

        # n = NamedArray()

    def test_to_pickle_dump(self):
        fname = os.path.join(os.path.dirname(__file__), "data.pickle")

        with open(fname, 'wb') as f:
            pickle.dump(self.data, f)
        # with open(fname, 'rb') as f:
        #     data2 = pickle.load(f)
        #
        # print(data2)

        #
        # print(n)
        #     # k, rownames=['R1'], colnames=[f'C{i}' for i in k]
        # # ))
        # import builtins
        # # for i in dir(builtins):
        # #     print(i)
        # # for i in dir(roadrunner._roadrunner):
        # #     print(i)
        # # print(dir(builtins))
        # # if os.path.isfile(fname):
        # #     os.remove(fname)
        # #
        # with open(fname, 'wb') as f:
        #     pickle.dump(self.data, f)
        #
        # # with open(fname, 'wb') as f:
        # #     data2 = pickle.load(f)
        # #
        # # print(data2)
        #
