---
layout: page
title: "Linux Build"
description: "How to Build Roadrunner on linux"
---
{% include JB/setup %}

#Distribution
Select your distribution:

* [Ubuntu CMake GUI](buildrr_ubuntu_cmakegui.html) or [Ubuntu CMake CLI](buildrr_ubuntu_cmakecli.html) (last tested with 12.10)
* [Fedora](buildrr_fedora.html) (last tested with fc17)

##IMPORTANT NOTES FOR ALL LINUX BUILDS
* After Installing RoadRunner, it may complain about not being able to find a `gcc`. To fix this a soft linkage to your system's `gcc` can be created with the following steps:
    1. In the terminal, enter the command which gcc, and note the path that returns. It is typically something like /usr/bin/gcc.
    2. create the soft link with the following command `ln -s </path/to/your/gcc> </path/to/rr/install>`, using the path to your system's gcc and the path to your RoadRunner install directory.
* If you choose to **NOT** install RoadRunner to the default folder, `/usr/local`, the following apply:
1. The command line client rr_c (C-api client of RoadRunner) need to receive the install path to where RoadRunner was installed, using the flag "-i", e.g. -i/some/non/standard_path
2. To use Python with a non-standard RoadRunner install path, the python module script rrPython.py needs to be modified. On the line starting with #handle.setInstallFolder(..): Uncomment the line and supply proper path to where RoadRunner was installed.


If you don't make the above changes you will get errors like this: `gcc: error: /usr/local/rr_support/rrSupport.c: No such file or directory`

NOTE: Roadrunner do not work fully on Linux 32 bit systems in release mode. To have a working distribution in 32 bit release mode you need to change the CMAKE C flags: cmake_c_flags_release to -O0 -NDEBUG.
