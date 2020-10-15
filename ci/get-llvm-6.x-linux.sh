#!/usr/bin/env bash
current_directory=$(pwd)
echo "$current_directory"
wget https://github.com/sys-bio/llvm-6.x/releases/download/release%2F6.x/llvm-6.x-gcc7.5.0-x64-release.tar.gz
tar zxvf llvm-6.x-gcc7.5.0-x64-release.tar.gz
llvm_install_prefix=$(pwd)/llvm-6.x-gcc7.5.0-x64-release
if [ ! -d $llvm_install_prefix ]
then
  echo "Path to llvm: \"$llvm_install_prefix\" does not exist"
  exit 1
fi

echo "llvm installed to: $llvm_install_prefix"










