import sys
sys.path += [
  r"D:\roadrunner\roadrunner\cmake-build-release-llvm-13\lib\site-packages"
]
from roadrunner import RoadRunner
import roadrunner
from roadrunner.testing import TestModelFactory as tmf
from multiprocessing import Pool, cpu_count
import time
from platform import platform
import cpuinfo # pip install py-cpuinfo
import numpy as np

from multiprocessing import Pool, cpu_count

# print(roadrunner.__file__)

NCORES = cpu_count()
NSIMS = 10000


def simulate_worker(r: RoadRunner):
  r.resetAll()
  r.simulate(0, 10000, 10000)


def parallel(r : RoadRunner, N: int):
  p = Pool(processes=cpu_count())
  p.map(simulate_worker, [r for i in range(N)])


def serial(r: RoadRunner, N: int):
  for i in range(N):
    r.simulate(0, 10000, 10000)


if __name__ == '__main__':

  nsimsVec = [1, 10, 100, 1000, 10000, 100000]
  for i in nsimsVec:
    start = time.time()

    r = RoadRunner(tmf.SimpleFlux().str())
    # r = RoadRunner(tmf.Brown2004().str())
    serial(r, N=i)
    # parallel(r, N=i)

    duration = time.time() - start

    print(f'Took {duration} seconds to run', i, 'deterministic simulations on', NCORES, 'cores')
    cpu_info = cpuinfo.get_cpu_info()
    print(f'Platform: {platform()}')
    print('python_version:', cpu_info['python_version'])
    print('Processor:', cpu_info['brand_raw'])
    print()

