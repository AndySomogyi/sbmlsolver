---
layout: page
title: "Build RoadRunner from Source on Windows"
description: "howto build the library"
---

## Overview
The step-by-step instructions will guide you to:
1. Create the directory structure for the build and install
2. Install **SWIG**
3. Get Source Code (placed in `src/`)
4. CMake **LLVM**, **roadrunner_thirdparty**, **roadrunner** (outputs to `build/`) 
5. Build and install in Visual Studio: LLVM, roadrunner_thirdparty, roadrunner (outputs to `install/`) 

To prepare download and install the following. These are the tools we use to build roadrunner from source:
* [CMake](http://www.cmake.org/files/v2.8/cmake-2.8.12.1-win32-x86.exe)
* [Microsoft Visual Studio](http://www.visualstudio.com/)
* [Python 2.7](http://python.org/ftp/python/2.7.6/python-2.7.6.msi)
* [Git 1.8.4](http://git-scm.com/download/win)

# 1. Create directory structure 
These will be the directories for build, install, and source

The commands in these instructions are used in the Git Bash console. To use `wget` in Git Bash follow the instructions here to install `wget`: [http://www.kadamwhite.com/archives/2012/wget-in-git-bash](http://www.kadamwhite.com/archives/2012/wget-in-git-bash)
The commands should work in a regular windows command prompt `cmd` as well, except for `wget`.
Your directory tree will look like this:

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


### To create directories run these commands
The commands below should be copied and pasted into the Git Bash console to run. Start the console by going to your windows `Start menu`, typing to search `git bash`, then clicking on it when it comes up. 

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

   
# 2. Install SWIG

Get [swigwin-2.0.11](http://prdownloads.sourceforge.net/swig/swigwin-2.0.11.zip) binaries for windows using the `wget` commad below. Otherwise download it in your browser by clicking on the link, then move the file to the `C:\vs\install` directory and continue with the next command.
    
    cd install
    wget http://prdownloads.sourceforge.net/swig/swigwin-2.0.11.zip
    unzip swigwin-2.0.11.zip
    cd ..

# 3. Get source code

Clone the **roadrunner** source code repository
 
    cd src
    git clone https://github.com/AndySomogyi/roadrunner.git

Get the [LLVM 3.3](http://llvm.org/releases/3.3/llvm-3.3.src.tar.gz) source code using the `wget` command below. Otherwise download it from the link and move the file to the `C:\vs\src` directory and continue with the next command.

    wget http://llvm.org/releases/3.3/llvm-3.3.src.tar.gz
    tar -xzvf llvm-3.3.src.tar.gz



# 4. Using CMake
Using *CMake GUI* we will Configure and Generate Solution files (outputs to `build/`)

## 4.a. CMake LLVM
The [screenshot panels](assets/images/CMakeGUI-LLVM_screenshot.png) show you where things are on the CMake GUI.

Set the Source Path and Build Path to directories for `llvm`

    Source Path = C:/vs/src/llvm-3.3.src
    Build Path = C:/vs/build/llvm

Then click Configure and the default options, listed here:

    Configure > Visual Studio 10 > Use default Native compilers > Finish

Set the Install Path by pasting it in the `CMAKE_INSTALL_PREFIX` option. See the [2nd screenshot panel](assets/images/CMakeGUI-LLVM_screenshot.png) to see how it is supposed to look.

    CMAKE_INSTALL_PREFIX = C:/vs/install/llvm

Press Configure again and wait until each option has changed color from [Red in panel 2](assets/images/CMakeGUI-LLVM_screenshot.png) to [White in panel 3](assets/images/CMakeGUI-LLVM_screenshot.png). Finally, press Generate.

    Configure > Generate

When done the bottom output window will say 

    Configuring done
    Generating done



## 4.b. CMake Roadrunner Thirdparty
Set the Source Path and Build Path to directories for `roadrunner_thirdparty`

    Source = C:/vs/src/roadrunner/third_party
    Build = C:/vs/build/roadrunner_thirdparty/release

Then click Configure and the default options, listed here:

    Configure >Visual Studio 10 > Use default Native compilers > Finish

Set the following options

    CMAKE_INSTALL_PREFIX = C:/vs/install/roadrunner_thirdparty/release
    CMAKE_BUILD_TYPE     = Release  

Press Configure and then Generate

    Configure > Generate

When done the bottom output window will say 

    Configuring done
    Generating done

Note: The `CMAKE_BUILD_TYPE` flag does not actually control the type of build, its ignored, but set it anyway, otherwise the output says Debug when it is actually Release)

*Note Special Instructions: To also build the C API (below)*



## 4.c. CMake Roadrunner 
Set the Source Path and Build Path to directories for `roadrunner`
These instructions build **with SWIG Python API**

    Source = C:/vs/src/roadrunner
    Build = C:/vs/build/roadrunner/release

Then click Configure and the default options, listed here:

    Configure > Visual Studio 10 > Use default Native compilers > Finish

Set the following options

    CMAKE_INSTALL_PREFIX       = C:/vs/install/roadrunner/release
    LLVM_INSTALL_PREFIX        = C:/vs/install/llvm
    CMAKE_BUILD_TYPE           = Release
    THIRD_PARTY_INSTALL_FOLDER = C:/vs/install/roadrunner_thirdparty/release
    BUILD_LLVM                 = Mark checkbox (yes)
    BUILD_SWIG_PYTHON          = Mark the checkbox (yes)
    SWIG_DIR                   = C:/vs/install/swigwin-2.0.11

Press Configure and then Generate

    Configure > Generate

When done the bottom output window will say 

    Configuring done
    Generating done



# 5. Using Visual Studio
Build and install in Visual Studio: LLVM, roadrunner_thirdparty, roadrunner (outputs to `install/`) 

## 5.a. Build LLVM
Open Solution file in Visual Studio

    File > Open > Project/Solution > C:\vs\build\llvm\LLVM.sln

Set the Configuration and Platform to `Release` and `Win32`, the [screenshot](assets/images/VisualStudio-LLVM_Release_INSTALL_Build.png) shows where these are in Visual Studio.

    Solution Configurations: Release
    Solution Platforms: Win32

Build and Install. In Visual Studio's [Solution Explorer](assets/images/VisualStudio-LLVM_Release_INSTALL_Build.png)

    Right-Click > INSTALL > Build



## 5.b. Build Roadrunner ThirdParty
Open Solution file

    File > Open > Project/Solution > C:\vs\build\roadrunner_thirdparty\release\RRThirdPartyBuild.sln

Set the Configuration and Platform to `Release` and `Win32`

    Solution Configurations: Release
    Solution Platforms: Win32

Build and Install. In Visual Studio's Solution Explorer

    Right-Click > INSTALL > Build



## 5.c. Build Roadrunner
Open Solution file

    File > Open > Project/Solution > C:\vs\build\roadrunner\release\rr.sln

Set the Configuration and Platform to `Release` and `Win32`

    Solution Configurations: Release
    Solution Platforms: Win32

Build and Install. In Visual Studio's Solution Explorer

    Right-Click > INSTALL > Build



#Note Special Instructions: To also build the C API

If building a `Release` version of RoadRunner for C 
The `CMAKE_C_FLAGS_RELEASE` flag and the `CMAKE_CXX_FLAGS_RELEASE` need to be modified from its default. The default switches 

    /O2 /Ob2

needs to be changed to

    /Od /Ob0

This need to be observed for both the ThirdParty build, as well as for RoadRunner itself.

Comment on this Issue: The problem seem to be in the CVODE integration library, not being able to converge properly when full optimization is turned on.

[old_buildrr_visualstudio instructions file](old_buildrr_visualstudio.html)
