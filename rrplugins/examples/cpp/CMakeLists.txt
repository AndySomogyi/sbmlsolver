cmake_minimum_required(VERSION 2.8)
project(plugins_examples)

# rr Includes
include_directories(
${TLP_ROOT}/source/core
${TLP_ROOT}/source/common
${THIRD_PARTY_INSTALL_FOLDER}/include
${THIRD_PARTY_INSTALL_FOLDER}/include/rr
${THIRD_PARTY_INSTALL_FOLDER}/include/sbml
${THIRD_PARTY_INSTALL_FOLDER}/include/cvode
${THIRD_PARTY_INSTALL_FOLDER}/include/clapack
)

add_subdirectory(cpp_plugin_demo_1)
#add_subdirectory(one_rr)
add_subdirectory(property_meta_data)
