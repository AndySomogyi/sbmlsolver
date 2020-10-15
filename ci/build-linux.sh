#!/usr/bin/env bash

current_directory=$(pwd)
echo "Starting working directory: $current_directory"
# get a copy of cmake 3.18.4
wget https://github.com/Kitware/CMake/releases/download/v3.18.4/cmake-3.18.4-Linux-x86_64.tar.gz
tar -zxvf cmake-3.18.4-Linux-x86_64.tar.gz
PATH="$PATH:$(pwd)/cmake-3.18.4-Linux-x86_64/bin"
PATH="$PATH:$(pwd)/cmake-3.18.4-Linux-x86_64/share/cmake-3.18/"
PATH="$PATH:$(pwd)/cmake-3.18.4-Linux-x86_64/share/cmake-3.18/Modules"
PATH="$PATH:$(pwd)/cmake-3.18.4-Linux-x86_64/share/cmake-3.18/include"


# get llvm
wget https://github.com/sys-bio/llvm-6.x/releases/download/release%2F6.x/llvm-6.x-gcc7.5.0-x64-release.tar.gz
tar zxvf llvm-6.x-gcc7.5.0-x64-release.tar.gz
llvm_install_prefix=$(pwd)/llvm-6.x-gcc7.5.0-x64-release
if [ ! -d $llvm_install_prefix ]
then
  echo "Path to llvm: \"$llvm_install_prefix\" does not exist"
  exit 1
fi

echo "llvm installed to: $llvm_install_prefix"



# get a copy of roadrunner-deps
wget https://github.com/CiaranWelsh/roadrunner-deps/releases/download/v2.0.1/roadrunner-deps-gcc7.5-x64-release.tar.gz
tar -zxvf roadrunner-deps-gcc7.5-x64-release.tar.gz
rr_deps_install_prefix=$(pwd)/roadrunner-deps-gcc7.5-x64-release
if [ ! -d $rr_deps_install_prefix ]
then
  echo "Path to roadrunner dependencies : \"$rr_deps_install_prefix\" does not exist"
  exit 1
fi

# download roadrunner deps and build
git clone https://github.com/CiaranWelsh/roadrunner.git

cmake_install_prefix=$(pwd)/install-azure

cd roadrunner || exit
mkdir build
cd build || exit

# pull commands out because they are common to all roadrunner build commands.
# later we want to reuse minimal_nessary_commands when reconfiguring for things link language bindings etc.
minimal_necessary_commands="-DCMAKE_INSTALL_PREFIX=$cmake_install_prefix -DCMAKE_BUILD_TYPE="Release" -DLLVM_INSTALL_PREFIX=$llvm_install_prefix -DRR_DEPENDENCIES_INSTALL_PREFIX=$rr_deps_install_prefix"
$cmake_command  $minimal_necessary_commands ..
$cmake_command --build . --target install --config Release

#run the tests
ctest .


