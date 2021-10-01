"""
Example parallel simulation

requirements
    roadrunner
    ray
    pandas
"""
import roadrunner
from roadrunner.testing import TestModelFactory as tmf
import ray
import pandas as pd
import time

# start ray
ray.init(ignore_reinit_error=True)


@ray.remote
class SimulatorActorPath(object):
    """Ray actor to execute simulations."""
    def __init__(self, r: roadrunner.RoadRunner):
        self.r: roadrunner.RoadRunner = r

    def simulate(self, size=1):
        print("Start simulations")
        ts = time.time()
        results = []
        for k in range(size):
            self.r.resetAll()
            s = self.r.simulate(0, 100, steps=100)
            # create numpy array (which can be pickled), better use shared memory
            df = pd.DataFrame(s, columns=s.colnames)
            results.append(df)
        te = time.time()
        print("Finished '{}' simulations: {:2.2f} ms".format(size, (te - ts) * 1000))
        return results


if __name__ == "__main__":
    actor_count = 10   # cores to run this on

    rr = roadrunner.RoadRunner(tmf.SimpleFlux().str())
    simulators = [SimulatorActorPath.remote(rr) for _ in range(actor_count)]

    # run simulations
    sim_per_actor = 10
    tc_ids = []
    for k, simulator in enumerate(simulators):
        tcs_id = simulator.simulate.remote(size=sim_per_actor)
        tc_ids.append(tcs_id)

    results = ray.get(tc_ids)

    print(results)