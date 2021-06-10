#add_test_executable(cvsRoberts_FSA_dns cvode-example cvsRoberts_FSA_dns.c)

add_executable(cvsRoberts_FSA_dns cvsRoberts_FSA_dns.c)
set_target_properties(cvsRoberts_FSA_dns PROPERTIES LINKER_LANGUAGE C)

target_include_directories(cvsRoberts_FSA_dns PRIVATE
        "$<BUILD_INTERFACE:${DEPENDENCY_INCLUDE_DIR}>"
        )
target_link_libraries(cvsRoberts_FSA_dns PRIVATE
        SUNDIALS::generic_static
        SUNDIALS::nvecserial_static
        SUNDIALS::nvecmanyvector_static
        SUNDIALS::sunmatrixband_static
        SUNDIALS::sunmatrixdense_static
        SUNDIALS::sunmatrixsparse_static
        SUNDIALS::sunlinsolband_static
        SUNDIALS::sunlinsoldense_static
        SUNDIALS::sunlinsolpcg_static
        SUNDIALS::sunlinsolspbcgs_static
        SUNDIALS::sunlinsolspfgmr_static
        SUNDIALS::sunlinsolspgmr_static
        SUNDIALS::sunlinsolsptfqmr_static
        SUNDIALS::sunnonlinsolnewton_static
        SUNDIALS::sunnonlinsolfixedpoint_static
#        SUNDIALS::cvode_static
        SUNDIALS::cvodes_static
        SUNDIALS::kinsol_static
        )



add_test_executable(CVODESUnitTests test_targets CVODESUnitTests.cpp)
add_test_executable(CVODESIntegrationTests test_targets CVODESIntegrationTests.cpp)













