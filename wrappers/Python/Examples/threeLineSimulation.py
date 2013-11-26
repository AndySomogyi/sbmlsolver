from rrPython import *
rrPython.loadSBMLFromFile('../models/feedback.xml')
result = rrPython.simulateEx(0.0,2.0,20)

print getNPData(result)
