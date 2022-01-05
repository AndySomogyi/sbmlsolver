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
            # note this might not be needed now that
            # we are freeing all models in every iteration
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

            print(modelString)

            # convert to sbml here.
            # this helps avoid timing antimony's part in
            # building a roadrunner model through tellurium

            err = antimony.loadAntimonyString(modelString)
            if err < 0:
                raise ValueError("AntimonyError: " + antimony.getLastError())

            sbml = antimony.getSBMLString(f"NReactions{int(np.floor((modelID / 2)))}")
            yield sbml
            # clear memory associated with antimony
            # or after a while it'll get clogged and we'll
            #eventially run out of memory
            antimony.clearPreviousLoads()
            modelID += 2
            model_count += 1


if __name__ == "__main__":
    # generate N models with $i \in range(N)$ uncoupled reactions
    N = 6

    for i in generateModelsWithNUncoupledReactions(N):
        print(i)
