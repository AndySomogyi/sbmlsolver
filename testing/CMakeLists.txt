cmake_minimum_required(VERSION 2.8)
PROJECT(tests)

if(BUILD_TESTS)

set(tests
ss_threeSpecies.dat
ss_threeSpecies.xml
TestModel_1.dat
TestModel_1.xml
roadRunnerTest_1.xml
results_roadRunnerTest_1.txt
)


install(FILES ${tests}
DESTINATION testing 	
COMPONENT testing
)

endif(BUILD_TESTS)
