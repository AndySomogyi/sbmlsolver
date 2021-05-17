##################################################################
# Builds the TestModelFactory
# -----------------------------
# Targets:
#   - TestModelFactoryCXX: static library, linked by C++ tests.
#   - TestModelFactory: Python bindings to the TestModelFactory, used in Python tests
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


