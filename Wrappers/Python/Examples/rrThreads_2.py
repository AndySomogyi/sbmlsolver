import sys
from rrPython import *
#rr = rrPython

modelFile=''
if sys.platform.startswith('win32'):
    modelFile ="r:/models/test_1.xml"
    setTempFolder('r:/temp')
else:
    modelFile = "../models/test_1.xml"
    setTempFolder('../temp')

tempFolder  = getTempFolder()
handleCount = 10
threadCount = 4

rrInstances =  createRRInstances(handleCount)

if rrInstances is None:
    print 'Allocated roadrunner instances failed.. exiting'
    sys.exit()

# Setup the instances
for index in range(getInstanceCount(rrInstances)):
    setTempFolder(tempFolder, getRRHandle(rrInstances, index)) #Note that instances can have different temp folders

# loadSBML models using threads
tpHandle = loadSBMLFromFileT(rrInstances, modelFile, threadCount);
waitForJobs(tpHandle)

nrPoints=15
print '===== Setting parameters for each individual handle ====='
for index in range(getInstanceCount(rrInstances)):
    handle = getRRHandle(rrInstances, index)
    value = getValue('k1', handle)
    setValue('k1', value/(2.5*(index + 1)), handle)
    setNumPoints(nrPoints, handle)
    setTimeEnd(150, handle)
    setTimeCourseSelectionList("S1", handle)

print '===== Simulate all instances ====='
tpHandle = simulateT(rrInstances, threadCount)
waitForJobs(tpHandle)

data = zeros((nrPoints, handleCount))
for col in range (handleCount):
    handle = getRRHandle(rrInstances, col)
    simData = getSimulationResult(handle)
    for row in range(nrPoints):
        data[row,col] = simData[row]


print data

# Write data to a file
writeRRData("../RRPythonData.dat", rrInstances);

# Free instances.. will unload any model shared libs..
freeRRInstances(rrInstances)

print 'Errors: ' + getLastError()
print "RRPython is done"
