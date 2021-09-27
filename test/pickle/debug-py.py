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

import roadrunner
from roadrunner import *
import copy

sys.path += [
    r'D:\roadrunner\roadrunner\cmake-build-debug\lib\site-packages'
    # r"D:\roadrunner\roadrunner\install-msvc2019-rel\site-packages",
    # r"D:\roadrunner\roadrunner\cmake-build-release-visual-studio\lib\site-packages"
]

import roadrunner.testing.TestModelFactory as tmf
from roadrunner.roadrunner import RoadRunner

rr = RoadRunner(tmf.SimpleFlux().str())
rr.setIntegrator("gillespie")
gillespie = rr.getIntegrator()
gillespie.seed = 123

data = rr.simulate(0, 10, 11)
print(data.__reduce_ex__(5))
binary = pickle.dumps(data)

print(pickletools.dis(pickle.dumps(binary, protocol=5)))
print(pickle.loads(binary))
data2 = pickle.loads(binary)
# print(type(data2))
print(data2)








