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

#endif
