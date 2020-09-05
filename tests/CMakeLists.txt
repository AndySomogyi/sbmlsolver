set(target CanILinkRoadrunnerStatic)
add_executable(${target}
        main.cpp
        SBMLFactory.h SBMLFactory.cpp

        )

target_include_directories(${target} PRIVATE
        ${GOOGLETEST_INCLUDE_DIR}
        )

target_link_libraries(${target} PRIVATE roadrunner-static)
add_dependencies(${target} roadrunner-static)

#add_dependencies(${target} roadrunner-static
#        blas
#        f2c
#        lapack
#        libsbml-static
#        libiconv-static
#        libLZMA-static
#        libxml2-static
#        zlibstatic
#        nleq1-static
#        nleq2-static
#        sundials_cvode_static
#        sundials_cvodes_static
#        sundials_ida_static
#        sundials_idas_static
#        sundials_kinsol_static
#        sundials_nvecserial_static
#        PocoFoundation
#        PocoNet
#        PocoUtil
#        PocoXML
#        PocoZip
#        PocoJSON
#        rr-libstruct-static
#        )
