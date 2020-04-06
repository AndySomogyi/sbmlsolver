#!/bin/bash

###############################################################
# Automated download and build script for libRoadrunner.      #
# Navigate to desired directory first, which should be empty, #
# and run script. This will download source files, configure, #
# and compile them. Runtime may be several hours.             #
# BEFORE RUNNING: Make sure that libxml2 is installed.        #
###############################################################


shopt -s expand_aliases								# allow aliases in shell script

############################################################
#            Detect Windows vs non-Windows                 #
# This section defines two aliases: RRBmake compiles and   #
# RRBmakeinstall installs. They equal 'make' and 'make     #
# install' normally, but use MSBuild.exe for Windows.      #
############################################################
RRBsystem=$(echo "$(uname)" | cut -c1-5)
if [ $RRBsystem == "MINGW" ]; then
	echo "=== Detected Windows system ==="
	RRBmsbuild=$(find '/c/Program Files (x86)' -name "MSBuild.exe" -print -quit)
	RRBmakestr="\""$RRBmsbuild"\" *.sln"
	alias RRBmake=$RRBmakestr
	RRBmakestr="\""$RRBmsbuild"\" INSTALL.vcxproj"
	alias RRBmakeinstall=$RRBmakestr
else
	echo "=== Detected non-Windows system ==="
	alias RRBmake='make'
	alias RRBmakeinstall='make install'
fi

###########################
#   Prepare directories   #
###########################
echo "=== Creating directories ==="
mkdir -p build														# in <root>
mkdir -p install
mkdir -p source
cd build																	# <root>/build
mkdir -p llvm
mkdir -p libroadrunner-deps
mkdir -p roadrunner
cd ../install															# <root>/install
mkdir -p llvm
mkdir -p roadrunner
cd ..																			# <root>

###########################
#         LLVM            #
###########################
if ls install/llvm/lib/*LLVMCore* 1> /dev/null 2>&1; then
	echo "LLVM already exists. Do you want to rebuild it (y/n)?"
	read rebuild
else
	rebuild="y"
fi

if [ $rebuild == "y" ]; then
	cd source																	# <root>/source
	echo "=== Downloading LLVM ==="
	wget "https://releases.llvm.org/6.0.1/llvm-6.0.1.src.tar.xz"
	echo "=== Unpacking LLVM ==="
	tar -xvf llvm-6.0.1.src.*
	rm llvm-6.0.1.src.*
	cd ../build/llvm													# <root>/build/llvm
	echo "=== Configuring LLVM ==="
	cmake ../../source/llvm-6.0.1.src/ -DCMAKE_INSTALL_PREFIX=../../install/llvm
	echo "=== Building LLVM ==="
	RRBmake
	echo "=== Installing LLVM ==="
	RRBmakeinstall
	cd ../..																	# <root>
	if ls install/llvm/lib/*LLVMCore* 1> /dev/null 2>&1; then
		echo "LLVM built successfully"
	else
		echo "LLVM build failed"
		exit 0
	fi
fi

###########################
#   libroadrunner-deps    #
###########################
if ls install/roadrunner/lib/libsbml* 1> /dev/null 2>&1; then
	echo "libroadrunner-deps already exists. Do you want to rebuild it (y/n)?"
	read rebuild
else
	rebuild="y"
fi

if [ $rebuild == "y" ]; then
	cd source																	# <root>/source
	rm -r libroadrunner-deps
	echo "=== Downloading libroadrunner-deps ==="
	git clone https://github.com/sys-bio/libroadrunner-deps.git
	cd ../build/libroadrunner-deps						# <root>/build/libroadrunner-deps
	echo "=== Configuring libroadrunner-deps ==="
	if [ $RRBsystem == "MINGW" ]; then
		cmake ../../source/libroadrunner-deps/ -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../../install/roadrunner -DCMAKE_MC_COMPILER="/c/Program Files (x86)/Windows Kits/10/bin/10.0.18362.0/x64/mc.exe"
	else
		cmake ../../source/libroadrunner-deps/ -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../../install/roadrunner
	fi
	echo "=== Building libroadrunner-deps ==="
	RRBmake
	echo "=== Installing libroadrunner-deps ==="
	RRBmakeinstall
	cd ../..																	# <root>
	if ls install/roadrunner/lib/libsbml* 1> /dev/null 2>&1; then
		echo "libroadrunner-deps built successfully"
	else
		echo "libroadrunner-deps build failed"
		exit 0
	fi
fi

###########################
#       roadrunner        #
###########################
if ls install/roadrunner/lib/*roadrunner* 1> /dev/null 2>&1; then
	echo "roadrunner already exists. Do you want to rebuild it (y/n)?"
	read rebuild
else
	rebuild="y"
fi

if [ $rebuild == "y" ]; then
	cd source																	# <root>/source
	rm -r roadrunner
	echo "=== Downloading roadrunner ==="
	git clone https://github.com/sys-bio/roadrunner.git
	cd ../build/roadrunner										# <root>/build/roadrunner
	echo "=== Configuring roadrunner ==="
	cmake ../../source/roadrunner/ -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../../install/roadrunner/ -DLLVM_CONFIG_EXECUTABLE=../../install/llvm/bin/llvm-config -DTHIRD_PARTY_INSTALL_FOLDER=../../install/roadrunner/ -DRR_USE_CXX11=OFF -DUSE_TR1_CXX_NS=OFF -DCMAKE_CXX_FLAGS="-lcurses"
	echo "=== Building roadrunner ==="
	RRBmake
	echo "=== Installing roadrunner ==="
	RRBmakeinstall
	cd ../..																	# <root>
	if ls install/roadrunner/lib/*roadrunner* 1> /dev/null 2>&1; then
		echo "Roadrunner built successfully"
		echo "Roadrunner is installed in the 'install/roadrunner' directory."
	else
		echo "Roadrunner build failed"
		exit 0
	fi
fi

