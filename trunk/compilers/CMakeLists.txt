cmake_minimum_required(VERSION 2.8)
project(compilers)
set(COMPILERS_FOLDER 			${CMAKE_CURRENT_SOURCE_DIR})


#=== TCC ==================================================
FILE (GLOB tcc ${COMPILERS_FOLDER}/tcc/*.*)
install (FILES 	${tcc} DESTINATION compilers/tcc COMPONENT rr_core)

FILE (GLOB tcc ${COMPILERS_FOLDER}/tcc/lib/*.*)
install (FILES 	${tcc} DESTINATION compilers/tcc/lib COMPONENT rr_core)

FILE (GLOB tcc ${COMPILERS_FOLDER}/tcc/include/*.*)
install (FILES 	${tcc} DESTINATION compilers/tcc/include COMPONENT rr_core)

FILE (GLOB tcc ${COMPILERS_FOLDER}/tcc/include/sys/*.*)
install (FILES 	${tcc} DESTINATION compilers/tcc/include/sys COMPONENT rr_core)

FILE (GLOB tcc ${COMPILERS_FOLDER}/tcc/include/winapi/*.*)
install (FILES 	${tcc} DESTINATION compilers/tcc/include/winapi COMPONENT rr_core)

