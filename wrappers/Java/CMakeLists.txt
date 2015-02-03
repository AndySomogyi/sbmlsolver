include(${CMAKE_CURRENT_SOURCE_DIR}/FindSWIG.cmake) # this is slightly modified FindSWIG.cmake file -  had to tune it to work with OSX

FIND_PACKAGE(SWIG REQUIRED)

INCLUDE(UseSWIG)

FIND_PACKAGE(Java 1.7 REQUIRED COMPONENTS Development)
FIND_PACKAGE(JNI REQUIRED)

message("Java includes: ${Java_INCLUDE_DIRS}")
message("Java libs: ${Java_LIBRARIES}")
message("JNI includes: ${JNI_INCLUDE_DIRS}")
message("JNI libs: ${JNI_LIBRARIES}")

INCLUDE_DIRECTORIES(${Java_INCLUDE_DIRS})
# INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
# INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/include)
# INCLUDE_DIRECTORIES(${RR_SOURCE_PATH}/third_party/rr-libstruct)

IF(SWIG_FOUND)
    add_subdirectory(roadrunner)
ELSE(SWIG_FOUND)
    message("could not find SWIG in your system")
ENDIF(SWIG_FOUND)
