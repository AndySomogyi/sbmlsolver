##################################################################
# Builds the TestModelFactory
# -----------------------------
# Targets:
#   - TestModelFactoryCXX: static library, linked by C++ tests.
#   - TestModelFactory: Python bindings to the TestModelFactory, used in Python tests
#
# Instructions for adding a new test model
#   1) write your test model
#        - it should inherit from at least the TestModel class defined in TestModelInterface.h
#        - It should also inherit from any of the other interfaces that the model is designed to test for. For instance, some models are geared towards testing steady state output and only inherit from SteadyStateResult, whilst others might also inherit from TimeSeriesResult.
#        - Make sure all inherited classes and methods are public
#   2) Include the name of the new test model in TestModelFactory.h
#   3) Add name of new test model to getAvailableTestModels()
#   4) Add an if else to TestModelFactory();
#   4) Include the new test model in CMake target cxxTestModelFactory <rr_root>/test/test-model-factory/CMakeLists.txt
#   5) Add an %include "nameOfTestModel.h" in TestModelFactory.i next to all the others

set(target cxxTestModelFactory)
add_library(${target} STATIC
        TestModelFactory.h
        TestModelFactory.cpp
        TestModelInterface.h
        TestModelInterface.cpp
        Brown2004
        CeilInRateLaw
        FactorialInRateLaw
        Model28
        Model269
        OpenLinearFlux
        SimpleFlux
        SimpleFluxManuallyReduced
        Venkatraman2010
        LayoutOnly
        PythonAPITestsModel
        )

target_include_directories(${target} PUBLIC $<TARGET_PROPERTY:roadrunner-static,INCLUDE_DIRECTORIES>)
target_link_libraries(${target} PUBLIC $<TARGET_PROPERTY:roadrunner-static,LINK_LIBRARIES>)
add_dependencies(${target} roadrunner-static)

if (BUILD_PYTHON AND BUILD_TESTS)
    message(STATUS "Building TestModelFactory")
    set(target pyTestModelFactory)
    add_swig_executable(${target}
            "${CMAKE_CURRENT_SOURCE_DIR}/TestModelFactory.i"
            "../../wrappers/Python/roadrunner/PyUtils"
            )

    target_link_libraries(${target} PUBLIC cxxTestModelFactory)

    add_dependencies(${target} roadrunner-static)
    configure_file(testTestModelFactory.py "${RR_PYTHON_TESTING_DIR}" COPYONLY)

    # install python test files to install tree
    install(TARGETS pyTestModelFactory DESTINATION site-packages/roadrunner/testing)
    install(TARGETS cxxTestModelFactory DESTINATION lib)
    install(
            FILES
                "${CMAKE_CURRENT_BINARY_DIR}/pyTestModelFactory.py"
                "${CMAKE_CURRENT_SOURCE_DIR}/testTestModelFactory.py"
            DESTINATION
            site-packages/roadrunner/testing
    )

endif ()


