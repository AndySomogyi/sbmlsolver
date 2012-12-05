
echo "Running tests..."
@echo off 
set compiler=vs_2010
set build_type=debug
set install_folder=r:\installs\%compiler%\%build_type%
set model_folder=%install_folder%\models
set wc=r:\rrl
set report_file=%wc%\reports\cpp_api_tests-%compiler%.xml
set temp_folder=r:\rrTemp\%compiler%

cxx_api_tests.exe -m%model_folder% -r%report_file% -l%install_folder%\compilers\tcc\tcc.exe -s%install_folder%\rr_support -t%temp_folder%  -d%temp_folder%

echo done...
