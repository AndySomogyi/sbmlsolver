This project demonstrates creating a Python wrapper for a C library. Most of the work is done by SWIG, which generates the _hello_swig shared library and hello_swig Python module. Then on top of that there's more python code (hello_oo.py) that organizes the generated functions into classes. There's one example function for each wrapping technique I learned from libSBOLc.

Steps to build the wrapper:
1) Download CMake, MinGW, SWIG (swigwin), and Python (version 2.7 is best to start with).
2) Run CMake and put this folder as "Where is the source code".
3) For "where to build the binaries", use this_folder/build (specific folder shouldn't matter but it does because I haven't figured out CMake's INSTALL)
4) Click Configure and choose MinGW makefiles.
5) Now you'll probably have to provide the path to swig.exe manually. Then click Configure until the red goes away, and finally Generate.
6) Launch the MinGW shell, cd into the build folder, and run mingw32-make.exe.

To see the wrapper work, run C:\Python27\python.exe ../examples/demo_swig.py (or demo_oo.py) while still in the build directory. Hopefully it will print some stuff.