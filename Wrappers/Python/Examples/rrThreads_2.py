import sys
from rrPython import *
#rr = rrPython

modelFile=''
if sys.platform.startswith('win32'):
    modelFile ="r:/models/test_1.xml"
    setTempFolder('r:/rrTemp/python')
else:
    modelFile = "/home/totte/rrInstall/models/test_1.xml"
    setTempFolder('/home/totte/rrTemp/python')

tempFolder  = getTempFolder()
handleCount = 100
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

print '===== Setting parameters for each individual handle ====='
for index in range(getInstanceCount(rrInstances)):
    handle = getRRHandle(rrInstances, index)
    value = getValue('k1', handle)
    setValue('k1', value/(2.5*(index + 1)), handle)
    setNumPoints(500, handle)
    setTimeEnd(150, handle)
    setTimeCourseSelectionList("S1", handle)

print '===== Simulate all instances ====='
tpHandle = simulateT(rrInstances, threadCount)
waitForJobs(tpHandle)

# Write data to a file
writeRRData("r:\\allDataPython.dat", rrInstances);

# Free instances.. will unload any model shared libs..
freeRRInstances(rrInstances)

print 'Errors: ' + getLastError()
print "RRPython is done"
