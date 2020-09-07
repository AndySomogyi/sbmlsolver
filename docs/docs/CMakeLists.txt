##################################################################
# Usage
# -----
# Build the libomexmeta-docs-sphinx.
#
# setup
# ------
# To setup, you first need to tell cmake which sphinx-build you are using.
# do this with -DSPHINX_EXECUTABLE="/path/to/sphinx-build"
# Be careful with conda environments, since its possible that
# the target will find the python interpreter that you do not want to use.
# You will need to:
#   pip install sphinx breathe sphinxcontrib-bibtex sphinx-tabs sphinx_rtd_theme
# you will also need to ensure doxygen is installed and findable.
#
#

############################################################
# Build the documentation examples
#

set(EXAMPLE_EXECUTABLES_TARGETS)

macro(add_doc_example BINARY_NAME)

    # binary for testing Editor's ability to extract information from sbml
    add_executable(${BINARY_NAME} ${ARGN})
    set(EXAMPLE_EXECUTABLES_TARGETS ${EXAMPLE_EXECUTABLES_TARGETS} "${BINARY_NAME}")

    target_compile_definitions(
            ${BINARY_NAME} PRIVATE OMEXMETA_STATIC_DEFINE REDLAND_STATIC_DEFINE
    )
    # add test so it can be run with $ctest or $make test
    add_test(
            NAME ${BINARY_NAME}
            COMMAND $<TARGET_FILE:${BINARY_NAME}>
    )

    set_tests_properties(${BINARY_NAME} PROPERTIES DEPENDS OmexMeta)
    add_dependencies(${BINARY_NAME} OmexMeta-static)

    # links
    target_link_libraries(${BINARY_NAME} PRIVATE
                          # Order matters!
                          $<TARGET_FILE:OmexMetaCAPI> # for C examples
                          $<TARGET_FILE:OmexMeta-static> # for c++ examples
                          $<TARGET_FILE:redland-combined-static>
                          ${LINK_LIBRARIES}
                          ${RUNTIME}# best here so no clash with dynamic
                          pthread
                          )

    target_include_directories(${BINARY_NAME} PRIVATE
                               ${GOOGLETEST_SOURCE}/googletest/include

                               ${INCLUDE_DIRECTORIES}
                               ${WRAPPER_SOURCE_DIR}
                               )


endmacro()




if (BUILD_DOCS_EXAMPLES)

    # I've automated this as much as possible. The idea is that
    # I write the examples and compile the programs. Then I capture the
    # output and save those to file as well. Then, we can easily include
    # both code and output into sphinx using the `.. literalinclude` directive.
    # This can be done using pure cmake, and I asked how here:
    #   https://stackoverflow.com/questions/62978989/running-multiple-executables-with-a-single-command-in-cmake-and-collecting-the-r
    # But instead I opted to write a python script that will run the examples.
    #
    # So, its cmakes job to build these examples. And its Pythons job to
    # run the examples and write output to a known location. Then its
    # sphinx's job to pickup both code and output and include them in
    # the documentation.

    add_doc_example(AddFromFileC source/example_of_docs_source_folder/add_from_file_c.cpp)
    add_doc_example(AddFromFileCpp source/example_of_docs_source_folder/add_from_file_cpp.cpp)
    add_doc_example(AddFromStringC source/example_of_docs_source_folder/add_from_string_c.cpp)
    add_doc_example(AddFromStringCpp source/example_of_docs_source_folder/add_from_string_cpp.cpp)
    add_doc_example(AddFromUriC source/example_of_docs_source_folder/add_from_uri_c.cpp)
    add_doc_example(AddFromUriCpp source/example_of_docs_source_folder/add_from_uri_cpp.cpp)
    add_doc_example(FromStringC source/example_of_docs_source_folder/from_string_c.cpp)
    add_doc_example(FromStringCpp source/example_of_docs_source_folder/from_string_cpp.cpp)
    add_doc_example(FromUriC source/example_of_docs_source_folder/from_uri_c.cpp)
    add_doc_example(FromUriCpp source/example_of_docs_source_folder/from_uri_cpp.cpp)
    add_doc_example(FromFileC source/example_of_docs_source_folder/from_file_c.cpp)
    add_doc_example(FromFileCpp source/example_of_docs_source_folder/from_file_cpp.cpp)
    add_doc_example(CreateSingleAnnotationC                       source/editing_rdf/create_single_annotation_c.cpp)
    add_doc_example(CreateSingleAnnotationCpp                     source/editing_rdf/create_single_annotation_cpp.cpp)
    add_doc_example(CreateSingleAnnotationNoCommitC               source/editing_rdf/create_single_annotation_no_commit_c.cpp)
    add_doc_example(CreateSingleAnnotationNoCommitCpp             source/editing_rdf/create_single_annotation_no_commit_cpp.cpp)
    add_doc_example(CreateSingleAnnotationPredicateFromUriC       source/editing_rdf/create_single_annotation_predicate_from_uri_c.cpp)
    add_doc_example(CreateSingleAnnotationPredicateFromUriCpp     source/editing_rdf/create_single_annotation_predicate_from_uri_cpp.cpp)
    add_doc_example(CreateSingleAnnotationResourceC               source/editing_rdf/create_single_annotation_resource_c.cpp)
    add_doc_example(CreateSingleAnnotationResourceCpp             source/editing_rdf/create_single_annotation_resource_cpp.cpp)
    add_doc_example(CreateSingleAnnotationWithMetaidsC            source/editing_rdf/create_single_annotation_with_metaids_c.cpp)
    add_doc_example(CreateSingleAnnotationWithMetaidsCpp          source/editing_rdf/create_single_annotation_with_metaids_cpp.cpp)
    add_doc_example(CreatePhysicalEntityC                         source/editing_rdf/create_physical_entity_c.cpp)
    add_doc_example(CreatePhysicalEntityCpp                       source/editing_rdf/create_physical_entity_cpp.cpp)
    add_doc_example(CreatePhysicalProcessC                        source/editing_rdf/create_physical_process_c.cpp)
    add_doc_example(CreatePhysicalProcessCpp                      source/editing_rdf/create_physical_process_cpp.cpp)
    add_doc_example(CreatePhysicalForceC                          source/editing_rdf/create_physical_force_c.cpp)
    add_doc_example(CreatePhysicalForceCpp                        source/editing_rdf/create_physical_force_cpp.cpp)
    add_doc_example(WritingToStringC                              source/writing_rdf/writing_to_string_c.cpp)
    add_doc_example(WritingToStringCpp                            source/writing_rdf/writing_to_string_cpp.cpp)
    add_doc_example(QueryingRdfGraphC                             source/querying_rdf/querying_rdf_c.cpp)
    add_doc_example(QueryingRdfGraphCpp                           source/querying_rdf/querying_rdf_cpp.cpp)

    add_custom_target(AllDocExamples
            DEPENDS ${EXAMPLE_EXECUTABLES_TARGETS}
            )

    add_dependencies(AllDocExamples OmexMeta OmexMeta-static OmexMetaCAPI)

    # We now create a target that runs the Python script which runs the
    # executables.
    set(DOC_EXAMPLE_RUNNER "${CMAKE_CURRENT_SOURCE_DIR}/build_doc_examples.py")
    if (NOT EXISTS ${DOC_EXAMPLE_RUNNER})
        message(FATAL_ERROR "No Python script found where \"build_doc_examples.pY\" should be")
    endif ()


    # we do not want to use CMAKE_BINARY_DIR so we just take parent of current binary dir
    get_filename_component(PARENT_CURRENT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR} DIRECTORY)
    # later we'll set the build docs target to depend on this
    add_custom_target(RunDocExamplesAndGenerateOutput
            COMMAND ${PYTHON_EXECUTABLE} ${DOC_EXAMPLE_RUNNER} --python-interpretor ${PYTHON_EXECUTABLE} --build-dir ${PARENT_CURRENT_BINARY_DIR} --pyomexmeta-source-dir ${PYOMEXMETA_DIR}
            DEPENDS AllDocExamples
            )
endif ()


############################################################
#   doxygen
#


# https://devblogs.microsoft.com/cppblog/clear-functional-c-documentation-with-sphinx-breathe-doxygen-cmake/
find_package(Doxygen REQUIRED)

set(DOXYGEN_INPUT_DIR1 "${CMAKE_SOURCE_DIR}/src/omexmeta")
set(DOXYGEN_INPUT_DIR2 "${CMAKE_SOURCE_DIR}/src/redland/RedlandWrapper/src")
set(DOXYGEN_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/docs/doxygen-output")
set(DOXYGEN_INDEX_FILE ${DOXYGEN_OUTPUT_DIR}/html/index.xml)
set(DOXYFILE_IN ${CMAKE_SOURCE_DIR}/Doxyfile.in)
set(DOXYFILE_OUT ${CMAKE_BINARY_DIR}/Doxyfile)

configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

# Doxygen won't create this for us
file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR})

# Only regenerate Doxygen when the Doxyfile or public headers change
add_custom_command(
        OUTPUT ${DOXYGEN_INDEX_FILE}
        DEPENDS ${SEMSIM_HEADERS}
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        MAIN_DEPENDENCY ${DOXYFILE_OUT} ${DOXYFILE_IN}
        COMMENT "Generating docs"
        VERBATIM
)

add_custom_target(libomexmeta-docs-doxygen ALL DEPENDS ${DOXYGEN_INDEX_FILE})

############################################################
#   Sphinx
#

set(SPHINX_EXECUTABLE "/home/ciaran/miniconda3/bin/sphinx-build" CACHE FILEPATH
        "absolute path to the sphinx-build executable. If you do not have sphinx-build
you will need to install it. In your conda environment do: $conda activate <env>;
pip install breathe sphinx sphinxcontrib-bibtex sphinx-tabs sphinx_rtd_theme; which sphinx-build. Then copy the value of which sphinx-build into
-DSPHINX_EXECUTABLE")

message(STATUS "SPHINX_EXECUTABLE ${SPHINX_EXECUTABLE}")
set(SPHINX_SOURCE ${CMAKE_SOURCE_DIR}/docs/source)
set(SPHINX_BUILD ${CMAKE_SOURCE_DIR}/docs/docs-build)
set(SPHINX_INDEX_FILE ${SPHINX_BUILD}/index.html)

# Only regenerate Sphinx when:
# - Doxygen has rerun
# - Our doc files have been updated
# - The Sphinx config has been updated
add_custom_command(
        OUTPUT ${SPHINX_INDEX_FILE}
        COMMAND
        # Tell Breathe where to find the Doxygen output
        ${SPHINX_EXECUTABLE} -b html -Dbreathe_projects.libomexmeta=${DOXYGEN_OUTPUT_DIR}/xml
        ${SPHINX_SOURCE} ${SPHINX_BUILD}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        DEPENDS
        # Other docs files you want to track should go here (or in some variable)
        ${SPHINX_SOURCE}/index.rst
        ${DOXYGEN_INDEX_FILE}
        MAIN_DEPENDENCY ${SPHINX_SOURCE}/conf.py
        COMMENT "Generating documentation with Sphinx")


add_custom_target(libomexmeta-docs-sphinx DEPENDS ${SPHINX_INDEX_FILE})

# when were building the docs examples, we add the dependency so the
# examples get built when we run build the libomexmeta-dpcs-sphinx target
if (BUILD_DOCS_EXAMPLES)
    add_dependencies(libomexmeta-docs-sphinx RunDocExamplesAndGenerateOutput)
endif ()

# Add an install target to install the docs
include(GNUInstallDirs)
install(DIRECTORY ${SPHINX_BUILD}
        DESTINATION ${CMAKE_INSTALL_DOCDIR})



