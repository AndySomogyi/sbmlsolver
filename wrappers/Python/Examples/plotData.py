from rrPython import *
from numpy import *
from matplotlib.pyplot import *
model = open('../models/feedback.xml', 'r').read()

setTempFolder('r:/temp')
loadSBML(model)
timeStart = 0.0
timeEnd = 10.0
numPoints = 50
#resultHandle = simulateEx(timeStart, timeEnd, numPoints)

#rrcData = createRRCData(resultHandle)
#results = getNPData(rrcData)
#print results

##S1 = results[:,0]
##
##x = numpy.arange(timeStart, timeEnd, (timeEnd - timeStart)/numPoints)
##plot.plot(x, S1, label="S1")
##
##plot.legend(bbox_to_anchor=(1.05, 1), loc=5, borderaxespad=0.)
##
##plot.show()

t = arange(0.0, 2.0, 0.01)
s = sin(2*pi*t)
plot(t, s)

show()