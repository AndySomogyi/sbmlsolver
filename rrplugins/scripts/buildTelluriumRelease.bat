echo Building RoadRunner packages

REM INSTALL ThirdParties
cd vs_builds\tp_Release
msbuild /p:Configuration=Release INSTALL.vcxproj
cd ..
cd ..

REM INSTALL RoadRunner
cd vs_builds\rr_Release
msbuild /p:Configuration=Release INSTALL.vcxproj
cd ..
cd ..

REM INSTALL Tellurium plugins
cd vs_builds\tlp_Release
msbuild /p:Configuration=Release INSTALL.vcxproj
cd ..
cd ..

echo "Done....."
PAUSE
