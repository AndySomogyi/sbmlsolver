import os, sys, glob


testing_directory = os.path.dirname(os.path.realpath(__file__))
test_data_dir = os.path.join(testing_directory, "test_data")
roadrunner_directory = os.path.dirname(testing_directory)
rr_site_packages_dir = os.path.dirname(roadrunner_directory)

# add local site-packages dir to path for running tests
# without installing roadrunner.
sys.path.append(rr_site_packages_dir)

print("Adding roadrunner site-packages to sys.path: ", rr_site_packages_dir)

import roadrunner
import tester

roadrunner.sigtrap()

tester.runTester(test_data_dir)


# Run any other Python tests
import unittest

other_test_modules = [
    "python_api_tests",
    "testTestModelFactory",
    "AutomaticMoietyConservationAnalysisTests",
    "SteadyStateSolverTests",
]

for t in other_test_modules:
    unittest.main(module=t)




