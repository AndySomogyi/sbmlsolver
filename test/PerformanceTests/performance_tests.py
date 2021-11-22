# Run some performance tests for roadrunner
import sys
from typing import Optional

sys.path += [
    r"D:\roadrunner\roadrunner\cmake-build-release\lib\site-packages"
]
from typing import List
import os
import pickle
import roadrunner
from roadrunner import RoadRunner, Config
import NLinearChain
import NReactionsUncoupled
import NSpeciesIncreasingConnectivity
import build_biomodels
import numpy as np
import time
import matplotlib.pyplot as plt
import seaborn as sns
import multiprocessing as mp
from multiprocessing.managers import SyncManager

print(roadrunner.__file__)


def progressBarLinear(i: int, N: int, name: str):
    """Prints percentage completed when there are N items and we
    have processed i of them.

    Use this progress bar when the number of models scales linearly
    with N... aka $i \in {0 .. N}$

    :param i: int < N current iteration number
    :param N: int total number of expected items.
    :param name: identifier for task being tracked
    """
    if i % 10 == 0:
        print(f"{name}".ljust(60), f"-- {(i / N * 100)}% completed")


def progressBarQuadratic(i: int, N: int, name: str):
    """Prints percentage completed when there are N*N items and we
    have processed i of them.

    Use this progress bar when the number of models scales quadratically
    with N... aka $i \in {0 .. N*N}$

    :param i: int < N current iteration number
    :param N: int sqrt(total number of expected items).
    :param name: identifier for task being tracked

    """
    if i % 10 == 0:
        print(f"{name}".ljust(60), f"-- {(i / (N * N) * 100)}% completed")

def loadRoadrunnerFromSBMLString(sbml:str):
    try:
        rrModel = RoadRunner(sbml)
        return rrModel
    except RuntimeError as e:
        # delay differential equations
        print(e)
    except UnicodeDecodeError as e:
        # UnicodeDecodeError: 'charmap' codec can't decode byte
        # 0x9d in position 39237: character maps to <undefined>
        print(e)

def build_time(sbmlGeneratorFunction: callable, N: int, results_key: str, results: SyncManager.dict,
               progressBar=progressBarLinear) -> List[float]:
    """Measure time it takes to build roadrunner models sequence produced
    by the callable sbmlGeneratorFunction

    :param sbmlGeneratorFunction: a function that yields an sbml string.
    The signature must be "def exampleGenerator(N: int) -> str" where
    N encodes the number of models to generate (though may actually represent
    different quantities in different situations).
    :param N: is passed on to sbmlGeneratorFunction
    """
    times = []
    for i, sbmlString in enumerate(sbmlGeneratorFunction(N)):
        start = time.time()
        rrModel = loadRoadrunnerFromSBMLString(sbmlString)
        times.append(time.time() - start)
        progressBar(i, N, results_key)
        del sbmlString
    results[results_key] = times
    return times


def build_and_sim_time(sbmlGeneratorFunction: callable, N: int, results_key: str,
                       results: SyncManager.dict, progressBar=progressBarLinear) -> List[float]:
    """Measure time it takes to build roadrunner models sequence produced
    by the callable sbmlGeneratorFunction and run a 1000 time step simulation.

    :param sbmlGeneratorFunction: a function that yields an sbml string.
    The signature must be "def exampleGenerator(N: int) -> str" where
    N encodes the number of models to generate (though may actually represent
    different quantities in different situations).
    :param N: is passed on to sbmlGeneratorFunction
    """
    times = []
    for i, sbmlString in enumerate(sbmlGeneratorFunction(N)):
        start = time.time()
        rrModel = loadRoadrunnerFromSBMLString(sbmlString)
        rrModel.simulate(0, 1000, 1001)
        times.append(time.time() - start)
        progressBar(i, N, results_key)
        del sbmlString
    results[results_key] = times
    return times


def sim_time(sbmlGeneratorFunction: callable, N: int, results_key: str, results: SyncManager.dict,
             progressBar=progressBarLinear) -> List[float]:
    """Measure time it takes to simulate a roadrunner model sequence produced
    by the callable sbmlGeneratorFunction.

    Obviously, we still need to build a roadrunner model before it can be
    simulated, but this function isolates and only times the simulation part.

    :param sbmlGeneratorFunction: a function that yields an sbml string.
    The signature must be "def exampleGenerator(N: int) -> str" where
    N encodes the number of models to generate (though may actually represent
    different quantities in different situations).
    :param N: is passed on to sbmlGeneratorFunction
    """
    times = []
    for i, sbmlString in enumerate(sbmlGeneratorFunction(N)):
        rrModel = loadRoadrunnerFromSBMLString(sbmlString)
        start = time.time()
        rrModel.simulate(0, 1000, 1001)
        times.append(time.time() - start)
        progressBar(i, N, results_key)
        del sbmlString
    results[results_key] = times
    return times


def plot(results: dict, tag: Optional[str] = None, outputDir: Optional[str] = None):
    if not outputDir:
        outputDir = os.path.dirname(__file__)

    def plotResults(times: np.ndarray, fname: str, xlabel: str, ylabel: str, title: str):
        fig = plt.figure()
        plt.plot(range(len(times)), times, "ro")
        plt.xlabel(xlabel)
        plt.ylabel(ylabel)
        plt.title(title)
        sns.despine(fig=fig)
        fname = os.path.join(outputDir, f"{fname}_{tag}.png")
        plt.savefig(fname, dpi=150, bbox_inches='tight')

    def plot_linear_chain_build_time_results(times: np.ndarray):
        fname = "NLinearChainBuildTimes"
        plotResults(times, fname, "n species", "build time (s)", "NLinearChain")

    def plot_linear_chain_build_and_sim_time_results(times: np.ndarray):
        fname = "NLinearChainBuildAndSimTimes"
        plotResults(times, fname, "n species", "build and sim time (s)", "NLinearChain")

    def plot_linear_chain_time_results(times: np.ndarray):
        fname = "NLinearChainSimTimes"
        plotResults(times, fname, "n species", "sim time (s)", "NLinearChain")

    def plot_n_reactions_build_time_results(times: np.ndarray):
        fname = "NReactionsUncoupledBuildTimes"
        plotResults(times, fname, "n reactions", "build time (s)", "NReactions (uncoupled)")

    def plot_n_reactions_build_and_sim_time_results(times: np.ndarray):
        fname = "NReactionsUncoupledBuildAndSimTimes"
        plotResults(times, fname, "n reactions", "build and sim time (s)", "NReactions (uncoupled)")

    def plot_n_reactions_time_results(times: np.ndarray):
        fname = "NReactionsUncoupledSimTimes"
        plotResults(times, fname, "n reactions", "sim time (s)", "NReactions (uncoupled)")

    def plot_n_reactions_increasing_connectivity_build_time_results(times: np.ndarray):
        fname = "NReactionsIncreasingConnectivityBuildTimes"
        plotResults(times, fname, "n reactions", "build time (s)", "NReactions (Increasing Connectivity)")

    def plot_n_reactions_increasing_connectivity_build_and_sim_time_results(times: np.ndarray):
        fname = "NReactionsIncreasingConnectivityBuildAndSimTimes"
        plotResults(times, fname, "n reactions", "build and sim time (s)", "NReactions (Increasing Connectivity)")

    def plot_n_reactions_increasing_connectivity_time_results(times: np.ndarray):
        fname = "NReactionsIncreasingConnectivitySimTimes"
        plotResults(times, fname, "n reactions", "sim time (s)", "NReactions (Increasing Connectivity)")

    plot_linear_chain_build_time_results(results["NLinearChain_BuildTime"])
    plot_linear_chain_build_and_sim_time_results(results["NLinearChain_BuildAndSimTime"])
    plot_linear_chain_time_results(results["NLinearChain_SimTime"])

    plot_n_reactions_build_time_results(results["NReactionsUncoupled_BuildTime"])
    plot_n_reactions_build_and_sim_time_results(results["NReactionsUncoupled_BuildAndSimTime"])
    plot_n_reactions_time_results(results["NReactionsUncoupled_SimTime"])

    plot_n_reactions_increasing_connectivity_build_time_results(results["NReactionsIncreasingConnectivity_BuildTime"])
    plot_n_reactions_increasing_connectivity_build_and_sim_time_results(
        results["NReactionsIncreasingConnectivity_BuildAndSimTime"])
    plot_n_reactions_increasing_connectivity_time_results(results["NReactionsIncreasingConnectivity_SimTime"])


if __name__ == "__main__":
    ##############################################################
    #   Script options. Could use argparse to turn this into command line.
    #

    # when present, labels output graphs with a tag to identify them
    #  Keep the tags unique and make sure they are descriptive.
    RUN_TAG: str = "BuildBiomodels_LLJit_AggressiveOpt_16Thread"

    # pick which tests to run (all selected will be run in parallel)
    RUN_NSPECIES_INCREASING_CONNECTIVITY = False
    RUN_NLINEAR_CHAIN = False
    RUN_NREACTIONS_UNCOUPLED = False
    RUN_BUILD_BIOMODELS = True

    # set LLVM's LLJIT optimization level
    LLJIT_OPTIMIZATION = Config.AGGRESSIVE

    # set LLVM's LLJIT num threads
    # use None to use the default, which is the
    # maximum number of detected cores (hyperthreadded)
    LLJIT_NUM_THREADS = None

    # Plot results
    PLOT_RESULTS = False

    # Plot a bar graph using all the pickle data that can be found in subdirectories
    # Value plotted is sum of times in conditions
    PLOT_BARGRAPH = True

    # choose backend - either LLJit or MCJit
    JIT_ENGINE = "LLJit"
    JIT_ENGINE_LIST = ['MCJit', "LLJit"]
    if JIT_ENGINE not in JIT_ENGINE_LIST:
        raise ValueError(JIT_ENGINE)

    # output directory default to current directory
    # if RUN_TAG is not "" then OUTPUT_DIR becomes current_directory/RUN_TAG
    OUTPUT_DIR = os.path.dirname(__file__)
    if RUN_TAG != "":
        OUTPUT_DIR = os.path.join(os.path.dirname(__file__), RUN_TAG)
        if not os.path.exists(OUTPUT_DIR):
            os.makedirs(OUTPUT_DIR)

    # Where to store the results once the script has been executed
    RESULTS_PICKLE_FILE = os.path.join(OUTPUT_DIR, f"performance_test_results_{RUN_TAG}.pickle")

    # When true, pickled simulation results are be overwritten
    # When false, and if the RESULTS_PICKLE_FILE exists, read from the pickle
    USE_PICKLED_RESULTS = False

    # The N parameter for the model generator functions above.
    # Represents size of the problem series that we are timing.
    N = 600

    ###############################################################
    # Implement script options
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

    # To collect results from threads we need a shared dict
    # which is provided by the mp.Manager.
    manager = mp.Manager()

    # this effectively becomes an "out" parameter
    results = manager.dict()

    # add tag to pickle results file
    if RESULTS_PICKLE_FILE.endswith(".pickle"):
        RESULTS_PICKLE_FILE = RESULTS_PICKLE_FILE[:-7]
        RESULTS_PICKLE_FILE = f"{RESULTS_PICKLE_FILE}.pickle"

    if USE_PICKLED_RESULTS and os.path.exists(RESULTS_PICKLE_FILE):
        with open(RESULTS_PICKLE_FILE, 'rb') as f:
            results = pickle.load(f)
    else:

        # Run performance tests in separate processes.
        processes = []

        if RUN_NREACTIONS_UNCOUPLED:
            print("Running NReactionsUncoupled build time")
            p1 = mp.Process(target=build_time, args=(
                NReactionsUncoupled.generateModelsWithNUncoupledReactions,
                N,
                f"NReactionsUncoupled_BuildTime",
                results))
            p1.start()
            processes.append(p1)

            print("Running NReactionsUncoupled build and sim time")
            p2 = mp.Process(
                target=build_and_sim_time,
                args=(
                    NReactionsUncoupled.generateModelsWithNUncoupledReactions,
                    N,
                    "NReactionsUncoupled_BuildAndSimTime",
                    results)
            )
            p2.start()
            processes.append(p2)

            print("Running NReactionsUncoupled sim time")
            p3 = mp.Process(
                target=sim_time,
                args=(
                    NReactionsUncoupled.generateModelsWithNUncoupledReactions,
                    N,
                    "NReactionsUncoupled_SimTime",
                    results)
            )
            p3.start()
            processes.append(p3)

        if RUN_NLINEAR_CHAIN:
            print("Running NLinearChain build time")
            p4 = mp.Process(
                target=build_time,
                args=(NLinearChain.generateNLinearModels,
                      N, "NLinearChain_BuildTime", results)
            )
            p4.start()
            processes.append(p4)

            print("Running NLinearChain build and sim time")
            p5 = mp.Process(
                target=build_and_sim_time,
                args=(NLinearChain.generateNLinearModels, N,
                      "NLinearChain_BuildAndSimTime", results))
            p5.start()
            processes.append(p5)

            print("Running NLinearChain sim time")
            p6 = mp.Process(
                target=sim_time,
                args=(NLinearChain.generateNLinearModels, N, "NLinearChain_SimTime", results))
            p6.start()
            processes.append(p6)

        if RUN_NSPECIES_INCREASING_CONNECTIVITY:
            print("Running NSpeciesIncreasingConnectivity build time")
            p7 = mp.Process(
                target=build_time,
                args=(NSpeciesIncreasingConnectivity.generateModelsWithNSpeciesIncreasingConnectivity,
                      int(np.floor(np.sqrt(N))), "NReactionsIncreasingConnectivity_BuildTime", results,
                      progressBarQuadratic)
            )
            p7.start()
            processes.append(p7)

            print("Running NSpeciesIncreasingConnectivity build and sim time")
            p8 = mp.Process(
                target=build_and_sim_time,
                args=(NSpeciesIncreasingConnectivity.generateModelsWithNSpeciesIncreasingConnectivity,
                      int(np.floor(np.sqrt(N))),
                      "NReactionsIncreasingConnectivity_BuildAndSimTime", results, progressBarQuadratic))
            p8.start()
            processes.append(p8)

            print("Running NSpeciesIncreasingConnectivity sim time")
            p9 = mp.Process(
                target=sim_time,
                args=(NSpeciesIncreasingConnectivity.generateModelsWithNSpeciesIncreasingConnectivity,
                      int(np.floor(np.sqrt(N))), "NReactionsIncreasingConnectivity_SimTime", results,
                      progressBarQuadratic))
            p9.start()
            processes.append(p9)



        if RUN_BUILD_BIOMODELS:
            print("Running BuildBuimodels")
            p10 = mp.Process(
                target=build_time,
                args=(build_biomodels.buildBiomodels,
                      -1, "BuildBiomodels_BuildTime", results,
                      progressBarLinear))
            p10.start()
            processes.append(p10)

        # save data to pickle file
        for i, p in enumerate(processes):
            p.join()

        # convert to regular dict from here
        results = dict(results)
        with open(RESULTS_PICKLE_FILE, 'wb') as f:
            pickle.dump(results, f)


        print(results)
    if PLOT_RESULTS:
        # print(type(results))
        # print(len(results["NReactionsIncreasingConnectivity_BuildTime"]))
        plot(results, RUN_TAG, OUTPUT_DIR)

    if PLOT_BARGRAPH:
        pass # call other script when its ready
