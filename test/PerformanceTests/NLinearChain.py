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

            # convert to sbml here.
            # this helps avoid timing antimony's part in
            # building a roadrunner model through tellurium
            err = antimony.loadAntimonyString(modelString)
            if err < 0:
                raise ValueError("AntimonyError: " + antimony.getLastError())

            sbml = antimony.getSBMLString(f"linearChain{modelID}")
            # clear memory associated with antimony
            # or after a while it'll get clogged and we'll
            # eventially run out of memory
            antimony.clearPreviousLoads()
            yield sbml

