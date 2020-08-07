# Check for existance of a directory `directory` referred to by package_name
macro(CheckDirectoryExists package_name directory)
    if (NOT EXISTS "${directory}")
        message(FATAL_ERROR "Cannot find ${package_name} source directory.\
Was expecting to find it at \"${directory}\"")
    endif ()
endmacro()