# == PREAMBLE ================================================
# * Licensed under the Apache License, Version 2.0; see README

# == FILEDOC =================================================

# @file autotest/CMakeLists.txt
# @author JKM(0u812)
# @date 07/01/2015
# @copyright Apache License, Version 2.0
# @brief Unit tests
# @details Tests can be run via ctest -VV

if (BUILD_PYTHON)
    # performance benchmark (adapted from published benchmark in bioinformatics, 2015)
    file(COPY python-benchmark-bioinf DESTINATION .)

    # update when source changes
    configure_file(python-benchmark-bioinf/rr_bench_ode.py
            ${CMAKE_CURRENT_BINARY_DIR}/python-benchmark-bioinf/rr_bench_ode.py COPYONLY)

    # update when source changes
    configure_file(python-unit-tests/run.py
            ${CMAKE_CURRENT_BINARY_DIR}/python-unit-tests/run.py COPYONLY)

    add_test(
            NAME PythonTests
            COMMAND Python::Interpreter "${RR_PYTHON_TESTING_BUILD_PREFIX}"
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/lib/site-packages"
    )

    set_tests_properties(PythonTests PROPERTIES ENVIRONMENT "PYTHONPATH=${PYTHON_PACKAGE_SITE_DIR_BUILD_TREE}:$PYTHONPATH")

    # Creates a custom target which runs only the sbml tests suite
    add_custom_target(
            run-python-tests
            ctest ${ctest_args} --tests-regex PythonTests
    )

    add_dependencies(run-python-tests ${test_targets})

endif ()


# make test_targets list global to all tests
set(test_targets ${test_targets} PARENT_SCOPE)

