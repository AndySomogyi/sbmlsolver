import rrPython

modelPath = ('../models/feedback.xml')
rrPython.loadSBMLFromFile(modelPath)
matrix = rrPython.getStoichiometryMatrix()

print matrix
