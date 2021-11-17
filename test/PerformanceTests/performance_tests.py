# Run some performance tests for roadrunner
import sys
from typing import Optional

sys.path += [
    r"D:\roadrunner\roadrunner\cmake-build-release\lib\site-packages"
]
import os
import pickle
import roadrunner
from roadrunner import RoadRunner
import NLinearChain
import NReactionsUncoupled
import numpy as np
import time
import matplotlib.pyplot as plt
import seaborn as sns
import multiprocessing as mp
from multiprocessing.managers import SyncManager


def progressBar(i: int, N: int, name: str=None):
    if i % 10 == 0:
        if name:
            print(f"{name} -- {(i / N * 100)}% completed")
        else:
            print(f"{(i / N * 100)}% completed")

def build_time(sbmlGeneratorFunction: callable, N: int, results_key: str, results: SyncManager.dict) -> np.ndarray:
    """Measure time it takes to build roadrunner models sequence produced
    by the callable sbmlGeneratorFunction

    :param sbmlGeneratorFunction: a function that yields an sbml string.
    The signature must be "def exampleGenerator(N: int) -> str" where
    N encodes the number of models to generate (though may actually represent
    different quantities in different situations).
    :param N: is passed on to sbmlGeneratorFunction
    """
    times = np.zeros((N,))
    for i, sbmlString in enumerate(sbmlGeneratorFunction(N)):
        start = time.time()
        rrModel = RoadRunner(sbmlString)
        times[i] = time.time() - start
        i += 1
        progressBar(i, N, results_key)
    results[results_key] = times
    return times


def build_and_sim_time(sbmlGeneratorFunction: callable, N: int, results_key: str,
                       results: SyncManager.dict) -> np.ndarray:
    """Measure time it takes to build roadrunner models sequence produced
    by the callable sbmlGeneratorFunction and run a 1000 time step simulation.

    :param sbmlGeneratorFunction: a function that yields an sbml string.
    The signature must be "def exampleGenerator(N: int) -> str" where
    N encodes the number of models to generate (though may actually represent
    different quantities in different situations).
    :param N: is passed on to sbmlGeneratorFunction
    """
    times = np.zeros((N,))
    for i, sbmlString in enumerate(sbmlGeneratorFunction(N)):
        start = time.time()
        rrModel = RoadRunner(sbmlString)
        rrModel.simulate(0, 1000, 1001)
        times[i] = time.time() - start
        i += 1
        progressBar(i, N, results_key)
    results[results_key] = times
    return times


def sim_time(sbmlGeneratorFunction: callable, N: int, results_key: str, results: SyncManager.dict) -> np.ndarray:
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
    times = np.zeros((N,))
    for i, sbmlString in enumerate(sbmlGeneratorFunction(N)):
        rrModel = RoadRunner(sbmlString)
        start = time.time()
        rrModel.simulate(0, 1000, 1001)
        times[i] = time.time() - start
        i += 1
        progressBar(i, N, results_key)
    results[results_key] = times
    return times


def plotResults(times: np.ndarray, fname: str, xlabel: str, ylabel: str, title: str):
    fig = plt.figure()
    plt.plot(range(len(times)), times, "ro")
    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    plt.title(title)
    sns.despine(fig=fig)
    plt.savefig(fname, dpi=150, bbox_inches='tight')


def plot_linear_chain_build_time_results(times: np.ndarray):
    fname = os.path.join(os.path.dirname(__file__), "NLinearChainBuildTimes.png")
    plotResults(times, fname, "n species", "build time (s)", "NLinearChain")


def plot_linear_chain_build_and_sim_time_results(times: np.ndarray):
    fname = os.path.join(os.path.dirname(__file__), "NLinearChainBuildAndSimTimes.png")
    plotResults(times, fname, "n species", "build and sim time (s)", "NLinearChain")


def plot_linear_chain_time_results(times: np.ndarray):
    fname = os.path.join(os.path.dirname(__file__), "NLinearChainSimTimes.png")
    plotResults(times, fname, "n species", "sim time (s)", "NLinearChain")


def plot_n_reactions_build_time_results(times: np.ndarray):
    fname = os.path.join(os.path.dirname(__file__), "NReactionsUncoupledBuildTimes.png")
    plotResults(times, fname, "n reactions (uncoupled)", "build time (s)", "NReactions")


def plot_n_reactions_build_and_sim_time_results(times: np.ndarray):
    fname = os.path.join(os.path.dirname(__file__), "NReactionsUncoupledBuildAndSimTimes.png")
    plotResults(times, fname, "n reactions (uncoupled)", "build and sim time (s)", "NReactions")


def plot_n_reactions_time_results(times: np.ndarray):
    fname = os.path.join(os.path.dirname(__file__), "NReactionsUncoupledSimTimes.png")
    plotResults(times, fname, "n reactions (uncoupled)", "sim time (s)", "NReactions")


def plot(results: dict):
    plot_linear_chain_build_time_results(results["NLinearChain_BuildTime"])
    plot_linear_chain_build_and_sim_time_results(results["NLinearChain_BuildAndSimTime"])
    plot_linear_chain_time_results(results["NLinearChain_SimTime"])
    plot_n_reactions_build_time_results(results["NReactionsUncoupled_BuildTime"])
    plot_n_reactions_build_and_sim_time_results(results["NReactionsUncoupled_BuildAndSimTime"])
    plot_n_reactions_time_results(results["NReactionsUncoupled_SimTime"])


if __name__ == "__main__":
    # Where to store the results once the script has been executed
    RESULTS_PICKLE_FILE = os.path.join(os.path.dirname(__file__), "performance_test_results.pickle")

    # When true, pickled simulation results are be overwritten
    # When false, and if the RESULTS_PICKLE_FILE exists, read from the pickle
    USE_PICKLED_RESULTS = False

    # Plot results
    PLOT_RESULTS = True

    # To collect results from threads we need a shared dict
    # which is provided by the mp.Manager.
    manager = mp.Manager()

    # this becomes an "out" parameter
    results = manager.dict()

    # The N parameter for the functions above.
    # Represents size of the problem to time.
    N = 600

    sx = os.path.exists(RESULTS_PICKLE_FILE)

    if USE_PICKLED_RESULTS and os.path.exists(RESULTS_PICKLE_FILE):
        with open(RESULTS_PICKLE_FILE, 'rb') as f:
            results = pickle.load(f)
    else:

        # Run 6 performance tests in separate processes.
        processes = []
        print("Running NReactionsUncoupled build time")
        p1 = mp.Process(target=build_time, args=(
            NReactionsUncoupled.generateModelsWithNUncoupledReactions,
            N,
            "NReactionsUncoupled_BuildTime",
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

        for i, p in enumerate(processes):
            p.join()

        # convert to regular dict from here
        results = dict(results)
        with open(RESULTS_PICKLE_FILE, 'wb') as f:
            pickle.dump(results, f)

    if PLOT_RESULTS:
        print(results)
        print(type(results))
        plot(results)
