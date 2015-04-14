import roadrunner
from numpy import *
from matplotlib.pyplot import *
model = open('../models/squareWaveModel.xml', 'r').read()
rr = roadrunner.RoadRunner()

rr.load(model)
timeStart = 0
timeEnd = 50
numPoints = 500
data = rr.simulate(timeStart, timeEnd, numPoints)

print data

x = data['time']
S1 = data['[S1]']

plot(x, S1, "-ro", label="S1", )

legend(bbox_to_anchor=(1.05, 1), loc=5, borderaxespad=0.)
show()