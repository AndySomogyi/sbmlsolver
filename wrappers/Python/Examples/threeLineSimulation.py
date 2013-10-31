import rrPython
rrPython.loadSBMLFromFile('..\\models\\feedback.xml')
results = rrPython.simulateEx(0.0,2.0,20)

print results
