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
        c, args, state, _, _ = self.data.__reduce_ex__(5)
        self.assertTrue(callable(c))

    def test_reduce_args(self):
        c, args, state, _, _ = self.data.__reduce_ex__(5)
        self.assertEqual(args, ((11, 3),))

    def test_reduce_state(self):
        c, args, state, _, _ = self.data.__reduce_ex__(5)
        self.assertEqual(
            list(state.keys()),
            ['array', 'nDims', 'dim1',
             'dim2', 'rownames', 'colnames',
             '_pickle_version'])

    def test_reduce_args_work_with_callable(self):
        c, args, state, _, _ = self.data.__reduce_ex__(5)
        initialized = c(*args)
        self.assertIsInstance(initialized, NamedArray)
        self.assertEqual(initialized.shape, (11, 3))

    def test_reduce_and_setstate(self):
        c, args, state, _, _ = self.data.__reduce_ex__(5)
        newArray = c(*args)
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

    def test_NamedArray_is_part_of__roadrunner_module(self):
        """
        the tp_name field of NamedArray_Type needed to be
        changed to roadrunner._roadrunner.NamedArray rather
        than NamedArray so that the __module__attribute
        pointed to the correct location and pickle.dumps
        looks in the right place for NamedArray
        :return:
        """
        from roadrunner._roadrunner import NamedArray
        self.assertTrue(NamedArray.__module__, "roadrunner._roadrunner")

    def test_dumps(self):
        print(f"Python interpreter at: {sys.executable}")
        print(f'roadrunner at: {roadrunner.__file__}')

        binary = pickle.dumps(self.data)
        self.assertIsInstance(binary, bytes)

    def test_loads(self):
        binary = pickle.dumps(self.data)
        pickle.loads(binary)



    def testy(self):
        import roadrunner
        from roadrunner._roadrunner import NamedArray
        # from roadrunner._roadrunner import named_array

        # for i in dir(roadrunner._roadrunner):
        #     print(i)

        # print(NamedArray.__module__)
        # import named_array

    def test_d(self):
        l = 1
        print(l.__reduce_ex__(5))

    def test_to_pickle_dump(self):
        # l = (11, 3)
        # lred = l.__reduce_ex__(5)
        # fname = os.path.join(os.path.dirname(__file__), "data.pickle")
        # print (pickletools.dis(pickle.dumps(lred, protocol=5)))
        from roadrunner._roadrunner import NamedArray
        n = NamedArray()
        print(self.data.__reduce_ex__(5))
        # binary = pickle.dumps(self.data)

        print(pickletools.dis(pickle.dumps(binary, protocol=5)))
        n.__reduce_ex__(binary)
        # print(pickle.loads(binary))
        # data2 = pickle.loads(binary)
        # print(type(data2))
        # print(data2)

        # with open(fname, 'wb') as f:
        #     pickle.dump(self.data, f)
        # with open(fname, 'rb') as f:
        #     data2 = pickle.load(f)

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
