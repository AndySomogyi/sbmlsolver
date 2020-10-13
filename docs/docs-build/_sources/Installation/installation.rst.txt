=====================
Installing RoadRunner
=====================

Python Front End
===================

The recommended way to use roadrunner is by installing the Python front end and writing Python scripts.

.. code-block:: bash

   $ pip install libroadrunner

.. todo:

    Find out whether we support conda package?

C/C++ Front End
===================

If you want to use roadrunner from C++ or C, you may be able to use a prebuilt binary distribution
if one is suitable. To determine suitability, ensure your compiler, architecture (i.e. x86 vs x64)
and build configuration (i.e. Release or Debug) matches what was used to build the binaries.

.. todo::

    Put roadrunner binaries on sourceforce and update the links below

* `VisualStudio 2019 x64 release <>`_
* `VisualStudio 2019 x64 debug<>`_
* `GCC7.5.0 x64 release<>`_
* `GCC7.5.0 x64 debug<>`_
* `AppleClang11.0.3 darwin19.4.0 x86-64 release<>`_
* `AppleClang11.0.3 darwin19.4.0 x86-64 debug<>`_

If you cannot use the prebuilt binaries or want to built from source please see the following instructions.

Building Roadrunner from Source
==================================


.. toctree::
    :maxdepth: 1

    building_python_bindings
    building_plugins
    windows_warning
    llvm_dependency

You will need:

- `CMake <https://cmake.org/download/>`_ version > 3.14. The build system was developed with version 3.18.4.
- `Visual studio <https://visualstudio.microsoft.com/downloads/>`_ (windows only) with the C++ package installed.
- `git <https://git-scm.com/book/en/v2/Getting-Started-Installing-Git>`_

If you are new to CMake, we recommend you read the `cmake tutorial <https://cmake.org/cmake/help/latest/guide/tutorial/index.html>`_ to get the basics before continuing.

.. todo::

    Following sections: Building the plugins, Building the Python Bindings,
    Building the tests, Running the tests

.. note::

    If at any point you get stuck or encounter an error please first read the :ref:`Troubleshooting the build <Troubleshooting the build>`
    page and then if the problem is not resolved, submit a github issue and we'll be more than willing to help.

Ensure you have read the :ref:`windows warnings <A warning for windows users>`.

#. Install the :ref:`llvm-6.x dependency<LLVM-6.x dependency>`.
#. Install the `roadrunner dependency package <https://github.com/CiaranWelsh/roadrunner-deps>`_ by executing the following commands in a
windows/linux/mac shell.

.. code-block:: bash

    $ git clone https://github.com/CiaranWelsh/roadrunner-deps.git --recurse-submodules 	# get the dependency package using git
    $ cd roadrunner-deps-install
    $ mkdir build
    $ cd build
    $ cmake -DCMAKE_INSTALL_PREFIX="D:\roadrunner-deps\roadrunner-deps-install" -DCMAKE_BUILD_TYPE="Release" .. # configure the dependency package
    $ cmake --build . --target install --config Release # build the install target
    $ cd ../../ # return to directory containing roadrunner-deps

#. Install roadrunner using the following commands from a windows/linux/mac shell.

.. warning::

    Remember to substitute the values of `-DCMAKE_INSTALL_PREFIX` AND `-DLLVM_INSTALL_PREFIX` with
    values that work for you own system.

.. code-block:: bash

    $ git clone https://github.com/CiaranWelsh/roadrunner.git # get roadrunner
    $ cd roadrunner
    $ mkdir build
    $ cd build
    $ cmake -DCMAKE_INSTALL_PREFIX="D:\roadrunner\install-msvc2019" -DLLVM_INSTALL_PREFIX="D:\llvm-6.x\llvm\llvm-6.x-msvc-x64-release" -DRR_DEPENDENCIES_INSTALL_PREFIX="D:\roadrunner-deps\roadrunner-deps-install" -DCMAKE_BUILD_TYPE="Release" ..
    $ cmake --build . --target install --config Release

Roadrunner Optional Features
=============================

These are controlled by turning on or off desired options in cmake.

.. note::

    Generally speaking, the cmake-gui tool
    is the best way to get a good oversight of the available options.

Build the Python Bindings
-------------------------

#. Download `<SWIG version 3.0.0 <https://sourceforge.net/projects/swig/files/>`_ for linux,
macos or windows. It is just a zip file - decompress it and put it where you want it. SWIG is now installed and
the swig executable is under the top level swig directory.

#. Configure or reconfigure cmake using the `-DBUILD_PYTHON=ON` option. If you installed swig
globally or add the swig directory to the PATH environment variable, you will *not* need
the `-DSWIG_EXECUTABLE` argument as `swig.exe` will be found
automatically. However in most circumstances you will need to provide the path to the `swig.exe` using
`-DSWIG_EXECUTABLE`.

.. code-block:: bash

    $ cmake -DCMAKE_INSTALL_PREFIX="D:\roadrunner\install-msvc2019" \
        -DLLVM_INSTALL_PREFIX="D:\llvm-6.x\llvm\llvm-6.x-msvc-x64-release" \
        -DRR_DEPENDENCIES_INSTALL_PREFIX="D:\roadrunner-deps\roadrunner-deps-install" \
        -DCMAKE_BUILD_TYPE="Release" \
        -DBUILD_PYTHON=ON
        -DSWIG_EXECUTABLE="C:\Users\Ciaran\Documents\swigwin-3.0.0\swig.exe" ..
    $ cmake --build . --target install --config Release


Building the Roadrunner Plugins
================================

To build the plugins use the `-DBUILD_RR_PLUGINS=ON`. When set to on, the `BUILD_RR_PLUGINS`
option automatically turns on a set of variables. These include:

    * RR_PLUGINS_BUILD_SHARED_LIB
    * RR_PLUGINS_BUILD_C_API
    * RR_PLUGINS_BUILD_PYTHON_API
    * RR_PLUGINS_BUILD_TEST_MODEL_PLUGIN
    * RR_PLUGINS_BUILD_ADD_NOISE_PLUGIN
    * RR_PLUGINS_BUILD_CHISQUARE_PLUGIN
    * RR_PLUGINS_BUILD_AUTO2000_PLUGIN
    * RR_PLUGINS_BUILD_HELLO_PLUGIN

while the `RR_PLUGINS_BUILD_STATIC_LIB` option remains OFF.

.. code-block:: bash

    $ cmake -DCMAKE_INSTALL_PREFIX="D:\roadrunner\install-msvc2019" \
        -DLLVM_INSTALL_PREFIX="D:\llvm-6.x\llvm\llvm-6.x-msvc-x64-release" \
        -DRR_DEPENDENCIES_INSTALL_PREFIX="D:\roadrunner-deps\roadrunner-deps-install" \
        -DCMAKE_BUILD_TYPE="Release" \
        -DBUILD_RR_PLUGINS=ON
    $ cmake --build . --target install --config Release







Troubleshooting the build
===========================

Section on CMake bin directory being in the path environment

Section on forgetting the --recurse-submodules flag on git clone


