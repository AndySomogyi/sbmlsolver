#!/usr/bin/env python

from distutils.core import setup

# the source root directory
_src = "site-packages/"
_version = "1.1.0"

def _sharedLibExt():
    import platform
    return "so" if platform.system() != "Windows" else "pyd"

def _checkRequirements():
    import platform
    import sys

    if sys.version_info < (2,6):
        raise SystemExit("Error, libRoadRunner {0} requires Python 2.6 or better, detected Python version {1}"
                        .format(_version, sys.version_info))

    try:
        # check for numpy
        import numpy
    except ImportError:
        raise SystemExit("numpy package not found, libRoadRunner requires numpy 1.2.1 or better")

    # splits the string into an array like: ['1', '2', '1']
    nver = numpy.__version__.split('.')
    nver = [int(x) for x in nver]

    if nver < [1,2,1]:
        raise SystemExit("libRoadRunner requires numpy 1.2.1 or better, detected numpy version {0}"
                         .format(nver))


_checkRequirements()
        
setup(name='libRoadRunner',
      version=_version,
      description='libRoadRunner SBML JIT compiler and simulation library',
      url='http://libroadrunner.org',
      packages=['roadrunner', 'roadrunner.testing'],
      package_dir={
           "roadrunner" : _src + "roadrunner", 
           "roadrunner.testing" : _src + "roadrunner/testing"
      },
      package_data={
          "roadrunner" : ["_roadrunner." + _sharedLibExt() ],
          "roadrunner.testing" : ["*.xml", "*.txt", "*.dat"]
      }
     )
      
