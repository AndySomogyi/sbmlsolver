#add_test_executable(cvsRoberts_FSA_dns cvode-example cvsRoberts_FSA_dns.c)

add_test_executable(test_sundials_FFSUnitTests test_targets FFSUnitTests.cpp)
add_test_executable(test_sundials_FFSIntegrationTests test_targets FFSIntegrationTests.cpp ../CVODEIntegratorTests/CvodeIntegrationTest.h)



