include(${CMAKE_CURRENT_SOURCE_DIR}/FindSWIG.cmake) # this is slightly modified FindSWIG.cmake file -  had to tune it to work with OSX

FIND_PACKAGE(SWIG 3.0.0 REQUIRED)


INCLUDE(UseSWIG)

find_package(PythonInterp)
# specify that the same python library should be found 
# as the one the selected interpreter uses
set (Python_ADDITIONAL_VERSIONS ${PYTHON_VERSION_STRING})
find_package(PythonLibs)
FIND_PACKAGE(NumPy)

MESSAGE ("THIS IS NUMPY_VERSION " ${NUMPY_VERSION})

MESSAGE ("THIS IS NUMPY_INCLUDE_DIRS " ${NUMPY_INCLUDE_DIRS})

MESSAGE ("THIS IS PYTHON_INCLUDE_PATH " ${PYTHON_INCLUDE_PATH})

INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/include)
INCLUDE_DIRECTORIES(${NUMPY_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${RR_SOURCE_PATH}/third_party/rr-libstruct)



SET(PYINTERFACE_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/pyinterface)
SET(CMAKE_SWIG_FLAGS "")



IF(SWIG_FOUND)
    add_subdirectory(roadrunner)
ELSE(SWIG_FOUND)
    message("could not find SWIG in your system")
ENDIF(SWIG_FOUND)

if( SWIG_VERSION STRGREATER "4.0")
  message( SEND_ERROR "SWIG version too high:  unable to use SWIG version 4.0 and higher." )
endif( SWIG_VERSION STRGREATER "4.0")


install(FILES INSTALL.txt setup.py MANIFEST.in 
        DESTINATION .    
       )

if(WITH_CONDA_BUILDER)
  configure_file(conda/meta.yaml.in conda/meta.yaml @ONLY)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/conda/meta.yaml conda/build.sh conda/bld.bat DESTINATION .)
endif()
