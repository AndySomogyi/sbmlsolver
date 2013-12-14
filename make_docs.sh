#!/bin/sh

# lame ass quick hack to make the docs
cwd=`pwd`

cd source 
doxygen doxygen.conf
cd $cwd


cd wrappers/C
doxygen core_c_api.doxy
cd $cwd

make -f wrappers/Python/doc/Makefile html

cd plugins/source
doxygen Doxyfile_cpp_api.doxy
cd $cwd

cd plugins/wrappers/C
doxygen Doxyfile_c_api.doxy
cd $cwd

cd plugins/wrappers/Python
doxygen Doxyfile
cd $cwd
