import sys
sys.path += [r"D:\roadrunner\roadrunner\cmake-build-release\lib\site-packages"]
import roadrunner as rr
print(rr.__file__)
from roadrunner.testing.TestModelFactory import TestModelFactory, getAvailableTestModels
import numpy as np

sbml = TestModelFactory("Venkatraman2010").str() # get the test model's sbml string
model = rr.RoadRunner(sbml)

time, sens, rownames, colnames = model.timeSeriesSensitivities(
    0, 10, 11, params=["keff1", "keff2", "keff3"], species=["tcUPA", "scUPA"])
print("time: ", time)
print("rownames: ", rownames)
print("colnames: ", colnames)
print(sens)

import matplotlib.pyplot as plt

def plot(time, sens, rownames, colnames, ncol=3):

    total = len(rownames) * len(colnames)
    print(total)
    # if ncol columns, how many rows?
    nrow = total / ncol if total % ncol == 0 else np.ceil(total/ncol)
    nrow = int(nrow)

    print(ncol, nrow)

    fig, ax = plt.subplots()


plot(time, sens, rownames, colnames)



