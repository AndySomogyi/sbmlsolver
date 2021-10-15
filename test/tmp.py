import sys
sys.path += [
    # r'D:\roadrunner\roadrunner\roadrunner-install-rel-llvm6\site-packages',
    r'D:\roadrunner\roadrunner\roadrunner-install-rel-llvm12\site-packages'
]
import roadrunner
from roadrunner import RoadRunner
from roadrunner.testing import TestModelFactory as tmf
import time
import numpy as np
from platform import platform
import cpuinfo # pip install py-cpuinfo

print(roadrunner.__file__)
# setup timing
start = time.time()

# get sbml to work with from one of our test modules
sbml = tmf.BatchImmigrationDeath03().str()

# create our roadrunner instance
for i in range(500):
    r = RoadRunner(sbml)


print(time.time() - start)
