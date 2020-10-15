#!/usr/bin/env bash

# get a copy of cmake 3.18.4
wget https://github.com/Kitware/CMake/releases/download/v3.18.4/cmake-3.18.4-Linux-x86_64.tar.gz
tar -zxvf cmake-3.18.4-Linux-x86_64.tar.gz
cmake_command=$(pwd)/cmake-3.18.4-Linux-x86_64/bin/cmake

# get a copy of roadrunner-deps
#wget https://github.com/Kitware/CMake/releases/download/v3.18.4/cmake-3.18.4-Linux-x86_64.tar.gz
#tar -zxvf cmake-3.18.4-Linux-x86_64.tar.gz
#cmake_command=$(pwd)/cmake-3.18.4-Linux-x86_64/bin/cmake

# download roadrunner deps and build
git clone https://github.com/CiaranWelsh/roadrunner.git
cd roadrunner-deps || exit
cmake_install_prefix=$(pwd)/install-azure
mkdir build
cd build || exit
$cmake_command -DCMAKE_INSTALL_PREFIX=$cmake_install_prefix -DCMAKE_BUILD_TYPE="Release" -DLLVM_INSTALL_PREFIX=$LLVM_INSTALL_PREFIX -DRR_DEPENDENCIES_INSTALL_PREFIX=$RR_DEPS_INSTALL_PREFIX ..
$cmake_command --build . --target install --config Release



