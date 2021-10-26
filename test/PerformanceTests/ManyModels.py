"""Measure how long it takes to construct 1000 unique models

Roadrunner does model caching so to make sure the models are all being constructed
we use the direct API to ensure they are different models.

"""

import sys
llvmversion = 13
sys.path += [
    # r'D:\roadrunner\roadrunner\roadrunner-install-rel-llvm{}\site-packages'.format(llvmversion),
    r'D:\roadrunner\roadrunner\roadrunner-install-rel-llvm{}\site-packages'.format(llvmversion)
]

import roadrunner

assert "llvm13" in roadrunner.__file__, roadrunner.__file__

from roadrunner import RoadRunner, Config
from roadrunner.testing import TestModelFactory as tmf
import time
import numpy as np
from platform import platform
import cpuinfo  # pip install py-cpuinfo
import libsbml

NSIMS = 1000

def x(r: RoadRunner):
    Config.setValue(Config.LOADSBMLOPTIONS_OPTIMIZE_GVN, False)
    Config.setValue(Config.LOADSBMLOPTIONS_OPTIMIZE_CFG_SIMPLIFICATION, False)
    Config.setValue(Config.LOADSBMLOPTIONS_OPTIMIZE_DEAD_CODE_ELIMINATION, False)
    Config.setValue(Config.LOADSBMLOPTIONS_OPTIMIZE_DEAD_INST_ELIMINATION, False)
    Config.setValue(Config.LOADSBMLOPTIONS_OPTIMIZE_INSTRUCTION_COMBINING, False)
    Config.setValue(Config.LOADSBMLOPTIONS_OPTIMIZE_INSTRUCTION_SIMPLIFIER, False)

def addDummySpecies(model: libsbml.Model, id: int):
    assert id > 2, "Id should be greater than 2. "
    SNew = model.createSpecies()
    SNew.setId(f'S{id}')
    SNew.setCompartment('default_compartment')
    SNew.setConstant(False)
    SNew.setInitialAmount(5)
    SNew.setSubstanceUnits('mole')
    SNew.setBoundaryCondition(False)
    SNew.setHasOnlySubstanceUnits(False)


def removeDummySpecies(model: libsbml.Model, id: int):
    model.removeSpecies(f"S{id}")


if __name__ == '__main__':
    # setup timing
    start = time.time()

    # get sbml to work with from one of our test modules
    sbmlOrig = tmf.OpenLinearFlux().str()

    document = libsbml.readSBMLFromString(sbmlOrig)
    model = document.getModel()

    for i in range(3, NSIMS + 3):
        addDummySpecies(model, i)
        sbml = libsbml.writeSBMLToString(document)
        r = RoadRunner(sbml)
        removeDummySpecies(model, i)

    print('Took', time.time() - start, 'seconds to build', NSIMS, 'unique models')
    cpu_info = cpuinfo.get_cpu_info()
    print(f'Platform: {platform()}')
    print('python_version:', cpu_info['python_version'])
    print('Processor:', cpu_info['brand_raw'])

    '''
    With call to finalize
    ---------------------
    Took 16.030054569244385 seconds to build 1000 unique models
    Took 15.91389012336731 seconds to build 1000 unique models
    Took 16.681470155715942 seconds to build 1000 unique models
    Took 15.717174768447876 seconds to build 1000 unique models
    Took 16.053847551345825 seconds to build 1000 unique models

    
    Without call to finalize
    ----------------------------
    Took 13.828417778015137 seconds to build 1000 unique models
    Took 15.37995982170105 seconds to build 1000 unique models

    
    with call to finalize. No optimization passes. 
    -------------------------------------------------
    Took 12.361215829849243 seconds to build 1000 unique models
    Took 14.250553607940674 seconds to build 1000 unique models
    Took 13.101083278656006 seconds to build 1000 unique models
    Took 13.356266498565674 seconds to build 1000 unique models
    Took 13.263121604919434 seconds to build 1000 unique models




    

    '''
