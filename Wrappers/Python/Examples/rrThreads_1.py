import sys
import rrPython
rr = rrPython

if rr.hasError():
    print 'There was an error: '
    print rr.getLastError();
    print 'Exiting...'
    exit(0)

modelFile=''
if sys.platform.startswith('win32'):
    modelFile ="r:/models/feedback.xml"
    rr.setTempFolder('r:/temp')
else:
    modelFile = "../models/test_1.xml"
    rr.setTempFolder('../temp')

print rr.getLastError()

print 'TempFolder is :' + rr.getTempFolder()

threadHandle = rr.loadSBMLFromFileThread(modelFile)
rr.waitForJob(threadHandle)

print 'Loading SBML errors:' + rr.getLastError()

threadHandle = rr.simulateThread()

rr.waitForJob(threadHandle)

print  rr.getSimulationResult()

print rr.getLastError()
print "RRPython is done"
