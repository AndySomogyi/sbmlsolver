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


def updateSpecies(speciesString: str, i: int):
    if not speciesString.startswith("\t// species"):
        raise ValueError("String does not start with '// species'")
    speciesString += f"\tS{i} = 10;\n\tS{i + 1} = 10;\n"
    return speciesString


def updateParameters(paramString: str, i: int):
    if not paramString.startswith("\t// parameters"):
        raise ValueError("String does not start with '// parameters'")
    paramString += f"\tk{int(np.floor((i / 2)))} = 0.1;\n"
    return paramString


def updateReactions(reactionsString: str, i: int):
    if not reactionsString.startswith("\t// reactions"):
        raise ValueError("String does not start with '// reactions'")
    reactionsString += f"\tr{int(np.floor((i / 2)))}: S{i} => S{i + 1}; k{int(np.floor((i / 2)))}*S{i};\n"
    return reactionsString


def generateModelsWithNUncoupledReactions(n: int) -> str:
    """Create a model series which each have $i \in n$
    independent, uncoupled reactions.

    Models are in antimony format.

    :param n: The largest number of reactions.
    :return: generator. yields a single model at a time
    """
    if n < 1:
        raise ValueError("input should be 1 or greater.")

    species = "\t// species\n"
    parameters = "\t// parameters\n"
    reactions = "\t// reactions\n"

    model_count = 1
    modelID = 0
    while model_count <= n:
        # only allow a single thread to generate an antimony model at a time
        moduleName = f"NReactions{int(np.floor((modelID / 2)))}"
        # if we've built this model before, it'll be in antimony's cache.
        if moduleName in antimony.getModuleNames():
            yield antimony.getSBMLString(moduleName)
        else:
            modelString = f"model {moduleName}\n"
            species = updateSpecies(species, modelID)
            modelString += species

            parameters = updateParameters(parameters, modelID)
            modelString += parameters

            reactions = updateReactions(reactions, modelID)
            modelString += reactions
            modelString += "end"

            # print(modelString)

            # convert to sbml here.
            # this helps avoid timing antimony's part in
            # building a roadrunner model through tellurium

            err = antimony.loadAntimonyString(modelString)
            if err < 0:
                raise ValueError("AntimonyError: " + antimony.getLastError())

            sbml = antimony.getSBMLString(f"NReactions{int(np.floor((modelID / 2)))}")
            yield sbml
            modelID += 2
            model_count += 1


def doPlot(times: np.array, fname=os.path.join(os.path.dirname(__file__), "linearChainRoadRunnerOnly.png")):
    fig = plt.figure()
    plt.plot(range(len(times)), times, "ro")
    plt.xlabel("n species")
    plt.ylabel("compile time (s)")
    sns.despine(fig=fig)
    plt.savefig(fname, dpi=150, bbox_inches='tight')


if __name__ == "__main__":
    # generate N models with $i \in range(N)$ uncoupled reactions
    N = 6

    times = np.zeros((N,))
    for i, sbmlString in enumerate(generateModelsWithNUncoupledReactions(N)):
        start = time.time()
        rrModel = RoadRunner(sbmlString)
        times[i] = time.time() - start
        i += 1
        if (i % 10 == 0):
            print(i, sum(times[i - 10: i]))

        if i == N - 1:
            print(sbmlString)
    total = times.sum()

    print(times)
    print(f"Took {total} seconds to build {N} models")

    doPlot(times)
