import sys

sys.path += [
    r"D:\roadrunner\roadrunner\cmake-build-release\lib\site-packages"
]

from roadrunner import RoadRunner
import time
import numpy as np
import matplotlib.pyplot as plt
import matplotlib
import seaborn as sns
import os
import antimony
import glob
import zipfile


def buildBiomodels(n: int = -1):
    """Build all of the models downloaded from the biomodels
    database.

    If you do not yet have the curated section of biomodels downloaded
    this generator will do so for you the first time it is run.

    :param n: how many biomodels models to download. When -1, do all.
    """
    current_directory = os.path.dirname(__file__)
    biomodels_directory = os.path.join(current_directory, "biomodels")
    biomodels_zip = os.path.join(current_directory, "biomodels.zip")
    if not os.path.exists(biomodels_directory):
        os.system("python download_biomodels.py")
        if not os.path.exists(biomodels_zip):
            raise ValueError("A file called biomodels.zip should exist but doesn't")

        if not os.path.isdir(biomodels_directory):
            os.makedirs(biomodels_directory)

        with zipfile.ZipFile(biomodels_zip, 'r') as zip_ref:
            zip_ref.extractall(biomodels_directory)

    files = glob.glob("biomodels/*.xml")
    if n == -1:
        n = len(files)
    for f in files[:n]:
        with open(f, 'r') as f:
            # some fail with
            #   UnicodeDecodeError: 'charmap' codec can't decode byte 0x9d in position 39237: character maps to <undefined>
            try:
                yield f.read()
            except UnicodeDecodeError as e:
                print(e)


if __name__ == "__main__":
    """Time how long it takes to build the biomodels curated 
    section in the following conditions: 
        - roadrunner 2.1.3
        - MCJit
        - LLJit
            - 1 thread
                - Each of the optimization levels
            - 16 threads
                - Each of the optimization levels
    """

    for i in buildBiomodels():
        print(i)
