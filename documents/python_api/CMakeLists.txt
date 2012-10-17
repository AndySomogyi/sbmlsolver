cmake_minimum_required(VERSION 2.8)

#=============== DOXYGEN ====
# add a target to generate API documentation with Doxygen
#find_package(Doxygen)
#if(DOXYGEN_FOUND)
#configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
#add_custom_target(python_api_docs 
#${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
#WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
#COMMENT "Generating API documentation with Doxygen" VERBATIM
#)
#endif(DOXYGEN_FOUND)

#install Python documentation
FILE(GLOB docs ./html/*.*)
install(FILES ${docs} 
        DESTINATION Documentation/python_api/html 
        COMPONENT python_docs
       )

FILE(GLOB search_files ./html/search/*.*)
install(FILES 
        ${search_files} 
        DESTINATION Documentation/python_api/html/search 
        COMPONENT python_docs
       )

