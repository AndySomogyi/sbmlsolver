CMAKE_MINIMUM_REQUIRED(VERSION 2.6.3 FATAL_ERROR)
PROJECT(RR_CXX_API_TESTS)

set(target rr-sbml-benchmark)

add_executable(${target} main.cpp)

target_link_libraries(${target} PRIVATE
        $<TARGET_FILE:roadrunner-static>
        "${LINK_LIBRARIES}"
        "${UNIT_TEST_STATIC_LIBRARY}"
        )
target_include_directories(${target} PRIVATE "${INCLUDE_DIRECTORIES}")
add_dependencies(${target} unittest roadrunner-static)

install(TARGETS ${target}
        DESTINATION bin
        COMPONENT testing
        )


