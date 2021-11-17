import sys

sys.path += [
    r"D:\roadrunner\roadrunner\cmake-build-release\lib\site-packages"
]
from roadrunner import RoadRunner
import time
import numpy as np
import matplotlib.pyplot as plt
import matplotlib
import seaborn as sns
import os
from threading import Lock
import antimony

sns.set_context("talk")
matplotlib.use("TkAgg")


def updateParameters(paramString: str, i: int, j: int):
    if not paramString.startswith("\t// parameters"):
        raise ValueError("String does not start with '// parameters'")
    paramString += f"\tk{i}_{j} = 0.1;\n"
    return paramString


def updateReactions(reactionsString: str, i: int, j: int):
    if not reactionsString.startswith("\t// reactions"):
        raise ValueError("String does not start with '// reactions'")
    reactionsString += f"\tr{i}_{j}: S{i} => S{j}; k{i}_{j}*S{i};\n"
    return reactionsString


def generateModelsWithNReactionsIncreasingConnectivity(n: int) -> str:
    """Create a model series which all have the same :param n: number of
    species but where each sequential model is increasingly connected.

    All reactions are first order mass action for simplicity.
    The first model in the series is n disconnected nodes (0 reactions).
    Each subsequent model generated adds a single reaction until all nodes
    are connected to all other nodes.

    There should be n^2 models in this series.

    :param n: The largest number of reactions.
    :return: generator. yields a single model at a time
    """
    if n < 1:
        raise ValueError("input should be 1 or greater.")

    species = "\t// species\n"
    for i in range(n):
        species += f"\tS{i} = 10;\n"

    parameters = "\t// parameters\n"
    reactions = "\t// reactions\n"

    for i in range(n):
        for j in range(n):
            # note: we include i == j deliberately.
            modelId = i * n + j
            # only allow a single thread to generate an antimony model at a time
            moduleName = f"NReactionsIncreasingConnectivity{modelId}"
            # if we've built this model before, it'll be in antimony's cache.
            if moduleName in antimony.getModuleNames():
                yield antimony.getSBMLString(moduleName)
            else:
                modelString = f"model {moduleName}\n"
                modelString += species

                parameters = updateParameters(parameters, i, j)
                modelString += parameters

                reactions = updateReactions(reactions, i, j)
                modelString += reactions
                modelString += "end"

                # print(modelString)

                # convert to sbml here.
                err = antimony.loadAntimonyString(modelString)
                if err < 0:
                    raise ValueError("AntimonyError: " + antimony.getLastError())

                sbml = antimony.getSBMLString(f"NReactionsIncreasingConnectivity{modelId}")
                yield sbml


if __name__ == "__main__":
    # generate N models with $i \in range(N)$ uncoupled reactions
    N = 10

    for i, sbml in enumerate(generateModelsWithNReactionsIncreasingConnectivity(N)):
        print(i)
    # times = np.zeros((N,))
    # for i, sbmlString in enumerate(generateModelsWithNUncoupledReactions(N)):
    #     start = time.time()
    #     rrModel = RoadRunner(sbmlString)
    #     times[i] = time.time() - start
    #     i += 1
    #     if (i % 10 == 0):
    #         print(i, sum(times[i - 10: i]))
    #
    #     if i == N - 1:
    #         print(sbmlString)
    # total = times.sum()
    #
    # print(times)
    # print(f"Took {total} seconds to build {N} models")
    #
    # doPlot(times)
