#we have to do this to get the generated header
include_directories(${RR_GENERATED_HEADER_PATH})

# ref: https://github.com/shenshei/cmake-swig-java-example/blob/master/CMakeLists.txt

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${RR_SOURCE_ROOT}
    ${THIRD_PARTY_INSTALL_FOLDER}/include
    ${THIRD_PARTY_INSTALL_FOLDER}/include/rr-libstruct
    ${THIRD_PARTY_INSTALL_FOLDER}/include/sbml
    ${THIRD_PARTY_INSTALL_FOLDER}/include/cvode
    )

set_source_files_properties(jroadrunner.i PROPERTIES CPLUSPLUS ON)

swig_add_module(jroadrunner java jroadrunner.i)

if(WIN32)
    message(FATAL_ERROR "Java build on Windows not supported")
    # changes dll name to pyd sop that it is compatible with new Python standard
#     set_property(TARGET jroadrunner
#         PROPERTY  COMPILE_DEFINITIONS
#         EXPORT_RRC
#         LIBSBML_STATIC
#         LIBLAX_STATIC
#         STATIC_PUGI
#         POCO_STATIC
#         STATIC_LIBSTRUCT
#         STATIC_RR
#         )
#
#     swig_link_libraries ( jroadrunner
#         jroadrunner-static
#         ${Java_LIBRARIES}
#         )
else()
  #    set(CMAKE_SHARED_LINKER_FLAGS "-Wl, -z,defs --no-undefined")
  #    set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined")

  if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
      # Linux specific code
      #SET_TARGET_PROPERTIES(_jroadrunner PROPERTIES LINK_FLAGS "-Wl,--no-undefined")
  else()
#         SET_TARGET_PROPERTIES(_jroadrunner PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
  endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

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
  add_jar(roadrunner_jar NativeLib.java VERSION 1.3.0 ENTRY_POINT roadrunner)
  install_jar(roadrunner_jar ${LIB_INSTALL_DIR}/java)
  install_jni_symlink(roadrunner_jar .)

  set(JAVA_BUNDLE "${CMAKE_CURRENT_SOURCE_DIR}/java_bundle.sh")

#   set(JAVA_BUNDLE_SO_FILES "${CMAKE_CURRENT_SOURCE_DIR}/libexample_swig.so")

#   set(MANIFEST_FILE "${CMAKE_CURRENT_SOURCE_DIR}/MANIFEST")

  add_custom_command(TARGET example_swig
    POST_BUILD
    DEPEND example_swig_jar
    COMMAND ${JAVA_BUNDLE} "roadrunner.jar" "${Java_JAVA_EXECUTABLE}" "NativeLib" "${CMAKE_STRIP}"
    COMMAND ${JAVA_ARCHIVE} -uvf "example_swig_jar.jar" "NATIVE"
    COMMAND ${CMAKE_COMMAND} -E echo "Class-Path: ." > ${MANIFEST_FILE}
    COMMAND ${JAVA_ARCHIVE} -uvmf "MANIFEST" "example_swig_jar.jar"
    COMMAND ${CMAKE_COMMAND} -E remove ${MANIFEST_FILE}
    )

  #        lapack
  #        blas
  #        f2c
  #        "-Wl,--whole-archive"
  #        rr-libstruct-static
  #        "-Wl,--no-whole-archive"
  #        lmfit-static
  #        sbml-static
  #        jroadrunner-static


endif()
