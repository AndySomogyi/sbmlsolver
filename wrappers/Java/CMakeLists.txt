include(${CMAKE_CURRENT_SOURCE_DIR}/FindSWIG.cmake) # this is slightly modified FindSWIG.cmake file -  had to tune it to work with OSX

# https://github.com/shenshei/cmake-swig-java-example

find_package(SWIG REQUIRED)
find_package(Java 1.7 REQUIRED COMPONENTS Runtime Development)
find_package(JNI REQUIRED)
include(UseSWIG)
include(UseJava)

message("Java includes: ${Java_INCLUDE_DIRS}")
message("Java libs: ${Java_LIBRARIES}")
message("JNI includes: ${JNI_INCLUDE_DIRS}")
message("JNI libs: ${JNI_LIBRARIES}")

include_directories(${Java_INCLUDE_DIRS})
include_directories(${JNI_INCLUDE_DIRS})
# INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
# INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/include)
# INCLUDE_DIRECTORIES(${RR_SOURCE_PATH}/third_party/rr-libstruct)

if(SWIG_FOUND)
    add_subdirectory(roadrunner)
else(SWIG_FOUND)
    message("could not find SWIG in your system")
endif(SWIG_FOUND)
