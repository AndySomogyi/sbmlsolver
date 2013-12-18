import sys;
import numpy
import os
import roadrunner
from ctypes import *
from rrPlugins import *

rrDataHandle = createRoadRunnerDataFromFile("testData.dat")
hdr = getRoadRunnerDataColumnHeader(rrDataHandle)
npData = getNumpyData(rrDataHandle)

print `hdr` + `npData`

print "done"
