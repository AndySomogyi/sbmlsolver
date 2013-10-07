---
layout: page
title: "Fedora Build"
description: ""
---
{% include JB/setup %}

The following steps need to be taken in order to build and install RoadRunner on a Fedora, 32 bit platform.

The deployment steps below was performed on a Fedora distribution, running in a virtual machine using Oracle VirtualBox environment.

##Prepare for installation
In a terminal create a folder that will contain roadrunner related files. In the following this folder is located in a users home directory and named rr.

The following applications and libraries need to be available on the system:

* gcc/g++
* libxml2-devel
* libxml2-static
* tcc
* svn
* cmake-gui

Use 'yum install' to install the above.

To install tcc on Fedora 32 bit, you need to follow the instructions here [http://pkgs.org/fedora-17/rpm-sphere-i586/tcc-0.9.25-3.1.i686.rpm.html](http://pkgs.org/fedora-17/rpm-sphere-i586/tcc-0.9.25-3.1.i686.rpm.html)

## Checkout RoadRunner code
Clone RoadRunner source files from google code, to folder ~/rr/rrl using git:

    git clone https://github.com/AndySomogyi/roadrunner.git rr/rrl

## Create build folder
To build and install RoadRunner, build files need to be generated using CMake. Currently RoadRunner is build in two passes,

1) Thirdparty code is first built and installed
2) RoadRunner library and applications are built and installed.

## Build and Install RoadRunner ThirdParties
Open a CMake UI and select source and build folders. The source folder will be /home/yourName/rr/rrl/ThirdParties and the build folder should be something like /home/yourName/rr/builds/ThirdParty Make an effort to keep the build folder out of the source directory.

Then Press "Configure"

Specify UNIX makefiles as the generator, and for the follow up question select 'Use default native compilers'.

* Set CMAKE_BUILD_TYPE to Debug (Release build don't work on Fedora yet)
* Set CMAKE_INSTALL_PREFIX to /usr/local


When there are no red records shown in the CMake UI, press the Generate button. This step will create Makefiles in the build folder that you selected earlier.

Open a terminal, and cd into ~/rr/builds/ThirdParty and type

    sudo make install

This should make and install necessary ThirdParty libraries into the /usr/local folder.

## Build and Install Roadrunner

Open a CMake UI and select the source code folder ~/rr/rrl and build folder, e.g. ~/rr/builds/rr.

As for the ThirdParty build, select UNIX make files.

Among the UI options, the only needed change is to set the THIRD_PARTY_INSTALL_FOLDER to /usr/local

As this is a beta page, feel free to email comments and suggestions.

## Testing
You can test the installation of RoadRunner using the command line client program rr. See the rr_application page for more information.
