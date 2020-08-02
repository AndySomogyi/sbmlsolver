macro(SetTargetMSVCRuntimeStatic target)
    # set msvc runtime
    if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        set_property(TARGET ${target} PROPERTY
                MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")
    elseif (${CMAKE_BUILD_TYPE} STREQUAL "Release")
        set_property(TARGET ${target} PROPERTY
                MSVC_RUNTIME_LIBRARY "MultiThreaded")
    else ()
        message(FATAL_ERROR "Only Debug and Release configurations are supported")
    endif ()
endmacro()


