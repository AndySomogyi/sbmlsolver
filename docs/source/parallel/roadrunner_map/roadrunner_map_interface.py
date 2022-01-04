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
rrm = RoadRunnerMap(biomodels_files[:number_of_models], number_of_threads)

# keys, values and items work as expected
print("keys:", rrm.keys())
# print("values", rrm.values()) # prints out list of roadrunner models

for modelName, rrModel in rrm.items():
    print(f"model \"{modelName}\" has {rrModel.getModel().getNumReactions()} reactions")

# delete an item
print(f"number of models before deletion: {len(rrm)}")
del rrm[rrm.keys()[0]]
print(f"number of models after deletion: {len(rrm)}")

"""
Script output
=============
keys: ('Goldbeter1995_CircClock', 'Tyson1991 - Cell Cycle 6 var', 'Novak1997 - Cell Cycle', 'Elowitz2000 - Repressilator', 'Levchenko2000_MAPK_noScaffold', 'Gardner1998 - Cell Cycle Goldbeter', 'Goldbeter1991 - Min Mit Oscil', 'Schoeberl2002 - EGF MAPK', 'Huang1996 - Ultrasensitivity in MAPK cascade', 'Goldbeter1991 - Min Mit Oscil, Expl Inact', 'Edelstein1996 - EPSP ACh species', 'hodgkin-huxley squid-axon 1952', 'Kholodenko2000 - Ultrasensitivity and negative feedback bring oscillations in MAPK cascade', 'Hoefnagel2002_PyruvateBranches', 'Tyson1991 - Cell Cycle 2 var', 'Curto1998 - purine metabolism', 'Edelstein1996 - EPSP ACh event', 'Morrison1989 - Folate Cycle', 'Levchenko2000_MAPK_Scaffold', 'Poolman2004_CalvinCycle')
model "Goldbeter1995_CircClock" has 10 reactions
model "Tyson1991 - Cell Cycle 6 var" has 9 reactions
model "Novak1997 - Cell Cycle" has 25 reactions
model "Elowitz2000 - Repressilator" has 12 reactions
model "Levchenko2000_MAPK_noScaffold" has 30 reactions
model "Gardner1998 - Cell Cycle Goldbeter" has 13 reactions
model "Goldbeter1991 - Min Mit Oscil" has 7 reactions
model "Schoeberl2002 - EGF MAPK" has 125 reactions
model "Huang1996 - Ultrasensitivity in MAPK cascade" has 20 reactions
model "Goldbeter1991 - Min Mit Oscil, Expl Inact" has 7 reactions
model "Edelstein1996 - EPSP ACh species" has 17 reactions
model "hodgkin-huxley squid-axon 1952" has 0 reactions
model "Kholodenko2000 - Ultrasensitivity and negative feedback bring oscillations in MAPK cascade" has 10 reactions
model "Hoefnagel2002_PyruvateBranches" has 14 reactions
model "Tyson1991 - Cell Cycle 2 var" has 3 reactions
model "Curto1998 - purine metabolism" has 37 reactions
model "Edelstein1996 - EPSP ACh event" has 17 reactions
model "Morrison1989 - Folate Cycle" has 47 reactions
model "Levchenko2000_MAPK_Scaffold" has 300 reactions
model "Poolman2004_CalvinCycle" has 21 reactions
number of models before deletion: 20
number of models after deletion: 19
"""