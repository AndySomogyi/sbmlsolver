import os, sys, glob
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
rrm = RoadRunnerMap(biomodels_files[:number_of_models], number_of_threads)

print(f"RoadRunnerMap size: {len(rrm)}")

# insert a single model with a custom key
rrm["newModel"] = biomodels_files[50]
print(f"RoadRunnerMap size: {len(rrm)}")

# or equivalently
rrm.insert("newModel", biomodels_files[50])
print(f"RoadRunnerMap size: {len(rrm)}")

# Insert using the model name as key, which is the default behaviour
rrm.insert(biomodels_files[50])
print(f"RoadRunnerMap size: {len(rrm)}")

# Insert another set of models into the map in parallel
print(f"number of threads being used: {rrm.getNumThreads()}")

# change the number of threads
number_of_threads = 5
rrm.setNumThreads(5)
print(f"number of threads being used: {rrm.getNumThreads()}")

start = time.time()
rrm.insert(biomodels_files[500:600])
duration = time.time() - start

print(f"Another 100 models loaded in {duration:0.2f} seconds")

print(f"RoadRunnerMap size: {len(rrm)}")


"""
Script output
=============
RoadRunnerMap size: 20
RoadRunnerMap size: 21
RoadRunnerMap size: 21
RoadRunnerMap size: 22
number of threads being used: 3
number of threads being used: 5
Another 100 models loaded in 6.34 seconds
RoadRunnerMap size: 122

"""

