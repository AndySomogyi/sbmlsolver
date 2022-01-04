import os, sys, glob

sys.path += [
    r"D:\roadrunner\roadrunner\cmake-build-release\lib\site-packages"
]

from roadrunner import RoadRunner, RoadRunnerMap, Config
from get_biomodels import get_biomodels_folder
import time

# get curated section of biomodels locally.
biomodels_folder = get_biomodels_folder()
biomodels_files = glob.glob(os.path.join(biomodels_folder, "*.xml"))

# disable model caching to compare model load times
Config.setValue(Config.LOADSBMLOPTIONS_RECOMPILE, True)

number_of_threads = 3
number_of_models = 20

# build the first `number_of_models` models using `number_of_threads` threads
start_time = time.time()
rrm = RoadRunnerMap(biomodels_files[:number_of_models], number_of_threads)
duration = time.time() - start_time

print(f"{number_of_models} models loaded in {duration:.4f} seconds using {number_of_threads} threads")

"""
Script output
=============
20 models loaded in 0.7088 seconds using 3 threads

"""