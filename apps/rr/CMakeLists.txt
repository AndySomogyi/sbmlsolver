cmake_minimum_required(VERSION 2.8)
set(target rr)

add_executable(${target}
        main.cpp
        Args.cpp Args.h
        )

target_compile_definitions(${target} PRIVATE STATIC_RR)
target_include_directories(${target} PRIVATE ${INCLUDE_DIRECTORIES})
target_link_libraries(${target} PRIVATE roadrunner-static ${LINK_LIBRARIES})

#if (WIN32)
#    add_definitions(
#            -DWIN32
#    )
#
#    target_link_libraries(${target}
#            #roadrunner-static
#            roadrunner
#            )
#endif ()
#
##Not sure why linux linker need all libraries at linktime?? All symbols 'should' be in roadrunner-static
#if (UNIX)
#    target_link_libraries(${target}
#            roadrunner-static
#            nleq1-static.a
#            nleq2-static.a
#            rr-libstruct-static
#            lapack
#            blas
#            f2c
#            ${LIBSBML_STATIC_LIBRARY}
#            xml2
#            sundials_nvecserial.a
#            sundials_cvode.a
#            pthread
#            dl
#            )
#endif ()


install(TARGETS ${target}
        DESTINATION bin
        COMPONENT apps
        )
