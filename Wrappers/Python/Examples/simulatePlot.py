#from scipy import *

import numpy
import pylab
import matplotlib
import rrPython
#import matplotlib.pyplot as plot
modelFile='R:\\models\\feedback.xml'
#model = open(modelFile, 'r').read()
rrPython.loadSBMLFromFile(modelFile)
timeStart = 0.0
timeEnd = 10.0
numPoints = 50
#results = rrPython.simulateEx(timeStart, timeEnd, numPoints)
#print results

#S1 = results[:,2]
#S2 = results[:,3]
#S3 = results[:,4]
#x = numpy.arange(timeStart, timeEnd, (timeEnd - timeStart)/numPoints)

#pylab.plot(x, S1)
#plot(x, S2, label="S2")
#plot(x, S3, label="S3")
#legend(bbox_to_anchor=(1.05, 1), loc=5, borderaxespad=0.)
#ylabel('Concentration (moles/L)')
#xlabel('time (s)')

#pylab.plot.show()

t = numpy.arange(0.0, 1.0+0.01, 0.01)
s = numpy.cos(2*2*numpy.pi*t)
pylab.plot(t, s)

pylab.xlabel('time (s)')
pylab.ylabel('voltage (mV)')
pylab.title('About as simple as it gets, folks')
pylab.grid(True)
pylab.savefig('simple_plot')

pylab.show()
