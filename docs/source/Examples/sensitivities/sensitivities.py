from roadrunner import RoadRunner
from roadrunner.testing.TestModelFactory import TestModelFactory, getAvailableTestModels


# note: use getAvailableTestModels() to see options for test models.
sbml = TestModelFactory("OpenLinearFlux").str() # get the test model's sbml string

# print(sbml) # uncomment if you want to see the sbml

# Create a RoadRunner instance
model = RoadRunner(sbml)
sensSolver = model.getSensitivitySolver()

# start integration with sensitivities at 0, end at 10 and collect 11 data points
# The return type is a 4-Tuple, a time vector and a 3D sensitivity matrix with
# dimensions time x parameters x variables - that is, an array of matrices corresponding
# to the time points in the time vector with parameters down the rows and model variables (species)
# accross the columns. The third and forth elements are row and column names respectively.
time, sens, rownames, colnames = model.timeSeriesSensitivities(0, 10, 11)

print("time: ", time)
print("rownames: ", rownames)
print("colnames: ", colnames)

# Note that in this model, the parameter kb is not used in any
# rate law and therefore the sensitivities of model variables
# to kb is always 0
print(sens)

