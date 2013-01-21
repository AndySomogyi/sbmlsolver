cmake_minimum_required(VERSION 2.8)
PROJECT(tests)

if(BUILD_TESTS)

set(tests
Test_1.dat
Test_1.xml
Test_1.txt
)


install(FILES ${tests}
DESTINATION testing 	
COMPONENT testing
)

endif(BUILD_TESTS)
