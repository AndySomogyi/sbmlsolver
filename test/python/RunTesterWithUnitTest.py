"""

This is a simple mechanism to enable running the tests
that reside inside "tester.py" with normal python
unit testing frameworks.

"""

import unittest
from os.path import dirname, join, exists, abspath, realpath
import sys

tester_file = join(dirname(__file__), "test_rrtests.py")
if not exists(tester_file):
    raise FileNotFoundError(tester_file)


testing_directory = dirname(realpath(__file__))
test_data_dir = join(testing_directory, "test_data")
roadrunner_directory = dirname(testing_directory)
rr_site_packages_dir = dirname(roadrunner_directory)

# add local site-packages dir to path for running tests
# without installing roadrunner.
sys.path.append(rr_site_packages_dir)

# some debugging messages
print(f"Executing __main__.py located at \"{__file__}\"")
print("Adding roadrunner site-packages to sys.path: ", rr_site_packages_dir)

import roadrunner
try:
    import test_rrtests
except ImportError:
    from . import test_rrtests
print(f"Using roadrunner at {abspath(roadrunner.__file__)}")
print(f"Running tester at {abspath(test_rrtests.__file__)}")

class RunTesterWithUnitTest(unittest.TestCase):

    def setUp(self) -> None:
        pass

    def test_run_tester(self):
        # indicates 0 tests failed.
        # but will error if number of tests > 0 anyway
        self.assertEqual(test_rrtests.testAllRRTestFiles(), 0)
