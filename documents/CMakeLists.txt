cmake_minimum_required(VERSION 2.8)

if(${INSTALL_CXX_DOCS})
add_subdirectory(cpp_api)
install(FILES
        cpp-docs.html.lnk
        DESTINATION .
        COMPONENT cpp_docs)
endif()

if(${INSTALL_C_DOCS})
add_subdirectory(c_api)
install(FILES
        c-docs.html.lnk
        DESTINATION .
        COMPONENT c_docs)
endif()

if(${INSTALL_PYTHON_DOCS})
add_subdirectory(python_api)
install(FILES
        python-docs.html.lnk
        DESTINATION .
        COMPONENT python_docs)
endif()

