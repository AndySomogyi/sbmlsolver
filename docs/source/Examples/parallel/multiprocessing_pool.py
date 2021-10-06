from roadrunner import RoadRunner
from roadrunner.testing import TestModelFactory as tmf
from multiprocessing import Pool, cpu_count
import time
from platform import platform
import cpuinfo # pip install py-cpuinfo

NCORES = cpu_count()
NSIMS = 1000000

def simulate_worker(r: RoadRunner):
    return r.simulate(0, 10, 11)


if __name__ == '__main__':
    # setup timing
    start = time.time()

    # get sbml to work with from one of our test modules
    sbml = tmf.OpenLinearFlux().str()

    # create our roadrunner instance
    r = RoadRunner(sbml)

    # set up a stochastic simulation
    r.setIntegrator('gillespie')

    # set the seed for reproducuble example
    gillespie_integrator = r.getIntegrator()
    gillespie_integrator.seed = 1234

    # create a processing pool
    p = Pool(processes=NCORES)

    # perform the simulations
    arrays = p.map(simulate_worker, [r for i in range(NSIMS)])

    print('Took',  time.time() - start, 'seconds to run', NSIMS, 'stochastic simulations on', NCORES, ' core')
    cpu_info = cpuinfo.get_cpu_info()
    print(f'Platform: {platform()}')
    print('python_version:', cpu_info['python_version'])
    print('Processor:', cpu_info['brand_raw'])
    #  Took 14.95907735824585 seconds to run 1000000 stochastic simulations on 16  core
    #  Platform: Windows-10-10.0.22000-SP0
    #  python_version: 3.9.5.final.0 (64 bit)
    #  Processor: 11th Gen Intel(R) Core(TM) i9-11980HK @ 2.60GHz










