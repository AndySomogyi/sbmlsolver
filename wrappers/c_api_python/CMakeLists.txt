
if (BUILD_TEST_TOOLS)
    add_subdirectory(Testing)

    if (INSTALL_EXAMPLES)
        add_subdirectory(Examples)
    endif ()

    #=============== install =====================
    install(
            FILES
            rrPython.py README.txt
            DESTINATION c_api_python
            COMPONENT testing
    )
endif (BUILD_TEST_TOOLS)
