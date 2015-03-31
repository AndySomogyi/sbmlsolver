include(${CMAKE_CURRENT_SOURCE_DIR}/FindSWIG.cmake) # this is slightly modified FindSWIG.cmake file -  had to tune it to work with OSX

# https://github.com/shenshei/cmake-swig-java-example

find_package(SWIG REQUIRED)
find_package(Java 1.7 REQUIRED COMPONENTS Runtime Development)
find_package(JNI REQUIRED)
include(UseSWIG)
include(UseJava)

message("Java includes: ${Java_INCLUDE_DIRS}")
message("Java libs: ${Java_LIBRARIES}")
message("JNI includes: ${JNI_INCLUDE_DIRS}")
message("JNI libs: ${JNI_LIBRARIES}")

include_directories(${Java_INCLUDE_DIRS})
include_directories(${JNI_INCLUDE_DIRS})
# INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
# INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/include)
# INCLUDE_DIRECTORIES(${RR_SOURCE_PATH}/third_party/rr-libstruct)

if(SWIG_FOUND)
  #we have to do this to get the generated header
  include_directories(${RR_GENERATED_HEADER_PATH})

  # ref: https://github.com/shenshei/cmake-swig-java-example/blob/master/CMakeLists.txt

  include_directories(
      ${CMAKE_CURRENT_SOURCE_DIR}
      ${CMAKE_CURRENT_SOURCE_DIR}/include
      ${RR_SOURCE_ROOT}
      ${THIRD_PARTY_INSTALL_FOLDER}/include
      ${THIRD_PARTY_INSTALL_FOLDER}/include/rr-libstruct
      ${THIRD_PARTY_INSTALL_FOLDER}/include/sbml
      ${THIRD_PARTY_INSTALL_FOLDER}/include/cvode
      )

  set_source_files_properties(roadrunner/jroadrunner.i PROPERTIES CPLUSPLUS ON)

  set(CMAKE_SWIG_FLAGS -package roadrunner)
  set(CMAKE_SWIG_OUTDIR "roadrunner")

  swig_add_module(jroadrunner java roadrunner/jroadrunner.i)

  set(ROADRUNNER_JAVA_GENERATED
    ${CMAKE_CURRENT_SOURCE_DIR}/roadrunner/NativeLib.java
    ${CMAKE_CURRENT_BINARY_DIR}/roadrunner/BasicDictionary.java
    ${CMAKE_CURRENT_BINARY_DIR}/roadrunner/roadrunnerJNI.java
#     ${CMAKE_CURRENT_BINARY_DIR}/roadrunner/RoadrunnerJNI.java
    ${CMAKE_CURRENT_BINARY_DIR}/roadrunner/Compiler.java
    ${CMAKE_CURRENT_BINARY_DIR}/roadrunner/Config.java
    ${CMAKE_CURRENT_BINARY_DIR}/roadrunner/Dictionary.java
    # ${CMAKE_CURRENT_BINARY_DIR}/roadrunner/DictionaryVector.java
    ${CMAKE_CURRENT_BINARY_DIR}/roadrunner/EventListener.java
    ${CMAKE_CURRENT_BINARY_DIR}/roadrunner/ExecutableModelFactory.java
    ${CMAKE_CURRENT_BINARY_DIR}/roadrunner/ExecutableModel.java
    ${CMAKE_CURRENT_BINARY_DIR}/roadrunner/IntegratorException.java
    ${CMAKE_CURRENT_BINARY_DIR}/roadrunner/IntegratorFactory.java
    ${CMAKE_CURRENT_BINARY_DIR}/roadrunner/Integrator.java
    ${CMAKE_CURRENT_BINARY_DIR}/roadrunner/IntVector.java
    ${CMAKE_CURRENT_BINARY_DIR}/roadrunner/LoadSBMLOptions.java
    ${CMAKE_CURRENT_BINARY_DIR}/roadrunner/Logger.java
    ${CMAKE_CURRENT_BINARY_DIR}/roadrunner/PyConservedMoietyConverter.java
    ${CMAKE_CURRENT_BINARY_DIR}/roadrunner/roadrunner.java
#     ${CMAKE_CURRENT_BINARY_DIR}/roadrunner/Roadrunner.java
    ${CMAKE_CURRENT_BINARY_DIR}/roadrunner/RoadRunner.java
    ${CMAKE_CURRENT_BINARY_DIR}/roadrunner/RoadRunnerOptions.java
    ${CMAKE_CURRENT_BINARY_DIR}/roadrunner/SBMLReader.java
    ${CMAKE_CURRENT_BINARY_DIR}/roadrunner/SelectionRecord.java
    ${CMAKE_CURRENT_BINARY_DIR}/roadrunner/SimulateOptions.java
    ${CMAKE_CURRENT_BINARY_DIR}/roadrunner/StringVector.java
    ${CMAKE_CURRENT_BINARY_DIR}/roadrunner/SWIGTYPE_p_cxx11_ns__shared_ptrT_rr__EventListener_t.java
    ${CMAKE_CURRENT_BINARY_DIR}/roadrunner/SWIGTYPE_p_cxx11_ns__shared_ptrT_rr__IntegratorListener_t.java
    ${CMAKE_CURRENT_BINARY_DIR}/roadrunner/SWIGTYPE_p_double.java
    ${CMAKE_CURRENT_BINARY_DIR}/roadrunner/SWIGTYPE_p_Integrator__IntegratorId.java
    ${CMAKE_CURRENT_BINARY_DIR}/roadrunner/SWIGTYPE_p_int.java
    ${CMAKE_CURRENT_BINARY_DIR}/roadrunner/SWIGTYPE_p_ls__DoubleMatrix.java
  #   ${CMAKE_CURRENT_BINARY_DIR}/roadrunner/SWIGTYPE_p_PyObject.java
  #   ${CMAKE_CURRENT_BINARY_DIR}/roadrunner/SWIGTYPE_p_rr__PyEventListener.java
  #   ${CMAKE_CURRENT_BINARY_DIR}/roadrunner/SWIGTYPE_p_rr__PyIntegratorListenerPtr.java
    ${CMAKE_CURRENT_BINARY_DIR}/roadrunner/SWIGTYPE_p_std__vectorT_double_t.java
    ${CMAKE_CURRENT_BINARY_DIR}/roadrunner/SWIGTYPE_p_std__vectorT_ls__Complex_t.java
    ${CMAKE_CURRENT_BINARY_DIR}/roadrunner/SWIGTYPE_p_std__vectorT_rr__Dictionary_const_p_t.java
    ${CMAKE_CURRENT_BINARY_DIR}/roadrunner/SWIGTYPE_p_string.java
    ${CMAKE_CURRENT_BINARY_DIR}/roadrunner/SWIGTYPE_p_unsigned_char.java
    ${CMAKE_CURRENT_BINARY_DIR}/roadrunner/SWIGTYPE_p_Variant.java
    ${CMAKE_CURRENT_BINARY_DIR}/roadrunner/ValidateSBML.java
    ${CMAKE_CURRENT_BINARY_DIR}/roadrunner/VersionStrOptions.java
    )

  set_property(TARGET jroadrunner
      PROPERTY  COMPILE_DEFINITIONS
      LIBSBML_STATIC
      LIBLAX_STATIC
      STATIC_PUGI
      STATIC_LIBSTRUCT
      )

  swig_link_libraries ( jroadrunner
      roadrunner-static
      rr-libstruct-static
      nleq-static
      sbml-static
      lapack
      blas
      f2c
      ${Java_LIBRARIES}
      )

  set(CMAKE_JNI_TARGET TRUE)
#   add_jar(roadrunner_jar ${CMAKE_CURRENT_BINARY_DIR}/ValidateSBML.java VERSION 1.3.0 ENTRY_POINT roadrunner)
#   add_jar(roadrunner_jar ${ROADRUNNER_JAVA_GENERATED} ENTRY_POINT roadrunner.NativeLib)
  add_jar(roadrunner_jar ${ROADRUNNER_JAVA_GENERATED})

  # need to make this depend on the generated files as well
  add_dependencies(roadrunner_jar jroadrunner)
#   install_jar(roadrunner_jar ${LIB_INSTALL_DIR}/java)
#   install_jni_symlink(roadrunner_jar .)

  set(JAVA_BUNDLE "${CMAKE_CURRENT_SOURCE_DIR}/roadrunner/java_bundle.sh")

  set(JAVA_BUNDLE_SO_FILES "${CMAKE_BINARY_DIR}/lib/libjroadrunner.so")

  set(MANIFEST_FILE "MANIFEST")

  add_custom_command(TARGET jjroadrunner
    POST_BUILD
    DEPEND roadrunner_jar
    COMMAND ${JAVA_BUNDLE} "roadrunner_jar.jar" "${Java_JAVA_EXECUTABLE}" "roadrunner/NativeLib" "${CMAKE_STRIP}" -so ${JAVA_BUNDLE_SO_FILES}
    COMMAND ${JAVA_ARCHIVE} -uvf "roadrunner_jar.jar" "NATIVE"
    COMMAND ${CMAKE_COMMAND} -E echo "Class-Path: ." >${MANIFEST_FILE}
    COMMAND ${JAVA_ARCHIVE} -uvmf "MANIFEST" "roadrunner_jar.jar"
    COMMAND ${CMAKE_COMMAND} -E remove ${MANIFEST_FILE}
    )

  add_subdirectory(roadrunner)
else(SWIG_FOUND)
    message("could not find SWIG in your system")
endif(SWIG_FOUND)
