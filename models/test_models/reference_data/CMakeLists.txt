cmake_minimum_required(VERSION 2.8)

PROJECT(ref_data)

set(ref_data_files
)

install(    FILES ${ref_data_files}
            DESTINATION models/test_models/reference/data 	
            COMPONENT testing)
