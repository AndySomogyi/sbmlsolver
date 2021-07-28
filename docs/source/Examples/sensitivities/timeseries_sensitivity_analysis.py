############################################
# Time series sensitivities example
#

"""

In this example we show you how to use roadrunners time series sensitivities
feature. Roadrunner will integrate the model and compute sensitivities at
each time step. The return type is a 2-tuple where the first element is a
1D numpy.ndarray of the integration time points and the second elment is a 3D
numpy.ndarray containing the sensitivities matrix at each time point.

"""

from roadrunner import RoadRunner
from roadrunner.testing.TestModelFactory import TestModelFactory, getAvailableTestModels

# For this demonstration we use a toy sbml model prepackaged with roadrunner

# note: use getAvailableTestModels() to see options for test models.
sbml = TestModelFactory("OpenLinearFlux").str() # get the test model's sbml string

# print(sbml) # uncomment if you want to see the sbml

# Create a RoadRunner instance
model = RoadRunner(sbml)

# start integration with sensitivities at 0, end at 10 and collect 11 data points
# The return type is a 4-Tuple,
# a time vector, a 3D sensitivity matrix (time x parameters x variables )
# rownames and column names
time, sens, rownames, colnames = model.timeSeriesSensitivities(0, 10, 11)

print(time)
print(rownames)
print(colnames)
print(sens)

# We can also be selective about which parameter to compute sensitivity for
# time, sens, rownames, colnames = model.timeSeriesSensitivities(0, 10, 11, ["kin", "kf"])
solver = model.getSensitivitySolver()
print(type(solver))

print(type(model.getIntegrator()))

