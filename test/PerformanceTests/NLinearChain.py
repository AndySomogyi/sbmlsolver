# time how long it takes to build n linear chain models
from roadrunner import RoadRunner
import time
import numpy as np
import matplotlib.pyplot as plt
import matplotlib
import seaborn as sns
import os
import antimony
from threading import Lock

sns.set_context("talk")
matplotlib.use("TkAgg")


def updateSpecies(speciesString: str, i: int):
    if not speciesString.startswith("\t// species"):
        raise ValueError("String does not start with '// species'")
    speciesString += f"\tS{i} = 0;\n"
    return speciesString


def updateParameters(paramString: str, i: int):
    if not paramString.startswith("\t// parameters"):
        raise ValueError("String does not start with '// parameters'")
    paramString += f"\tk{i - 1} = 0.1;\n"
    return paramString


def updateReactions(reactionsString: str, i: int):
    if not reactionsString.startswith("\t// reactions"):
        raise ValueError("String does not start with '// reactions'")
    reactionsString += f"\tr{i - 1}: S{i - 1} => S{i}; k{i - 1}*S{i - 1};\n"
    return reactionsString


def generateNLinearModels(n: int):
    """Create n models which are in a linear chain. The last model
    will have n species and n-1 reactions.

    Models are in antimony format.

    :param n: How many models. Must be 2 or greater.
    :return: generator. yields a single model at a time
    """
    if n < 2:
        raise ValueError("Input must be 2 or more")
    species = "\t// species\n\tS1 = 10;\n"
    parameters = "\t// parameters\n"
    reactions = "\t// reactions\n"

    for modelID in range(2, n + 2):
        moduleName = f"linearChain{modelID}"

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

            # convert to sbml here.
            # this helps avoid timing antimony's part in
            # building a roadrunner model through tellurium
            err = antimony.loadAntimonyString(modelString)
            if err < 0:
                raise ValueError("AntimonyError: " + antimony.getLastError())

            sbml = antimony.getSBMLString(f"linearChain{modelID}")
            yield sbml


def doPlot(times: np.array, fname=os.path.join(os.path.dirname(__file__), "linearChainRoadRunnerOnly.png")):
    fig = plt.figure()
    plt.plot(range(N), times, "ro")
    plt.xlabel("n species")
    plt.ylabel("compile time (s)")
    sns.despine(fig=fig)
    plt.savefig(fname, dpi=150, bbox_inches='tight')


if __name__ == "__main__":
    # generate N models with $i \in range(N)$ species
    N = 300

    times = np.zeros((N,))
    for i, sbmlString in enumerate(generateNLinearModels(N)):
        start = time.time()
        rrModel = RoadRunner(sbmlString)
        times[i] = time.time() - start
        i += 1
        if i % 10 == 0:
            print(i, sum(times[i - 10: i]))

        if i == N - 1:
            print(sbmlString)
    total = times.sum()

    print(times)
    print(f"Took {total} seconds to build {N} models")

    doPlot(times)
