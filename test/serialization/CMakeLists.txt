include(UseSWIG)

if (BUILD_PYTHON AND BUILD_TESTS)

    #####################################################################
    #   Minimal swig bindings to a minimal C++ object.
    #   This is proof of principle of how to serialization a PySWIGObject
    #   and does not involve roadrunner itself.
    #

#    add_rr_test_swig_library()

    #todo write a cmake function for building swig targets
    set_source_files_properties(picklable_swig.i CPPObjectForPickling.cpp CPPObjectForPickling.h
            PROPERTIES
            CPLUSPLUS ON
            #            SWIG_FLAGS
            )
    add_rr_test_swig_library(
            picklable_swig
            picklable_swig.i
            CPPObjectForPickling.cpp
            CPPObjectForPickling.h
    )

    file(COPY pickle_tests.py DESTINATION "${RR_PYTHON_TESTING_BUILD_PREFIX}")


endif ()








