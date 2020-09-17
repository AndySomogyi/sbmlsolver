set(TESTDIRS
        c_api_core
        model_analysis
        model_editing
        sbml_features
        state_saving
        steady_state
#List the semantic_STS last, since it takes much longer to run than the others.
        semantic_STS
   )

set(GTESTLIB gtest)

#Sadly, googletest gives the debug library a different name, so we have to figure
# out which one is being used
file (GLOB LIBRARIES ${THIRD_PARTY_INSTALL_FOLDER}/lib/*)
foreach(libid IN LISTS LIBRARIES)
    string(FIND ${libid} "gtestd" pos)
    if(pos GREATER_EQUAL 0)
        set(GTESTLIB gtestd)
    endif()
endforeach()

message(STATUS "GTestLib = ${GTESTLIB}")

foreach(dirid IN LISTS TESTDIRS)
    #message(STATUS "dirid = ${dirid}")
    file(GLOB MAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp )
    file(GLOB UTIL_FILE ${CMAKE_CURRENT_SOURCE_DIR}/test_util.cpp )
    file(GLOB UTIL_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/test_util.h )
    file(GLOB CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/${dirid}/*.cpp )
    file(GLOB H_FILES ${CMAKE_CURRENT_SOURCE_DIR}/${dirid}/*.h )

    set(TEST_FILES ${MAIN_FILE} ${UTIL_FILE} ${UTIL_HEADER} ${CPP_FILES} ${H_FILES})

    set(testid "test_${dirid}")
    add_executable(${testid} ${TEST_FILES})

    set_property(TARGET ${testid}  
             PROPERTY  COMPILE_DEFINITIONS 
             LIBSBML_USE_CPP_NAMESPACE
             LIBSBML_STATIC
             STATIC_LIBSTRUCT
             STATIC_PUGI
             STATIC_RR
             STATIC_NLEQ1
             STATIC_NLEQ2
             POCO_STATIC
             XML_STATIC
             POCO_NO_AUTOMATIC_LIBS
     )

     link_directories(
        ${THIRD_PARTY_INSTALL_FOLDER}/lib
     )

     include_directories(
        ${RR_SOURCE_ROOT}
        ${RR_GENERATED_HEADER_PATH}
        ${THIRD_PARTY_INSTALL_FOLDER}/include/
     )

     if(UNIX) #Don't know why static roadrunner fails....?
          set(staticLibPrefix ".a")
          set(sharedLibPrefix ".so")
     endif()

     # This order is important:  on linux, including the core library before the C api library
     # causes the resulting program to crash in Poco.
     target_link_libraries(${testid}
        roadrunner_c_api
        roadrunner-static
        PocoXML
        ${GTESTLIB}
     )
     add_test(${testid}_run ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${testid} )
     set_tests_properties(${testid}_run PROPERTIES ENVIRONMENT 
          "testdir=${RR_ROOT}/test")

endforeach()
