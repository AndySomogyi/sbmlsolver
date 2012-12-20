import os
import rrPython

print 'RoadRunner Build Date: ' + rrPython.getCopyright()

startTime = 0
endTime = 5
numPoints = 50
selList="time,S1,S2"

rrPython.setTempFolder(".")
rrPython.enableLogging()

result = rrPython.loadSBMLFromFile("..\\Models\\test_1.xml")

if result == False:
    print "Failed loading model file.. "
    print "error was:" + rrPython.getLastError()
else:
    rrPython.setTimeStart(startTime)
    rrPython.setTimeEnd(endTime)
    rrPython.setNumPoints(numPoints)
    rrPython.setTimeCourseSelectionList(selList)
    k = rrPython.simulate()
    if k:
        print k
    else:
        print"No data to print"

print "done"
