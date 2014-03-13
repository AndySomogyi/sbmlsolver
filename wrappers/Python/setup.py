#!/usr/bin/env python

from distutils.core import setup
import platform
import shutil
import sys

# the source root directory
_src = "site-packages/"
_version = open("VERSION.txt").readline().strip()

print("packaging / installing pylibroadrunner version:{}".format(_version))

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

# get the os string
def _getOSString():
    
    
    s = platform.system().lower()
    unix = False

    if s.startswith('darwin'):
        s='macosx'
        unix = True
    elif s.startswith('linux'):
        s='linux'
        unix = True
    elif s.startswith('win'):
        s='win'
    else:
        raise SystemExit("I don't know how to install on {}, only know OSX(x86_64), Linux(i386,x86_64) and Win(32)"
                         .format(s))

    arch = platform.architecture()
    
    if unix and arch[0].startswith('64'):
        arch = 'x86_64'
    elif unix and arch[0].startswith('32'):
        arch = 'i386'
    else:
        arch = '32' #win 32

    return s + "_" + arch
    
# copy depdency dlls into same dir as _roadrunner.pyd on 
# windows
def _copyWindowsDLLs():
    if platform.system().lower().startswith("win"):
        print("copying windows dlls...")
        
        shutil.copyfile("bin/iconv.dll",      "site-packages/roadrunner/iconv.dll")
        shutil.copyfile("bin/msvcp100.dll",   "site-packages/roadrunner/msvcp100.dll")
        shutil.copyfile("bin/msvcr100.dll",   "site-packages/roadrunner/msvcr100.dll")
        shutil.copyfile("bin/libxml2.DLL",    "site-packages/roadrunner/libxml2.dll")
        shutil.copyfile("bin/zlib1.dll",      "site-packages/roadrunner/zlib1.dll")
    
    
_version = _version + "-" + _getOSString()

print("version: " + _version)

_checkRequirements()
  
# copy stuff if we are building
if len(sys.argv) > 1 and sys.argv[1].lower().find('dist') >= 0:
    print ("we're building...")
    _copyWindowsDLLs()

setup(name='pylibroadrunner',
      version=_version,
      description='libRoadRunner SBML JIT compiler and simulation library',
      url='http://libroadrunner.org',
      packages=['roadrunner', 'roadrunner.testing'],
      package_dir={
           "roadrunner" : _src + "roadrunner", 
           "roadrunner.testing" : _src + "roadrunner/testing"
      },
      package_data={
          # add dll, won't hurt unix, not there anyway
          "roadrunner" : ["_roadrunner." + _sharedLibExt(), "*.dll" ],  
          "roadrunner.testing" : ["*.xml", "*.txt", "*.dat"]
      }
     )
      
