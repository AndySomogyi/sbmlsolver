"""
This module has testing function and test data to excecise the roadrunner
python interface.

This module can also be run direct from the command line as it is
an runnable module as::
    pythom -m "roadrunner.testing"

This module basically has a single function,::
    runTester()

This runs the built in unit tests..
"""

try:
    from tester import *
    from testfiles import *
    from SBMLTest import *
except (ImportError):
    from .tester import *
    from .testfiles import *
    from .SBMLTest import *
