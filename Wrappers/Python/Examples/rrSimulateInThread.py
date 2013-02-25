import sys
import rrPython
rr = rrPython

rri = rr.createRRInstance()

modelFile=''
if sys.platform.startswith('win32'):
    modelFile ="r:/models/feedback.xml"
    if not rr.setTempFolder('r:/rrTemp/python', rri) == True:
        print "Failed to set temp folder"
else:
    modelFile ="/home/totte/rrInstall/models/feedback.xml"
    rr.setTempFolder('/home/totte/rrTemp/python')

print 'Errors: ' + rr.getLastError()
print 'RoadRunner Version: ' + rr.getVersion()
print 'RoadRunner Build DateTime: ' + rr.getBuildDateTime()
print 'Copyright: ' + rr.getCopyright()

tempFolder = rr.getTempFolder(rri)

print 'TempFolder is :' + tempFolder

rr.setComputeAndAssignConservationLaws(True, rri)
result = rr.loadSBMLFromFile(modelFile, rri)

print 'Result of loading sbml: '
print result;

print  rr.simulate(rri)
print "done"
