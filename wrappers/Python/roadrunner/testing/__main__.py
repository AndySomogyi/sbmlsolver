import os, sys
this_directory = os.path.dirname(__file__)
rr_site_packages_dir = os.path.dirname(this_directory)

# add local site-packages dir to path for running tests
# without installing roadrunner.
sys.path.append(rr_site_packages_dir)

test_data_dir = os.path.join(this_directory, "test_data")

import roadrunner
import tester

roadrunner.sigtrap()

tester.runTester(test_data_dir)
