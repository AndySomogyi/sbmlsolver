import numpy as np
import sys

sys.path += [
    r'/home/ciaran/roadrunner/roadrunner/cmake-build-debug-wsl/lib/site-packages',
    # r'D:\roadrunner\roadrunner\cmake-build-release\lib\site-packages'
    # r"D:\roadrunner\roadrunner\install-msvc2019-rel\site-packages",
    # r"D:\roadrunner\roadrunner\cmake-build-release-visual-studio\lib\site-packages"
]

import pickle
import pickletools
import unittest
import roadrunner
from roadrunner._roadrunner import NamedArray
from roadrunner import Logger

print(roadrunner.__file__)


# n = NamedArray((3, 4))
# print(n)
#

class NamedArrayTests(unittest.TestCase):

    def setUp(self) -> None:
        pass

    def tearDown(self) -> None:
        pass

    @unittest.skip("as far as i can tell, equality "
                   " operators do not work with NamedArray")
    def test_equality_operators(self):
        pass

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

    def test_init_from_constructor(self):
        """

        :return:
        """
        n = NamedArray((2, 3))
        self.assertIsInstance(n, NamedArray)
        # note getrefcount itself bumps the refcount up by 1
        self.assertEqual(1, sys.getrefcount(n) - 1)
        self.assertEqual(n.rownames, [])

    def test_init_from_view_casting(self):
        arr = np.ndarray((2, 3))
        self.assertIsInstance(arr, np.ndarray)
        n = arr.view(NamedArray)
        self.assertEqual(1, sys.getrefcount(n) - 1)  # -1 for the reference used by getrefcount
        self.assertIsInstance(n, NamedArray)

    def test_init_from_template(self):
        n = NamedArray((3, 4))
        self.assertEqual(1, sys.getrefcount(n) - 1)  # -1 for the reference used by getrefcount
        v = n[1:, :]
        self.assertEqual(1, sys.getrefcount(v) - 1)  # -1 for the reference used by getrefcount
        self.assertIsInstance(n, NamedArray)
        self.assertEqual((2, 4), v.shape)

    def test_init_from_template_with_rownames(self):
        n = NamedArray((2, 3))
        n.rownames = ['R1', 'R2']
        self.assertEqual(1, sys.getrefcount(n) - 1)  # -1 for the reference used by getrefcount
        v = n[1:, :]
        self.assertEqual((1, 3), v.shape)
        self.assertEqual(['R1'], v.rownames)

    def test_init_from_template_with_rownames_ref_count(self):
        n = NamedArray((2, 3))
        n.rownames = ['R1', 'R2']
        self.assertEqual(1, sys.getrefcount(n.rownames) - 1)  # -1 for the reference used by getrefcount
        v = n[1:, :]
        self.assertEqual(1, sys.getrefcount(v.rownames) - 1)  # -1 for the reference used by getrefcount
        self.assertEqual((1, 3), v.shape)
        self.assertEqual(['R1'], v.rownames)

    def test_init_from_template_with_colnames_ref_count(self):
        n = NamedArray((2, 3))
        n.colnames = ['C1', 'C2', 'C3']
        self.assertEqual(1, sys.getrefcount(n.colnames) - 1)  # -1 for the reference used by getrefcount
        v = n[:, 1:]
        print(v)
        self.assertEqual(1, sys.getrefcount(v.colnames) - 1)  # -1 for the reference used by getrefcount
        self.assertEqual((2, 2), v.shape)
        self.assertEqual(['C1', 'C2'], v.colnames)

    def test_rownames_when_empty(self):
        n = NamedArray((2, 3))
        self.assertEqual([], n.rownames)

    def test_colnames_when_empty(self):
        n = NamedArray((2, 3))
        self.assertEqual([], n.colnames)

    def test_assigning_rownames(self):
        n = NamedArray((2, 3))
        n.rownames = ["R1", "R2"]
        print(n.rownames, ["R1", "R2"])
        self.assertEqual(n.rownames, ["R1", "R2"])

    def test_assigning_colnames(self):
        n = NamedArray((2, 3))
        n.rownames = ["C1", "C2", 'C3']
        self.assertEqual(n.rownames, ["C1", "C2", 'C3'])

    def test_rowname_refcount(self):
        n = NamedArray((2, 3))
        self.assertEqual(1, sys.getrefcount(n.rownames) - 1)  # getrefcoutn itself adds 1 to ref count

    def test_colname_refcount(self):
        n = NamedArray((2, 3))
        self.assertEqual(1, sys.getrefcount(n.colnames) - 1)  # getrefcoutn itself adds 1 to ref count

    def test_getstate_isdict(self):
        n = NamedArray((2, 3))
        self.assertIsInstance(n.__getstate__(), dict)

    def test_getstate_rownames_when_empty(self):
        n = NamedArray((2, 3))
        state = n.__getstate__()
        print(state)
        self.assertEqual(state['rownames'], [])

    def test_getstate_rownames_when_full(self):
        n = NamedArray((2, 3))
        n.rownames = [f'R{i}' for i in range(1, 4)]
        state = n.__getstate__()
        print(state)
        self.assertEqual(state['rownames'], ['R1', 'R2', 'R3'])

    def test_getstate_colnames(self):
        n = NamedArray((2, 3))
        n.colnames = [f'C{i}' for i in range(1, 5)]
        state = n.__getstate__()
        print(state['colnames'])
        self.assertEqual(state['colnames'], [f'C{i}' for i in range(1, 5)])

    def test_getstate_array(self):
        n = NamedArray((2, 3))
        state = n.__getstate__()
        self.assertIsInstance(state['array'], bytes)

    def test_reduce_callable(self):
        n = NamedArray((2, 3))
        c, args, state, _, _ = n.__reduce_ex__(5)
        self.assertTrue(callable(c))

    def test_reduce_args(self):
        n = NamedArray((2, 3))
        c, args, state, _, _ = n.__reduce_ex__(5)
        self.assertEqual(args, ((2, 3),))

    def test_reduce_state(self):
        n = NamedArray((2, 3))
        c, args, state, _, _ = n.__reduce_ex__(5)
        self.assertEqual(
            sorted(list(state.keys())),
            ['_pickle_version', 'array', 'colnames', 'dim1', 'dim2', 'nDims', 'rownames']
        )

    def test_reduce_args_work_with_callable(self):
        n = NamedArray((2, 3))
        c, args, state, _, _ = n.__reduce_ex__(5)
        initialized = c(*args)
        self.assertIsInstance(initialized, NamedArray)
        self.assertEqual(initialized.shape, (2, 3))

    def test_reduce_and_setstate(self):
        n = NamedArray((2, 3))
        c, args, state, _, _ = n.__reduce_ex__(5)
        newArray = c(*args)
        newArray.__setstate__(state)
        # make sure we still have a NamedArray after setstate
        self.assertIsInstance(newArray, NamedArray)
        self.assertEqual(newArray.shape, (2, 3))

    def test_getstate_and_setstate(self):
        n = NamedArray((2, 3))
        n[(0, 0)] = 9
        pickled = NamedArray(n.shape)
        self.assertNotEqual(id(n), id(pickled))
        state = n.__getstate__()
        self.assertIsInstance(state, dict)
        pickled.__setstate__(state)
        self.assertAlmostEqual(n[(0, 0)], 9)

    def test_pickle_dumps_no_row_or_col(self):
        n = np.zeros((2, 3)).view(NamedArray)
        self.assertIsInstance(n, NamedArray)
        b = pickle.dumps(n)
        # can't check content here, only that we have successfully
        # acquired a bytes object
        self.assertIsInstance(b, bytes)

    def test_pickle_dumps_cols(self):
        n = np.zeros((2, 3)).view(NamedArray)
        n.colnames = ['C1', 'C2', 'C3']
        self.assertIsInstance(n, NamedArray)
        b = pickle.dumps(n)
        # can't check content here, only that we have successfully
        # acquired a bytes object
        self.assertIsInstance(b, bytes)

    def test_pickle_dumps_rows(self):
        n = np.zeros((2, 3)).view(NamedArray)
        n.rownames = ['R1', 'R2']
        self.assertIsInstance(n, NamedArray)
        b = pickle.dumps(n)
        # can't check content here, only that we have successfully
        # acquired a bytes object
        self.assertIsInstance(b, bytes)

    def test_pickle_loads(self):
        n = np.zeros((2, 3)).view(NamedArray)
        b = pickle.dumps(n)
        l = pickle.loads(b)
        print(l)
