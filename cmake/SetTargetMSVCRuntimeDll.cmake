macro(SetTargetMSVCRuntimeDLL target)
    # set msvc runtime
    if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        set_property(TARGET ${target} PROPERTY
                MSVC_RUNTIME_LIBRARY "MultiThreadedDebugDLL")
    elseif (${CMAKE_BUILD_TYPE} STREQUAL "Release")
        set_property(TARGET ${target} PROPERTY
                MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
    else ()
        message(FATAL_ERROR "Only Debug and Release configurations are supported")
    endif ()
endmacro()


