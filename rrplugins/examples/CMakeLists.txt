cmake_minimum_required(VERSION 2.8)
project(plugins_examples)

#Give access to headers in the following folders
include_directories(
	${TLP_ROOT}/source
	${TLP_ROOT}/wrappers/C
 	${THIRD_PARTY_INSTALL_FOLDER}/include
 	${THIRD_PARTY_INSTALL_FOLDER}/include/rr
 	${THIRD_PARTY_INSTALL_FOLDER}/include/rr/C
)

OPTION (INSTALL_C_EXAMPLES              "Build and Install Tellurium Plugin C Examples"         OFF)
OPTION (INSTALL_CPP_EXAMPLES            "Build and Install Tellurium Plugin CPP Examples"       OFF)
OPTION (INSTALL_PYTHON_EXAMPLES         "Install Tellurium Plugin PYTHON Examples"    OFF)

if(INSTALL_C_EXAMPLES)
add_subdirectory(c)
endif()

if(INSTALL_CPP_EXAMPLES)
add_subdirectory(cpp)
endif()

if(INSTALL_PYTHON_EXAMPLES)
add_subdirectory(python)
endif()

install(
    FILES README.txt
    DESTINATION examples
)
