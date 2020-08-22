echo Building RoadRunner packages

cd r:\vs_builds\tp_Release
msbuild /p:Configuration=Release INSTALL.vcxproj

cd r:\vs_builds\rr_Release
msbuild /p:Configuration=Release INSTALL.vcxproj

echo "Done....."
PAUSE
