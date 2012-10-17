echo Building RoadRunner packages

cd r:\builds\vs\release\ThirdParty
msbuild /p:Configuration=Release INSTALL.vcxproj

cd r:\builds\vs\release\all
msbuild /p:Configuration=Release RoadRunner.sln /t:clean
msbuild /p:Configuration=Release PACKAGE.vcxproj

cd r:\builds\vs\release\cxx_api
msbuild /p:Configuration=Release RoadRunner.sln /t:clean
msbuild /p:Configuration=Release PACKAGE.vcxproj

cd r:\builds\vs\release\c_api
msbuild /p:Configuration=Release RoadRunner.sln /t:clean
msbuild /p:Configuration=Release PACKAGE.vcxproj

cd r:\builds\vs\release\python
msbuild /p:Configuration=Release RoadRunner.sln /t:clean
msbuild /p:Configuration=Release PACKAGE.vcxproj


echo "Done....."
PAUSE
