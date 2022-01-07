"""
This module has testing function and test data to excecise the roadrunner
python interface.

This module can also be run direct from the command line as it is
an runnable module as::
    pythom -m "roadrunner.tests"

This module basically has a single function,::
    runTester()

This runs the built in unit tests..
"""

try:
    from test_rrtests import *
    from testfiles import *
    from SBMLTest import *
except ImportError:
    try:
        from .test_rrtests import *
        from .testfiles import *
        from .SBMLTest import *
    except Exception as e:
        # this allows us to run the Python tests from the source tree,
        # after having first provided the path to the build tree site-packages
        print("Failed to import tests.")
        print(str(e))
