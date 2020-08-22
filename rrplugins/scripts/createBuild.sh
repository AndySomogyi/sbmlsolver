#! bash

echo "Hello"
repo="telPlugins"
cd $repo

git checkout master 
git $repoFolder up -v --progress  "origin"

cmake -BTheBuild -G"Visual Studio 11"
cd ..

echo "Creating build files ======================"
cwd=`pwd`
TP_FOLDER=`cygpath -mw $cwd`
TP_FOLDER=$TP_FOLDER/TP

echo "TP Folder: "$TP_FOLDER
cmake -HtelPlugins -BTheBuild -G"Visual Studio 11" -DBUILD_EXAMPLES:BOOL=TRUE -DTHIRD_PARTY_INSTALL_FOLDER:STRING=$TP_FOLDER
echo "In folder: "`pwd`
echo "done"
