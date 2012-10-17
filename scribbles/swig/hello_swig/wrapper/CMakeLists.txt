FIND_PACKAGE( SWIG )
IF( SWIG_FOUND )
    FIND_PACKAGE( PythonLibs )
    IF( PYTHONLIBS_FOUND )

    # set up SWIG
    INCLUDE_DIRECTORIES( ${PYTHON_INCLUDE_PATH} )
    INCLUDE( ${SWIG_USE_FILE} )
    SET( CMAKE_SWIG_FLAGS "" )
    SET( CMAKE_SWIG_OUTDIR ${HELLO_RELEASE_DIR} )

    # build wrapper
    INCLUDE_DIRECTORIES( ${HELLO_SOURCE_DIR} )
    SWIG_ADD_MODULE( hello_swig
        python
        hello.i
        ${HELLO_SOURCE_DIR}/hello.h
        ${HELLO_SOURCE_DIR}/hello.c
    )
    SWIG_LINK_LIBRARIES( hello_swig ${PYTHON_LIBRARIES} )

    # copy the manually written object-oriented wrapper
    # to the release dir so everything's in one place
    FILE(
        COPY hello_oo.py
        DESTINATION ${HELLO_RELEASE_DIR}
    )

    ELSE( PYTHONLIBS_FOUND )
        MESSAGE( "Python not found; unable to generate Python wrapper" )
    ENDIF( PYTHONLIBS_FOUND )
ELSE( SWIG_FOUND )
    MESSAGE( "SWIG not found; unable to generate Python wrapper" )
ENDIF( SWIG_FOUND )
