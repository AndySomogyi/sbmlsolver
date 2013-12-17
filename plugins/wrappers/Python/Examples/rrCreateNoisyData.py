import sys;
import numpy
import os
import roadrunner
from ctypes import *
from rrPlugins import *

#Create a plugin manager
pm = createPluginManager()

#Load the 'noise' plugin in order to add some noise to roadrunner data later on
noisePlugin = loadPlugin(pm, "rrp_add_noise")

#Create a roadrunner instance
rr = roadrunner.RoadRunner()
rr.load("../../models/sbml_test_0001.xml")

#Generate data
rr.simulate(0, 10, 511) #Want 512 points

#The plugin will need a handle to the underlying roadrunner data
rrDataHandle = getRoadRunnerDataHandle(rr)
assignPluginInput(noisePlugin, rrDataHandle)

#get parameter for the 'size' of the noise
setPluginParameter(noisePlugin, "Sigma", "1.e-3")

#Execute the noise plugin which will add some noise to the (internal) data
executePlugin(noisePlugin)

writeRoadRunnerData(rrDataHandle, "testData.dat")
print "done"
