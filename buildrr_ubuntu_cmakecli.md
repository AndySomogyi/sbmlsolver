---
layout: page
title: "Build Roadrunner on Ubuntu CMake CLI"
description: ""
---
{% include JB/setup %}

The following steps need to be taken in order to build and install RoadRunner on an Ubuntu platform.

This procedure has been tested on

* An Ubuntu distribution, running in a virtual machine using !Oracle VirtualBox environment
* A native Ubuntu 12.04 installation on x86_64
* An Amazon EC2 instance running 64-bit 12.10 (ami-9465dbfd)

##Prepare for installation
In a terminal create a folder that will contain roadrunner related files. In the following this folder is located in a users home directory and named rr.

The following applications and libraries need to be available on the system:

* gcc/g++
* libxml2
* libxml2-dev
* subversion
* make
* cmake-gui
* zliblg-dev
* liblzma-dev

## Checkout RoadRunner code
Checkout RoadRunner source files from google code using subversion:

    svn checkout http://roadrunnerlib.googlecode.com/svn/trunk roadrunnerlib

This will create a directory roadrunnerlib and put all the project code in it.

## Create build folder
Currently, RoadRunner is built in two passes:

1) Thirdparty code is first built and installed
2) RoadRunner library and applications are built and installed.

##Build and Install RoadRunner ThirdParties Using the Command Line
In-source builds are not supported. Create two directories alongside roadrunnerlib named build and install. In the build directory, create ThirdParty and all directories.

###Step 1: Build Third Party Libraries
cd to build/Thirdparty and run

    cmake ../../roadrunnerlib/ThirdParty/ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=your_rr_directory/install/unified -DCMAKE_CXX_FLAGS="-fPIC" -DCMAKE_C_FLAGS="-fPIC" -DBUILD_SHARED_LIBS=ON
where "your_rr_directory" is the directory containing roadrunnerlib.

Experimental: relative paths might work (if they get completed by CMake):

    cmake ../../roadrunnerlib/ThirdParty/ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=../../install/unified -DCMAKE_CXX_FLAGS="-fPIC" -DCMAKE_C_FLAGS="-fPIC" -DBUILD_SHARED_LIBS=ON
After CMake finishes, run make && make install.

###Step 2: Build and Install Roadrunner
cd to build/all and run

    cmake ../../roadrunnerlib/ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=your_rr_directory/install/unified -DCMAKE_CXX_FLAGS="-fPIC" -DCMAKE_C_FLAGS="-fPIC" -DBUILD_SHARED_LIBS=ON -DTHIRD_PARTY_INSTALL_FOLDER=your_rr_directory/install/unified
Or, the experimental relative path version:

    cmake ../../roadrunnerlib/ -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=../../install/unified -DCMAKE_CXX_FLAGS="-fPIC" -DCMAKE_C_FLAGS="-fPIC" -DBUILD_SHARED_LIBS=ON -DTHIRD_PARTY_INSTALL_FOLDER=your_rr_directory/install/unified
Again run make && make install after CMake finishes.

###Linking GCC
* After Installing RoadRunner, it may complain about not being able to find a gcc. To fix this a soft linkage to your system's gcc can be created with the following steps:
1. In the terminal, enter the command which gcc, and note the path that returns. It is typically something like /usr/bin/gcc.
2. create the soft link with the following command `ln -s /path/to/your/gcc /path/to/rr/install`, using the path to your system's gcc and the path to your RoadRunner install directory.

###Comment by project member stanle...@gmail.com, Dec 17, 2012
I think the linuxSBMLTest branch has been merged so the svn checkout command should be updated to svn checkout http://roadrunnerlib.googlecode.com/svn/trunk roadrunnerlib
