cmake_minimum_required(VERSION 2.8)
project(RoadRunner)

SET(RR_MAJOR_VERSION 1)
SET(RR_MINOR_VERSION 0)
SET(RR_BUILD_VERSION 0)
SET(RR_VERSION "${RR_MAJOR_VERSION}.${RR_MINOR_VERSION}.${RR_BUILD_VERSION}")

OPTION (INSTALL_CXX_DOCS 		"Package c++ docs" 	        	OFF)
OPTION (INSTALL_PYTHON_DOCS 	"Package Python docsr" 		    OFF)
OPTION (INSTALL_C_DOCS  		"Package C docs"                OFF)
OPTION (INSTALL_SBML_MODELS		"Install SBML Models"		    ON)
OPTION (BUILD_TRUNK     		"BUILD TRUNK"		            ON)

#------ RELEASES FOLDER
if(${BORLAND})
set(RR_RELEASE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/releases/xe CACHE PATH "Directory containing roadrunner releases.")
if("${RR_RELEASE_FOLDER}" STREQUAL "")
  set(LIBSBML_INSTALL_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/releases/xe CACHE PATH "Directory containing roadrunner releases" FORCE)
endif()
else(${BORLAND})
set(RR_RELEASE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/releases/vs CACHE PATH "Directory containing roadrunner releases.")
if("${RR_RELEASE_FOLDER}" STREQUAL "")
  set(RR_RELEASE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/releases/vs CACHE PATH "Directory containing roadrunner releases" FORCE)
endif()
endif()


#What sub folders to move into...?
if(${INSTALL_SBML_MODELS})
add_subdirectory(Models)
endif()

if(${BUILD_TRUNK})
add_subdirectory(trunk)
endif()

#================ CPACK =========================================================================
#Create distribution Pack
include (InstallRequiredSystemLibraries)

#COMPONENTS OF THE INSTALL
set(CPACK_COMPONENTS_ALL rr_core apps c_api compiled_examples example_files sbml_models Delphi Python misc_docs cpp_docs c_docs python_docs)
#set(CPACK_COMPONENTS_ALL rr_docs)
set(CPACK_COMPONENT_RR_CORE_DISPLAY_NAME					"Core Files")
set(CPACK_COMPONENT_APPS_DISPLAY_NAME						"Applications")
set(CPACK_COMPONENT_C_API_DISPLAY_NAME						"C API DLL")
set(CPACK_COMPONENT_COMPILED_EXAMPLES_DISPLAY_NAME			"Compiled Examples")
set(CPACK_COMPONENT_EXAMPLE_FILES_DISPLAY_NAME				"Examples Source")
set(CPACK_COMPONENT_SBML_MODELS_DISPLAY_NAME				"SBML Models")
set(CPACK_COMPONENT_DELPHI_DISPLAY_NAME						"Delphi Units")
set(CPACK_COMPONENT_PYTHON_DISPLAY_NAME						"Python Module")
set(CPACK_COMPONENT_MISC_DOCS_DISPLAY_NAME					"Misc. Documentation")
set(CPACK_COMPONENT_C_DOCS_DISPLAY_NAME					    "C API Documentation")
set(CPACK_COMPONENT_CPP_DOCS_DISPLAY_NAME				    "C++ API Documentation")
set(CPACK_COMPONENT_PYTHON_DOCS_DISPLAY_NAME				"Python API Documentation")

set(CPACK_PACKAGE_ICON										${CMAKE_CURRENT_SOURCE_DIR}\\\\Roadrunner.bmp)

#NSIS Specific
set(CPACK_NSIS_MUI_ICON									    ${CMAKE_CURRENT_SOURCE_DIR}/RoadRunner.ico)
set(CPACK_NSIS_MUI_UNIICON									${CMAKE_CURRENT_SOURCE_DIR}/RoadRunner.ico)
set(CPACK_NSIS_INSTALL_ROOT									"C:")

#COMPONENTS
set(CPACK_COMPONENT_RR_CORE_DESCRIPTION						"Headers (.h), link library files (.lib) and dynamic link libraries (.dll's)")
set(CPACK_COMPONENT_APPS_DESCRIPTION						"Applications using RoadRunner API")
set(CPACK_COMPONENT_C_API_DESCRIPTION						"C API Wrapper for RoadRunner")
set(CPACK_COMPONENT_COMPILED_EXAMPLES_DESCRIPTION			"This will install pre-compiled binaries for all examples")
set(CPACK_COMPONENT_EXAMPLE_FILES_DESCRIPTION				"Examples Source")
set(CPACK_COMPONENT_SBML_MODELS_DESCRIPTION					"Install various SBML Model files")
set(CPACK_COMPONENT_DELPHI_DESCRIPTION						"Installs RoadRunner Delphi Units")
set(CPACK_COMPONENT_PYTHON_DESCRIPTION						"Installs RoadRunner Python Module")
set(CPACK_COMPONENT_MISC_DOCS_DESCRIPTION					"Misc. documentation, like Readme files etc.")
set(CPACK_COMPONENT_CPP_DOCS_DESCRIPTION					"C++ documentation")
set(CPACK_COMPONENT_C_DOCS_DESCRIPTION				    	"C documentation")
set(CPACK_COMPONENT_PYTHON_DOCS_DESCRIPTION			    	"Python documentation")

#Grouping
set(CPACK_COMPONENT_GROUP_DOCUMENTATION_DESCRIPTION         "Doxygen docs")
set(CPACK_COMPONENT_CPP_DOCS_GROUP                          "Documentation")
set(CPACK_COMPONENT_C_DOCS_GROUP                            "Documentation")
set(CPACK_COMPONENT_PYTHON_DOCS_GROUP                       "Documentation")

#Dependencies
set(CPACK_RESOURCE_FILE_LICENSE								"${CMAKE_CURRENT_SOURCE_DIR}/trunk/License.txt")
set(CPACK_PACKAGE_VENDOR									"UW")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY						"rr library - headers, libraries and examples")
set(CPACK_PACKAGE_VERSION_MAJOR								"${RR_MAJOR_VERSION}")
set(CPACK_PACKAGE_VERSION_MINOR								"${RR_MINOR_VERSION}")
set(CPACK_PACKAGE_VERSION_PATCH								"${RR_BUILD_VERSION}")

set(CPACK_NSIS_INSTALL_ROOT                                 "$PROGRAMFILES") 
#set(CPACK_PACKAGE_INSTALL_DIRECTORY							"RoadRunner")


if(${INSTALL_CXX_DOCS})
set(CPACK_NSIS_MENU_LINKS                                   "Documentation/cpp_api/html/index.html" "C++ Documentation")
endif()

if(${INSTALL_C_DOCS})
set(CPACK_NSIS_MENU_LINKS                                  ${CPACK_NSIS_MENU_LINKS} "Documentation/c_api/html/index.html" "C API Documentation")
endif()

if(${INSTALL_PYTHON_DOCS})
set(CPACK_NSIS_MENU_LINKS                                  ${CPACK_NSIS_MENU_LINKS} "Documentation/python_api/html/index.html" "Python API Documentation")
endif()

#The following packages can be created
# 1) RR_ALL    - This is a full release, containing ALL stuff (except the source)
# 2) RR_CXX     - C++ Distribution, contains relevant CXX files
# 3) RR_C_API   - C API Distribution, contains relevant C API files
# 4) RR_PYTHON  - Python API Distribution
# 5) RR_DELPHI  - Delphi Distribution

# In addition, on the installer page the user will be able to uncheck examples and applications (relevant to each distro)

if(${BORLAND})
set(COMPILER_FOR_PACKAGE "cg")
endif()

if(${MSVC})
set(COMPILER_FOR_PACKAGE "vs")
endif()

if(${MINGW})
set(COMPILER_FOR_PACKAGE "mingw")
endif()

set(CPACK_PACKAGE_FILE_NAME RoadRunner CACHE PATH "Set package file name (without extension).")

MESSAGE( STATUS "PACKAGE NAME: " ${CPACK_PACKAGE_FILE_NAME})


set(CPACK_OUTPUT_FILE_PREFIX								"${CMAKE_CURRENT_SOURCE_DIR}/releases/${COMPILER_FOR_PACKAGE}")
include(CPack)

#======= Docuementation
if(${INSTALL_CXX_DOCS} OR ${INSTALL_C_DOCS} OR ${INSTALL_PYTHON_DOCS})
add_subdirectory(documents)
endif()


