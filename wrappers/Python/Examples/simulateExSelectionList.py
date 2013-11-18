import rrPython

rrPython.loadSBMLFromFile('../models/test_1.xml')
rrPython.setSteadyStateSelectionList('time S1 S2')
results = rrPython.simulateEx(0.0,10.0,11)

print results
