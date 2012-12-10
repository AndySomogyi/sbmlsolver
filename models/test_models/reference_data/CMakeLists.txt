cmake_minimum_required(VERSION 2.8)

PROJECT(ref_data)

set(ref_data_files
SSTest_1.txt
)

install(
FILES ${ref_data_files}
DESTINATION models/test_models/reference_data 	
COMPONENT testing
)
