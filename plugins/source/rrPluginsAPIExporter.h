#ifndef rrPluginsAPIExporterH
#define rrPluginsAPIExporterH

#if defined(_WIN32) || defined(__WIN32__)
    #if defined(STATIC_PLUGINS_API)
        #define PLUGINS_API_DECLSPEC
    #else
        #if defined(EXPORT_PLUGINS_API)
            #define PLUGINS_API_DECLSPEC __declspec(dllexport)
        #else
            #define PLUGINS_API_DECLSPEC __declspec(dllimport)
        #endif
    #endif
#else
    #define PLUGINS_API_DECLSPEC
#endif

//Helper macro for plugins
#if defined(_WIN32) || defined(__WIN32__)
    #if defined(EXPORT_RR_PLUGIN)
        #define RR_PLUGIN_DECLSPEC __declspec(dllexport)
    #else
        #define RR_PLUGIN_DECLSPEC __declspec(dllimport)
    #endif
#else
    #define RR_PLUGIN_DECLSPEC
#endif

#if defined(_MSC_VER) || defined(__CODEGEARC__)
    #define plugins_cc  __cdecl      //For the C functions that are exported from a plugin
    #define callback_cc __cdecl      //For plugin callback functions
#else
    #define plugins_cc
    #define callback_cc
#endif


#endif

