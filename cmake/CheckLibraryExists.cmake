macro(CheckLibraryExists library_name library_path)

    if (NOT EXISTS ${library_path})
        message(FATAL_ERROR "Library \"${library_name}\" was expected at \"${library_path}\" but was not found")
    endif ()
endmacro()