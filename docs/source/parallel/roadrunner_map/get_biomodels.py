import zipfile
import os, sys, glob

from roadrunner import RoadRunner, RoadRunnerMap
from roadrunner.tests.download_biomodels import download_biomodels

current_directory = os.path.dirname(__file__)
biomodels_zip = os.path.join(current_directory, "biomodels.zip")
biomodels_directory = os.path.join(current_directory, "biomodels")

def get_biomodels_folder() -> str:
    """download curated section of biomodels, if not already exists and
    extract to a folder called biomodels"""
    # don't do work more than once
    if os.path.isdir(biomodels_directory):
        return biomodels_directory
    # downloads models if not exist
    print("Downloading the curated section from biomodels")
    models_zip = download_biomodels(current_directory)
    assert biomodels_zip == models_zip, f"{biomodels_zip} != {models_zip}"

    # extract models.
    with zipfile.ZipFile(models_zip, 'r') as zip_ref:
        zip_ref.extractall(biomodels_directory)
    return biomodels_directory



if __name__ == "__main__":
    # get some local sbml files for use with the example
    get_biomodels_folder()
