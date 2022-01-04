import os, sys, glob
from roadrunner import RoadRunner, RoadRunnerMap, Config
from get_biomodels import get_biomodels_folder
import time

# get curated section of biomodels locally.
biomodels_folder = get_biomodels_folder()
biomodels_files = glob.glob(os.path.join(biomodels_folder, "*.xml"))

# disable model caching to compare model load times
Config.setValue(Config.LOADSBMLOPTIONS_RECOMPILE, True)

number_of_threads = 1
number_of_models = 20

# build the first `number_of_models` models using `number_of_threads` threads
# When the number of threads is 1, multithreading is disabled and
# serial algorithms take over
start_time = time.time()
rrm = RoadRunnerMap(biomodels_files[:20], number_of_threads)
assert len(rrm) == number_of_models
duration = time.time() - start_time
print(f"{number_of_models} models loaded in {duration:.4f} seconds using {number_of_threads} threads")


"""
Script output
=============
20 models loaded in 1.7368 seconds using 1 threads

"""