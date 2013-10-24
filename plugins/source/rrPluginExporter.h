#ifndef rrPluginExporterH
#define rrPluginExporterH
#include "rrOSSpecifics.h"

#if defined(_WIN32)
    #if defined(EXPORT_PLUGIN)
        #define PLUGIN_DECLSPEC __declspec(dllexport)
    #else
        #define PLUGIN_DECLSPEC __declspec(dllimport)
    #endif
#else
    #define PLUGIN_DECLSPEC
#endif

#endif
