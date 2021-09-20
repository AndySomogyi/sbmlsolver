from threading import Thread
from multiprocessing import Queue
from multiprocessing.pool import ThreadPool
import numpy as np
import unittest

import sys
import pickle
import os
from os.path import dirname, exists, join

import roadrunner

sys.path += [
    # r"D:\roadrunner\roadrunner\install-msvc2019-rel\site-packages",
    r"D:\roadrunner\roadrunner\cmake-build-release-visual-studio\lib\site-packages"
]

import roadrunner.testing.TestModelFactory as tmf
from roadrunner.roadrunner import RoadRunner


class SimulationWorkerThread(Thread):

    def __init__(self, input_queue: Queue, results_queue: Queue):
        super().__init__()
        self.input_queue = input_queue
        self.results_queue = results_queue

    def run(self) -> None:
        for data in iter(self.input_queue.get, "STOP"):
            # self.results_queue.put(data.simulate(0, 10, 11))
            self.results_queue.put(data)
results = []
def f(r):
    results.append(r.simulate(0, 10, 11))


def job(r):
    return r.simulate(0, 10, 11)

class RoadRunnerPickleTests(unittest.TestCase):

    def setUp(self):
        rr = RoadRunner(tmf.SimpleFlux().str())
        rr.setIntegrator("gillespie")
        gillespie = rr.getIntegrator()
        gillespie.seed = 123
        self.rr = rr

    def tearDown(self):
        pass

    def test_with_pool(self):
        import numpy as np
        from multiprocessing import Pool
        p = Pool(processes=4)
        data = p.map(job, [self.rr for i in range(10)])
        p.close()
        print(data)

    def test_named_array(self):
        pass
    #
    # def test_to_pickle(self):
    #     pfile = os.path.join(os.path.dirname(__file__), "pkl.pickle")
    #     with open(pfile, 'wb') as f:
    #         pickle.dump(self.rr, f)
    #
    #     self.assertTrue(os.path.isfile(pfile))
    #
    #     with open(pfile, "rb") as f:
    #         rr = pickle.load(f)
    #     try:
    #         print(rr.simulate(0, 10, 11))
    #     except Exception:
    #         self.fail("Cannot simulate a pickle loaded model")
    #
    #     if os.path.isfile(pfile):
    #         os.remove(pfile)
    #
    # def test_simulate(self):
    #     N = 10  # number of simulations to run
    #     NP = 8  # number of threads to use
    #
    #     input_queue = Queue()
    #     results_queue = Queue()
    #
    #     # populate input queue
    #     for i in range(N):
    #         input_queue.put(self.rr)
    #
    #     # create threads
    #     for _ in range(NP):
    #         SimulationWorkerThread(input_queue, results_queue).start()
    #
    #     # get results
    #     for i in range(N):
    #         print(results_queue.get())
    #
    #     # terminate the continually iterating threads
    #     for _ in range(NP):
    #         input_queue.put("STOP")
    #
    #
