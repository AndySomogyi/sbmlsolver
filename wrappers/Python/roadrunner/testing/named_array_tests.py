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

    def test_init_from_constructor(self):
        """

        :return:
        """
        n = NamedArray((2, 3))
        self.assertIsInstance(n, NamedArray)
        # note getrefcount itself bumps the refcount up by 1
        self.assertEqual(1, sys.getrefcount(n) - 1 )
        self.assertEqual(n.rownames, [])

    def test_init_from_view_casting(self):
        arr = np.ndarray((2, 3))
        self.assertIsInstance(arr, np.ndarray)
        n = arr.view(NamedArray)
        self.assertEqual(1, sys.getrefcount(n) - 1) # -1 for the reference used by getrefcount
        self.assertIsInstance(n, NamedArray)

    def test_init_from_template(self):
        n = NamedArray((3, 4))
        self.assertEqual(1, sys.getrefcount(n) - 1) # -1 for the reference used by getrefcount
        v = n[1:,:]
        self.assertEqual(1, sys.getrefcount(v) - 1) # -1 for the reference used by getrefcount
        self.assertIsInstance(n, NamedArray)
        self.assertEqual((2, 4), v.shape)

    def test_init_from_template_with_rownames(self):
        n = NamedArray((2, 3))
        n.rownames = ['R1', 'R2']
        self.assertEqual(1, sys.getrefcount(n) - 1) # -1 for the reference used by getrefcount
        v = n[1:,:]
        self.assertEqual((1, 3), v.shape)
        self.assertEqual(['R1'], v.rownames)

    def test_init_from_template_with_rownames_ref_count(self):
        n = NamedArray((2, 3))
        n.rownames = ['R1', 'R2']
        self.assertEqual(1, sys.getrefcount(n.rownames) - 1) # -1 for the reference used by getrefcount
        v = n[1:,:]
        self.assertEqual(1, sys.getrefcount(v.rownames) - 1) # -1 for the reference used by getrefcount
        self.assertEqual((1, 3), v.shape)
        self.assertEqual(['R1'], v.rownames)

    def test_init_from_template_with_colnames(self):
        n = NamedArray((2, 3))
        n.rownames = ['C1', 'C2', 'C3']
        self.assertEqual(1, sys.getrefcount(n) - 1) # -1 for the reference used by getrefcount
        v = n[:,1:]
        self.assertEqual((2, 2), v.shape)
        self.assertEqual(['C2', 'C3'], v.colnames)

    def test_init_from_template_with_colnames_ref_count(self):
        n = NamedArray((2, 3))
        n.colnames = ['C1', 'C2', 'C3']
        self.assertEqual(1, sys.getrefcount(n.colnames) - 1) # -1 for the reference used by getrefcount
        v = n[:,1:]
        print(v)
        self.assertEqual(1, sys.getrefcount(v.colnames) - 1) # -1 for the reference used by getrefcount
        self.assertEqual((2, 2), v.shape)
        self.assertEqual(['C2', 'C3'], v.colnames)

    def test_rownames_when_empty(self):
        n = NamedArray((2, 3))
        self.assertEqual([], n.rownames)

    def test_colnames_when_empty(self):
        n = NamedArray((2, 3))
        self.assertEqual([], n.colnames)

    @unittest.skip("The expected behaviour of returning an "
                   "empty list is not yet implemented. This test "
                   "is left, but skipped for when roadrunner "
                   "developers have time to implement it")
    def test_rownames_in_constructor(self):
        # rownames in NamedArray constructor not supported
        n = NamedArray((2, 3), rownames=["R1", "R2"])

    @unittest.skip("The expected behaviour of returning an "
                   "empty list is not yet implemented. This test "
                   "is left, but skipped for when roadrunner "
                   "developers have time to implement it")
    def test_colnames_in_constructor(self):
        # colnames in NamedArray constructor not supported
        n = NamedArray((2, 3), colnames=["R1", "R2"])

    def test_assigning_rownames(self):
        n = NamedArray((2, 3))
        n.rownames = ["R1", "R2"]
        self.assertEqual(n.rownames, ["R1", "R2"])

    def test_assigning_colnames(self):
        n = NamedArray((2, 3))
        n.rownames = ["C1", "C2", 'C3']
        self.assertEqual(n.rownames, ["C1", "C2", 'C3'])

    def test_rowname_refcount(self):
        n = NamedArray((2, 3))
        self.assertEqual(1, sys.getrefcount(n.rownames) - 1) # getrefcoutn itself adds 1 to ref count

    def test_colname_refcount(self):
        n = NamedArray((2, 3))
        self.assertEqual(1, sys.getrefcount(n.colnames) - 1) # getrefcoutn itself adds 1 to ref count

    def test_pickle_dumps_no_row_or_col(self):
        n = np.zeros((2, 3)).view(NamedArray)
        self.assertIsInstance(n, NamedArray)
        b = pickle.dumps(n)
        # can't check content here, only that we have successfully
        # acquired a bytes object
        self.assertIsInstance(b, bytes)


    def test_pickle_dumps_no_rows(self):
        n = np.zeros((2, 3)).view(NamedArray)
        n.colnames = ['C1', 'C2', 'C3']
        self.assertIsInstance(n, NamedArray)
        b = pickle.dumps(n)
        # can't check content here, only that we have successfully
        # acquired a bytes object
        self.assertIsInstance(b, bytes)


    def test_pickle_dumps_no_cols(self):
        n = np.zeros((2, 3)).view(NamedArray)
        n.rownames = ['R1', 'R2']
        self.assertIsInstance(n, NamedArray)
        b = pickle.dumps(n)
        # can't check content here, only that we have successfully
        # acquired a bytes object
        self.assertIsInstance(b, bytes)





































