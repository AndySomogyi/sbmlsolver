---
layout: page
title: "Build RR on Ubuntu CMake GUI"
description: ""
---
{% include JB/setup %}

The following steps need to be taken in order to build and install RoadRunner on a Ubuntu platform.

This procedure has been tested on

* An Ubuntu distribution, running in a virtual machine using !Oracle VirtualBox environment
* A native Ubuntu 12.04 installation on x86_64

##Prepare for installation
In a terminal create a folder that will contain roadrunner related files. In the following this folder is located in a users home directory and named rr.

The following applications and libraries need to be available on the system:

* gcc/g++
* libxml2
* libxml2-dev
* tcc
* svn
* cmake-gui
* zliblg-dev
* liblzma-dev

##Checkout RoadRunner code
Clone RoadRunner source files from GitHub, to folder ~/rr/roadrunnerlib using git:

    git clone https://github.com/AndySomogyi/roadrunner.git rr/roadrunnerlib

##Create build folder
To build and install RoadRunner, build files need to be generated using CMake. Currently RoadRunner is build in two passes,

1) Thirdparty code is first built and installed
2) RoadRunner library and applications are built and installed.

##Build and Install RoadRunner ThirdParties Using the CMake GUI
Open a CMake UI and select source and build folders. The source folder will be /home/yourName/rr/roadrunnerlib/ThirdParties and the build folder should be something like /home/yourName/rr/builds/ThirdParty Make an effort to keep the build folder out of the source directory.

Then Press "Configure"

Specify UNIX makefiles as the generator, and for the follow up question select 'Use default native compilers'.

* Set CMAKE_BUILD_TYPE to Release
* Set CMAKE_INSTALL_PREFIX to /usr/local
When there are no red records shown in the CMake UI, press the Generate button. This step will create Makefiles in the build folder that you selected earlier.

Open a terminal, and cd into ~/rr/builds/ThirdParty and type

    sudo make install

This should make and install necessary ThirdParty libraries into the /usr/local folder.

## Step 2: Build RoadRunner
## Build and Install Roadrunner
Open a CMake UI and select source code and build folders as in the screenshot below.

As for the ThirdParty build, select UNIX make files.

Among the UI options, the only needed change is to set the THIRD_PARTY_INSTALL_FOLDER to /usr/local

When there are no red records shown in the CMake UI, press the Generate button. This step will create Makefiles in the build folder that you selected earlier.

Open a terminal, and cd into ~/rr/builds/!rr and type

    sudo make install

##Step 3: Confirm install targets
Binary targets are installed in the bin folder in where you installed RoadRunner. The command line application rr should be available in this folder.

As this is a beta page, feel free to email comments and suggestions!
