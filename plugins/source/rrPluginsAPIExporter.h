#ifndef rrPluginsAPIExporterH
#define rrPluginsAPIExporterH
#include "rrOSSpecifics.h"

#if defined(_WIN32)
    #if defined(EXPORT_PLUGINS_API)
        #define PLUGINS_API_DECLSPEC __declspec(dllexport)
    #else
        #define PLUGINS_API_DECLSPEC __declspec(dllimport)
    #endif
#else
    #define PLUGINS_API_DECLSPEC
#endif

#if defined(_WIN32)
    #if defined(EXPORT_RR_PLUGIN)
        #define RR_PLUGIN_DECLSPEC __declspec(dllexport)
    #else
        #define RR_PLUGIN_DECLSPEC __declspec(dllimport)
    #endif
#else
    #define RR_PLUGIN_DECLSPEC
#endif

#endif


