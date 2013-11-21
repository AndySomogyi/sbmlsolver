---
layout: page
title: "Build RoadRunner from Source on Windows"
description: "howto build the library"
---

## Introduction

To build roadrunner from source we use:

Download and install the following prior to starting. These are the download links.
* [CMake 2.8.12.1](http://www.cmake.org/files/v2.8/cmake-2.8.12.1-win32-x86.exe)
* [Microsoft Visual Studio 2010 Express](https://www.dreamspark.com/Product/Product.aspx?productid=32)
* [Python 2.7.6](http://python.org/ftp/python/2.7.6/python-2.7.6.msi)
* [Git 1.8.4](http://git-scm.com/download/win) Choose "Use GitBash Only" and default install options.

The instructions below will guide you to:
 * Install **SWIG**
 * Build and install **LLVM** (roadrunner depends on llvm)
 * Build and install **roadrunner_thirdparty** libraries (roadrunner depends on these libraries)
 * Build and install **roadrunner** (these instructions include building the Python bindings)

## Overview
1. Create the directory structure we want for the build and install
2. Install SWIG
3. Get the Source Code
4. CMake Configure and Generate Solution files (outputs to build/)
5. Visual Studio build and install (outputs to install/)


#Create directory structure for build, install, and source

The following commands are used in the Git Bash console. They should work in a regular windows command prompt as well.  To use `wget` in Git Bash follow the instructions here: [http://www.kadamwhite.com/archives/2012/wget-in-git-bash](http://www.kadamwhite.com/archives/2012/wget-in-git-bash)

Your directory tree wil look like this:

<pre>
vs
+---build
|   +---llvm
|   +---roadrunner
|   |   \---release
|   \---roadrunner_thirdparty
|       \---release
+---install
|   +---llvm
|   +---roadrunner
|   |   \---release
|   +---roadrunner_thirdparty
|   |   \---release
|   \---swigwin-2.0.11
\---src
    +---llvm-3.3.src
        \---roadrunner
	        \---third_party

</pre>


###Create directories
The commands below can be copied and pasted into the console.

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
    cd ../..

    #Source dirs
    mkdir src
    cd src
    cd ..

   
#Install swig

Get **swigwin-2.0.11** binaries for windows
    
    cd install
    wget http://prdownloads.sourceforge.net/swig/swigwin-2.0.11.zip
    unzip swigwin-2.0.11.zip
    cd ..

#Get source code

Clone the **roadrunner** source code repository
 
    cd src
    git clone https://github.com/AndySomogyi/roadrunner.git

Get the **LLVM 3.3** source code

    wget http://llvm.org/releases/3.3/llvm-3.3.src.tar.gz
    tar -xzvf llvm-3.3.src.tar.gz



#Configure with CMake
Using *CMake GUI* Configure and Generate Solution files (outputs to build/)

## CMake LLVM
Set:

    Source: C:/vs/src/llvm-3.3.src
    Build: C:/vs/build/llvm

Configure >Visual Studio 10 > Use default Native compilers > Finish

Set the following:

    CMAKE_INSTALL_PREFIX:PATH=C:/vs/install/llvm

Press configure again and wait until each option has changed color from [Red](assets/images/ThirdPartyCMake.jpg) to [White](assets/images/ThirdPartyCMakeB.jpg)

Configure>Generate

## CMake Roadrunner Thirdparty
    Source: C:/vs/src/roadrunner/third_party
    Build: C:/vs/build/roadrunner_thirdparty/release

Configure >Visual Studio 10 > Use default Native compilers > Finish

Set the following

    CMAKE_INSTALL_PREFIX:PATH=C:/vs/install/roadrunner_thirdparty/release
    CMAKE_BUILD_TYPE:STRING=Release (this flag does not control the type of build, its ignored)

*Note Special Instructions: When building for use in C (below)*

## CMake Roadrunner (with SWIG Python API)
    Source: C:/vs/src/roadrunner
    Build: C:/vs/build/roadrunner/release

Configure >Visual Studio 10 > Use default Native compilers > Finish

Set the following

    CMAKE_INSTALL_PREFIX:PATH=C:/vs/install/roadrunner/release
    LLVM_INSTALL_PREFIX:PATH=C:/vs/install/llvm
    CMAKE_BUILD_TYPE:STRING=Release
    THIRD_PARTY_INSTALL_FOLDER:PATH=C:/vs/install/roadrunner_thirdparty/release
    BUILD_LLVM:BOOL=1
    BUILD_SWIG_PYTHON:BOOL=1
    SWIG_DIR:PATH=C:/vs/install/swigwin-2.0.11

#Using Visual Studio

## Build LLVM
Open Solution file

File > Open > Project/Solution > C:\vs\build\llvm\LLVM.sln

Set

    Solution Configurations: Release
    Solution Platforms: Win32

###Build and Install
In Visual Studio's [Solution Explorer](assets/images/VSInstallB.jpg) Right-Click > INSTALL > Build

## Build Roadrunner ThirdParty
Open Solution file

File > Open > Project/Solution > C:\vs\build\roadrunner_thirdparty\release\RRThirdPartyBuild.sln

Set

    Solution Configurations: Release
    Solution Platforms: Win32

###Build and Install
In Solution Explorer Right-Click > INSTALL > Build

## Build Roadrunner ThirdParty
Open Solution file

File > Open > Project/Solution > C:\vs\build\roadrunner\release\rr.sln

Set

    Solution Configurations: Release
    Solution Platforms: Win32

###Build and Install
In Solution Explorer Right-Click > INSTALL > Build



#Note Special Instructions: When building for use in C 

If building a release version of RoadRunner for C 
The `CMAKE_C_FLAGS_RELEASE` flag and the `CMAKE_CXX_FLAGS_RELEASE` need to be modified from its default. The default switches 

    /O2 /Ob2

needs to be changed to

    /Od /Ob0

This need to be observed for both the ThirdParty build, as well as for RoadRunner itself.

Comment on this Issue: The problem seem to be in the CVODE integration library, not being able to converge properly when full optimization is turned on.

[old_buildrr_visualstudio instructions file](old_buildrr_visualstudio.html)
