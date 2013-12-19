from rrPlugins import *

rrDataHandle = createRoadRunnerDataFromFile("testData.dat")
hdr = getRoadRunnerDataColumnHeader(rrDataHandle)
npData = getNumpyData(rrDataHandle)
print `hdr` + `npData`


plotRoadRunnerData(npData, hdr)

print "done"
