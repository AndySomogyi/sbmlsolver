#ifndef rrPluginsAPIExporterH
#define rrPluginsAPIExporterH
#include "rrOSSpecifics.h"

#if defined(_WIN32)
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
#if defined(_WIN32)
    #if defined(EXPORT_RR_PLUGIN)
        #define RR_PLUGIN_DECLSPEC __declspec(dllexport)
    #else
        #define RR_PLUGIN_DECLSPEC __declspec(dllimport)
    #endif
#else
    #define RR_PLUGIN_DECLSPEC
#endif

#if defined(_MSC_VER) || defined(__CODEGEARC__)
    #define rrpCallConv __stdcall
#else
    #define rrpCallConv
#endif

#endif

