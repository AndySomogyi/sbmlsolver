import sys
import rrPython
rr = rrPython

modelFile=''
tempFolder=''
if sys.platform.startswith('win32'):
    modelFile ="r:/models/feedback.xml"
    tempFolder = 'r:/rrTemp/python'
else:
    modelFile ="/home/totte/rrInstall/models/feedback.xml"
    tempFolder = '/home/totte/rrTemp/python'

print 'Errors: ' + rr.getLastError()
print 'RoadRunner Version: ' + rr.getVersion()
print 'RoadRunner Build DateTime: ' + rr.getBuildDateTime()
print 'Copyright: ' + rr.getCopyright()


handleCount = 10
threadCount = 4

rrs = rr.createRRInstances(handleCount)

if rrs is None:
    print 'Allocated roadrunner instances failed..'
else:
    print 'Allocated instances...'


#Setup the instances
for i in range(1, handleCount):
    aHandle = rr.getHandle(i, rrs)
    rr.setTempFolder(tempFolder, aHandle)
    rr.setComputeAndAssignConservationLaws(True, aHandle)

print "done"
