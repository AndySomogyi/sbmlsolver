set(target CanILinkRoadrunnerStatic)
add_executable(${target} main.cpp)

target_include_directories(${target} PRIVATE
        ${INCLUDE_DIRECTORIES}
        D:/rrbuild/roadrunner/third_party/googletest/googletest/include
        )

target_link_libraries(${target} PRIVATE
        roadrunner-static
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
        )

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

if (WIN32)
    # Indicate that we want to link to static roadrunner
    target_compile_options(${target} PRIVATE -DSTATIC_RR -DLIBXML_STATIC
            -DLIBSBML_STATIC
            -DLIBLAX_STATIC
            -DSTATIC_LIBSTRUCT
            -DSTATIC_PUGI
            -DSTATIC_NLEQ1
            -DSTATIC_NLEQ2
            -DPOCO_STATIC)


endif ()