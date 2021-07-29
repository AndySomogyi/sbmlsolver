############################################
# Time series sensitivities example
#

"""
In this example we show you how to use roadrunners time series sensitivities
feature. Roadrunner will integrate the model and compute sensitivities at
each time step. You can select which parameters you want sensitivities for
and you can have the return matrix (a 3D numpy.ndarray) as the kth order
derivative of the sensitivity matrix.
"""

import roadrunner
from roadrunner import RoadRunner
from roadrunner.testing.TestModelFactory import TestModelFactory, getAvailableTestModels

# For this demonstration we use a toy sbml model prepackaged with roadrunner

# note: use getAvailableTestModels() to see options for test models.
sbml = TestModelFactory("OpenLinearFlux").str() # get the test model's sbml string

# print(sbml) # uncomment if you want to see the sbml

# Create a RoadRunner instance
model = RoadRunner(sbml)

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

# By default, all model parameters are automatically selected.
# You can be more selective about which parameters to compute sensitivities for.
# This will have a performance advantage, because sensitivities are only computed
# for the parameters specified here.
time, sens, rownames, colnames = model.timeSeriesSensitivities(0, 10, 11, params=["kin"])

print("time: ", time)
print("rownames: ", rownames)
print("colnames: ", colnames)
print(sens)

# The return type, by default, returns the k=0th derivative of the
# sensitivity matrix at each time point. The kth order derivative
# be obtained instead by using the k parameter
time, sens, rownames, colnames = model.timeSeriesSensitivities(0, 10, 11, k=1)

print("time: ", time)
print("rownames: ", rownames)
print("colnames: ", colnames)
print(sens)


# sensSolver = model.getSensitivitySolver()
# sensSolver.setValue("sensitivity_method", "staggered")
# time, sens, rownames, colnames = model.timeSeriesSensitivities(0, 10, 11, k=1)
#
# print("time: ", time)
# print("rownames: ", rownames)
# print("colnames: ", colnames)
# print(sens)
