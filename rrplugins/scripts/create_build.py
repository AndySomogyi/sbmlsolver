import sys
from setupVSEnv import *
from os import *
from os.path import *
import subprocess
import datetime

from pbs import ls
from pbs import git

try:

    root = getcwd()
    repo="telPlugins"
    tpFolder=join(root, "TP")
    installFolder=join(root, "INSTALL")
    #:wchdir(repo)

    #git checkout master
    #git $repoFolder up -v --progress  "origin"

    opt1=" -DBUILD_EXAMPLES:BOOL=TRUE"
    opt2=" -DTHIRD_PARTY_INSTALL_FOLDER:PATH='" + tpFolder
    opt3=" -DCMAKE_INSTALL_PREFIX:PATH='" + installFolder

    cmd ='cmake -HtelPlugins -BTheBuild -G"Visual Studio 11"' + opt1 + opt2 + opt3
    output = subprocess.check_output(cmd, shell=True)


#=== Now do the build
    #setupVSEnv(11)
    VSPath="C:\\Program Files (x86)\\Microsoft Visual Studio 11.0\\VC\\bin"
    vsVars=join(VSPath,"vcvars32.bat")
    #msBuild='C:\\Windows\\Microsoft.NET\\Framework\\v3.5\\msbuild.exe'
    chdir(join(root,'TheBuild'))

    print 'Current dir: ' + getcwd()
    cmd =  vsVars +' &&  msbuild /p:Configuration=Release tellurium.sln /t:clean'
    print cmd
    output = subprocess.check_output(cmd, shell=True)

    #cmd = 'msbuild /p:Configuration=Release INSTALL.vcxproj'
    #output = subprocess.check_output(cmd, shell=True)

##
##    cd ..
##
##    echo "Creating build files ======================"
##    cwd=`pwd`
##    TP_FOLDER=`cygpath -mw $cwd`
##    TP_FOLDER=$TP_FOLDER/TP
##
##    echo "TP Folder: "$TP_FOLDER
##    cmake -HtelPlugins -BTheBuild -G"Visual Studio 11" -DBUILD_EXAMPLES:BOOL=TRUE -DTHIRD_PARTY_INSTALL_FOLDER:STRING=$TP_FOLDER
##    echo "In folder: "`pwd`
##    echo "done"
except Exception as e:
    print e
    print output