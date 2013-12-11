cmake_minimum_required(VERSION 2.8)
PROJECT(tests)

set(tests
    README.txt
    NOM_Test.dat
    LibStructTest.dat
    Test_1.dat
    Test_1.xml
    Test_1.txt
    ModelSourceTest.h
    ModelSourceTest.c
    feedback.xml
)


install(
    FILES ${tests}
    DESTINATION testing 	COMPONENT testing
)

