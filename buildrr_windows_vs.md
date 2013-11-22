Windows Build roadrunner from source

The following commands are used in the Git Bash console. They should work in a regular windows command prompt as well.
To use wget in Git Bash follow the instructions here: http://www.kadamwhite.com/archives/2012/wget-in-git-bash

Install the following prior to starting:
To build roadrunner from source we use:
- CMake
- Visual Studio 2010
and to run the library we like to use:
- Python 2.7

# Overview
#0. The instructions below will guide you to
#     Install SWIG
#     Build and install LLVM (roadrunner depends on llvm)
#     Build and install roadrunner_thirdparty libraries (roadrunner depends on these libraries)
#     Build and install roadrunner (these instructions include building the Python bindings)
#
#1. Make the directory structure we want for the build and install
#    These paths are going to be used throughout the instructions below.
#2. Get the Source Code
#3. CMake Configure and Generate Solution files (outputs to build/)
#4. Visual Studio build and install (outputs to install/)


#Make the directory structure we want to use
#Root dir
cd C:\\
mkdir vs
cd vs

#Build dirs
mkdir build
cd build
mkdir llvm
mkdir roadrunner_thirdparty
cd roadrunner_thirdparty
mkdir release
#mkdir debug
cd ..
mkdir roadrunner
cd roadrunner
mkdir release
#mkdir debug
cd ../..

#Install dirs
mkdir install
cd install
mkdir llvm
mkdir roadrunner_thirdparty
cd roadrunner_thirdparty
mkdir release
cd ..
mkdir roadrunner
cd roadrunner
mkdir release
#mkdir debug
cd ..

##########for tree
mkdir swigwin-2.0.11
cd ..
mkdir src
cd src
mkdir roadrunner
cd roadrunner
mkdir third_party
cd ..
mkdir llvm-3.3.src
################

#Install swig
#Get swigwin-2.0.11.zip binaries
wget http://prdownloads.sourceforge.net/swig/swigwin-2.0.11.zip
unzip swigwin-2.0.11.zip
cd ..

#Source dirs
mkdir src
cd src

#Get source code
#Clone the roadrunner source code repository
git clone https://github.com/AndySomogyi/roadrunner.git

#Get the LLVM 3.3 source code
wget http://llvm.org/releases/3.3/llvm-3.3.src.tar.gz
tar -xzvf llvm-3.3.src.tar.gz

swigwin-2.0.11/
llvm-3.3.src/

#Using  CMake

#LLVM
Source: C:/vs/src/llvm-3.3.src
Build: C:/vs/build/llvm
Configure >Visual Studio 10 > Use default Native compilers > Finish
#Set the following
CMAKE_INSTALL_PREFIX:PATH=C:/vs/install/llvm
Configure>Generate

#Roadrunner Thirdparty
Source: C:/vs/src/roadrunner/third_party
Build: C:/vs/build/roadrunner_thirdparty/release
Configure >Visual Studio 10 > Use default Native compilers > Finish
#Set the following
CMAKE_INSTALL_PREFIX:PATH=C:/vs/install/roadrunner_thirdparty/release
CMAKE_BUILD_TYPE:STRING=Release (this flag does not control the type of build, its ignored)
#Note Special Instructions: When building for use in C (below)

#Roadrunner (with SWIG Python API)
Source: C:/vs/src/roadrunner
Build: C:/vs/build/roadrunner/release
Configure >Visual Studio 10 > Use default Native compilers > Finish
#Set the following
CMAKE_INSTALL_PREFIX:PATH=C:/vs/install/roadrunner/release
LLVM_INSTALL_PREFIX:PATH=C:/vs/install/llvm
CMAKE_BUILD_TYPE:STRING=Release
THIRD_PARTY_INSTALL_FOLDER:PATH=C:/vs/install/roadrunner_thirdparty/release
BUILD_LLVM:BOOL=1
BUILD_SWIG_PYTHON:BOOL=1
SWIG_DIR:PATH=C:/vs/install/swigwin-2.0.11

#Using Visual Studio

#LLVM
#Open Solution file
File > Open > Project/Solution > C:\vs\build\llvm\LLVM.sln

#Set
Solution Configurations: Release
Solution Platforms: Win32

#Build and Install
In Solution Explorer Right-Click > INSTALL > Build

#Roadrunner ThirdParty
#Open Solution file
File > Open > Project/Solution > C:\vs\build\roadrunner_thirdparty\release\RRThirdPartyBuild.sln

#Set
Solution Configurations: Release
Solution Platforms: Win32

#Build and Install
In Solution Explorer Right-Click > INSTALL > Build

#Roadrunner ThirdParty
#Open Solution file
File > Open > Project/Solution > C:\vs\build\roadrunner\release\rr.sln

#Set
Solution Configurations: Release
Solution Platforms: Win32

#Build and Install
In Solution Explorer Right-Click > INSTALL > Build



#Note Special Instructions: When building for use in C 

#If building a release version of RoadRunner for C 
The CMAKE_C_FLAGS_RELEASE flag and the CMAKE_CXX_FLAGS_RELEASE need to be modified from its default. The default switches 

/O2 /Ob2

needs to be changed to

/Od /Ob0

This need to be observed for both the ThirdParty build, as well as for RoadRunner itself.

Comment on this Issue: The problem seem to be in the CVODE integration library, not being able to converge properly when full optimization is turned on.

