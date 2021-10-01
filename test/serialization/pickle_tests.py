from threading import Thread
from multiprocessing import Queue
from multiprocessing.pool import ThreadPool
import numpy as np
import unittest
import pandas as pd
import pickletools
import sys
import pickle
import os
from os.path import dirname, exists, join


print(f"Python interpreter at: {sys.executable}")

sys.path += [
    r'/mnt/d/roadrunner/roadrunner/cmake-build-release-wsl/lib/site-packages'
    # r"D:\roadrunner\roadrunner\install-msvc2019-rel\site-packages",
    # r"D:\roadrunner\roadrunner\cmake-build-release-visual-studio\lib\site-packages"

]
import roadrunner
from roadrunner import *
import copy


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
        pfile = os.path.join(os.path.dirname(__file__), "pkl.serialization")
        with open(pfile, 'wb') as f:
            pickle.dump(self.rr, f)

        self.assertTrue(os.path.isfile(pfile))

        with open(pfile, "rb") as f:
            rr = pickle.load(f)
        try:
            print(rr.simulate(0, 10, 11))
        except Exception:
            self.fail("Cannot simulate a serialization loaded model")

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


