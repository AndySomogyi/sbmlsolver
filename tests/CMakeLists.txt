cmake_minimum_required(VERSION 2.8)

PROJECT(tests)
set(tests
ss_threSpecies.dat
ss_threSpecies.xml
TestModel_1.dat
TestModel_1.xml
)

install(    
FILES ${tests}
DESTINATION tests 	
COMPONENT testing
)
