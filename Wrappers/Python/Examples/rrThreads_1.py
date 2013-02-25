import sys
import rrPython
rr = rrPython

modelFile=''
if sys.platform.startswith('win32'):
    modelFile ="r:/models/feedback.xml"
    rr.setTempFolder('r:/rrTemp/python')
else:
    modelFile = "/home/totte/rrInstall/models/feedback.xml"
    rr.setTempFolder('/home/totte/rrTemp/python')


print 'TempFolder is :' + rr.getTempFolder()

threadHandle = rr.loadSBMLFromFileThread(modelFile)
rr.waitForJob(threadHandle)

threadHandle = rr.simulateThread()
rr.waitForJob(threadHandle)

print  rr.getSimulationResult()

print rr.getLastError()
print "RRPython is done"
