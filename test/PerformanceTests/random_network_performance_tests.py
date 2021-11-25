import sys

import pandas as pd

sys.path += [
    r"D:\roadrunner\roadrunner\cmake-build-release\lib\site-packages"
]

from roadrunner import RoadRunner, Config
import roadrunner
import time
import numpy as np
import matplotlib.pyplot as plt
import matplotlib
import seaborn as sns
import os
import antimony
import glob
import zipfile
import json

# constants (not parameters)
JIT_ENGINE_LIST = ['MCJit', "LLJit"]
KINETICS_TYPES = ['MassAction', "Hanekom"]
MODEL_SIZES = ["Small", "Medium", "Large"]
MODEL_REPEATS = [0, 1, 2]
NANOSECOND_TO_MILLISECOND_CONVERSION_FACTOR = 1000000
NANOSECOND_TO_SECOND_CONVERSION_FACTOR = 100000000


def _getModelDirectory(kinetics:str, modelSize: str):
    if kinetics not in KINETICS_TYPES:
        raise ValueError(f"kinetics: {kinetics} not valid. Choose from {KINETICS_TYPES}")
    current_directory = os.path.dirname(__file__)
    models_directory = os.path.join(current_directory, "models")
    if not os.path.isdir(models_directory):
        raise NotADirectoryError(models_directory)

    small_directory = os.path.join(models_directory, f"Small{kinetics}Networks")
    medium_directory = os.path.join(models_directory, f"Medium{kinetics}Networks")
    large_directory = os.path.join(models_directory, f"Large{kinetics}Networks")

    if modelSize == "Small":
        models_directory = small_directory
    elif modelSize == "Medium":
        models_directory = medium_directory
    elif modelSize == "Large":
        models_directory = large_directory
    else:
        raise ValueError(f"modelSize input {modelSize} is invalid")
    return models_directory


def getSBMLFile(kinetics:str, modelSize: str, repeat: int) -> str:
    """load an sbml string from the models folder

    Args:
        kinetics: Either "MassAction" or "Hanekom"
        modelSize: One of "small", "medium" or "large"
        repeat: One of 1, 2 or 3

    """
    models_directory = _getModelDirectory(kinetics, modelSize)

    if repeat < 0 or repeat > 2:
        raise ValueError("Repeat should be 0, 1 or 2")

    sbml_file = os.path.join(models_directory, f"{repeat}.sbml")
    if not os.path.isfile(sbml_file):
        raise ValueError(f"sbml file not found at {sbml_file}")
    return sbml_file


def getSettingsFile() -> str:
    """Returns the file that stores settings"""
    current_directory = os.path.dirname(__file__)
    models_directory = os.path.join(current_directory, "models")
    if not os.path.isdir(models_directory):
        raise NotADirectoryError(models_directory)
    settings_file = os.path.join(models_directory, "settings.json")
    if not os.path.isfile(settings_file):
        raise ValueError("settings file not found: {}".format(settings_file))
    return settings_file


def getSettings() -> dict:
    """Load settings file into nested dict"""
    settings_file = getSettingsFile()
    with open(settings_file, 'r') as f:
        settings = json.load(f)
    return settings


def getResultsFile(kinetics:str, modelSize: str, repeat: int, numSimSteps: int, numRepeatSteps: int, run_label:str) -> str:
    """returns npy file produced by a simulation run. Throws if not exist"""
    which_models_directory = _getModelDirectory(kinetics, modelSize)
    models_directory = os.path.dirname(which_models_directory)
    performance_tests_directory = os.path.dirname(models_directory)
    simulations_directory = os.path.join(performance_tests_directory, "simulations")
    run_id_directory = os.path.join(simulations_directory, run_label)
    kinetics_size_directory = os.path.join(run_id_directory, f"{modelSize}{kinetics}Networks")
    results_directory = kinetics_size_directory
    if not os.path.exists(results_directory):
        os.makedirs(results_directory)
    fname = os.path.join(results_directory, f"{repeat}_Kinetics{kinetics}_numSteps{numSimSteps}_numRepeats{numRepeatSteps}.npy")
    return fname


def timeModelSimulation(sbml: str, simEndTime: float, numSimSteps=100, numRepeats=1000):
    """Time a model simulation

    Args:
        sbml: file containing sbml to run
        simEndTime: simulation end time
        numSimSteps: how many time steps to simulate
        numRepeats: How many times to measure simulation time?

    """
    rr = RoadRunner(sbml)

    times = np.zeros((numRepeats,))

    for i in range(numRepeats):
        # print(i)
        rr.resetAll()
        startTime = time.time_ns()
        data = rr.simulate(0, simEndTime, numSimSteps)
        times[i] = time.time_ns() - startTime

    # models_directory = _getModelDirectory(kinetics, modelSize, repeat)
    fname = getResultsFile(kinetics, modelSize, repeat, numSimSteps, numRepeats, RUN_LABEL)
    print(f"results saved to {fname}")
    np.save(fname, times)

    return times


def collectPerformanceData(numSimSteps: int, numRepeats: int, run_label:str):
    """Collect simulation times from npy files"""
    results = dict()
    for kinetics in KINETICS_TYPES:
        results[kinetics] = dict()
        for modelSize in MODEL_SIZES:
            results[kinetics][modelSize] = dict()
            for repeat in MODEL_REPEATS:
                resultsFile = getResultsFile(kinetics, modelSize, repeat, NUM_SIM_STEPS, NUM_REPEAT_SIMS, run_label)
                times = np.load(resultsFile) / NANOSECOND_TO_MILLISECOND_CONVERSION_FACTOR
                results[kinetics][modelSize][repeat] = pd.Series(times)
                results[kinetics][modelSize][repeat].name = repeat
            results[kinetics][modelSize] = pd.concat(results[kinetics][modelSize])
        results[kinetics] = pd.concat(results[kinetics])

    df = pd.concat(results, axis=1)
    df.index.names = ["Model Size", "Model ID", "Simulation Repeat"]
    df.columns.name = "kinetics type"
    # df = df.stack(level=0)
    # print(df)
    return df


########################################################333
#   Script constants (user options)
#

RUN_LABEL: str = "LLJit_AggressiveOpt"

# how many time steps to simulate
NUM_SIM_STEPS: int = 50000

# how many repeat simualtions to run
NUM_REPEAT_SIMS: int = 50

# run the simulations. Saves results to file
RUN_SIMULATIONS: bool = True

#
PLOT_SIMULATIONS = True

# set LLVM's LLJIT optimization level
LLJIT_OPTIMIZATION = Config.AGGRESSIVE

# set LLVM's LLJIT num threads
# use None to use the default, which is the
# maximum number of detected cores (hyperthreadded)
LLJIT_NUM_THREADS = 1

# choose backend - either LLJit or MCJit
JIT_ENGINE = "LLJit"

if __name__ == "__main__":

    ########################################################
    #   Work is done here
    #

    # set backend
    if JIT_ENGINE == "LLJit":
        print("Using LLJit")
        Config.setValue(Config.LLVM_BACKEND, Config.LLJIT)
    elif JIT_ENGINE == "MCJit":
        print("Using MCJit")
        Config.setValue(Config.LLVM_BACKEND, Config.MCJIT)
    else:
        raise ValueError(f"No JitEngine called {JIT_ENGINE}")

    # set the LLJit Optimization level
    Config.setValue(Config.LLJIT_OPTIMIZATION_LEVEL, LLJIT_OPTIMIZATION)

    # set the LLJit number of threads
    if LLJIT_NUM_THREADS:
        # otherwise use the default (hardware concurrency)
        Config.setValue(Config.LLJIT_NUM_THREADS, LLJIT_NUM_THREADS)

    settings = getSettings()

    if RUN_SIMULATIONS:
        for kinetics in KINETICS_TYPES:
            for modelSize in MODEL_SIZES:
                for repeat in MODEL_REPEATS:
                    print("running kinetics: ", kinetics, "model size: ", modelSize, "repeat", repeat)
                    sbmlFile = getSBMLFile(kinetics, modelSize, repeat)
                    timeModelSimulation(sbmlFile, settings[kinetics][modelSize][str(repeat)]["approx_time_at_steady_state"],
                                        numSimSteps=NUM_SIM_STEPS, numRepeats=NUM_REPEAT_SIMS)

    if PLOT_SIMULATIONS:
        df = collectPerformanceData(NUM_SIM_STEPS, NUM_REPEAT_SIMS, RUN_LABEL)
        df.to_csv(r"D:\roadrunner\roadrunner\test\PerformanceTests\data.csv")


        print(df)
