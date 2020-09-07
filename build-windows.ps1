# rr-install-windows.ps1. 

# handle input parameters
 param (
    [string]$msvc_root = "C:\Program Files (x86)\Microsoft Visual Studio",
    [string]$msvc_version = "2019",
    [string]$build_config = "Release"
 )

if (-NOT (Test-Path "$msvc_root\$msvc_version")){
	Write-Error "MSVC version folder inside Microsoft Visual Studio root folder not found. 
Was expecting to find $msvc_version. Please make sure the root of your Visual Studio 
installation is correct - or make sure you pass -msvc_root=`"c:\path\to\Microsoft Visual Studio`"".
}

$vcvars64 = "$msvc_root\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
if (-NOT (Test-Path "$vcvars64")){
	Write-Error "Something is wrong with your Visual Studio distribution. vcvars64.bat not found 
in its usual location: $vcvars64"
}

$valid_build_configs = New-Object string[] 2
$valid_build_configs[0] = "Release"
$valid_build_configs[1] = "Debug"


if (-NOT ($valid_build_configs -contains $$build_config)){
    Write-Error "Chosen configuration `"$build_config`" should be either `"Release`" or `"Debug`""
}

# we now open the developers command prompt to intiialize the 
#  environment from which we can build using msvc
cmd /c $vcvars64

# and then switch back to powershell because its nicer.
# by invoking powershell from the developer command prompt, 
# we get the necessary environment variables for correct configuration 
# and building inside powershell. 
powershell

# get LLVM
Invoke-WebRequest "https://github.com/sys-bio/LLVM6.0.1/releases/download/v0.0.1/LLVM6.0.1-windows10-msvc2019.zip"

# unzip archive 
expand-archive -path "LLVM6.0.1-windows10-msvc2019.zip" -destinationpath .

$llvm = "$(pwd)\LLVM6.0.1-windows10-msvc2019"

git clone https://github.com/CiaranWelsh/roadrunner.git
cd roadrunner
git checkout master
mkdir build ; cd build
cmake -DLLVM_INSTALL_PREFIX="$llvm" -DCMAKE_INSTALL_PREFIX="..\install-windows" -DCMAKE_BUILD_TYPE="Release" -A x64 .. 
msbuild /p:configuration=Release rr.sln 

