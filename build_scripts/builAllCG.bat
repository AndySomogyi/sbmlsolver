echo Building RoadRunner packages
cd r:\rrBuilds\xe\c_api
make clean
make install package

cd r:\rrBuilds\xe\cxx_api
make clean
make install package

cd r:\rrBuilds\xe\full
make clean
make install package

echo "Done....."
