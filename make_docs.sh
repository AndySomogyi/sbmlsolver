#!/bin/sh

# lame ass quick hack to make the docs
# this 'script' is not intended to live any longer than a 
# week or so utill we get a system down to make docs.

cwd=`pwd`

cd source 
doxygen doxygen.conf
cd $cwd


cd wrappers/C
doxygen core_c_api.doxy
cd $cwd

cd  wrappers/Python/doc
make html
cd $cwd

