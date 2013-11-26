#ifndef rrPluginsAPISettingsH
#define rrPluginsAPISettingsH

#if defined(_MSC_VER) || defined(__CODEGEARC__)
    #define plugins_cc  __cdecl      //For the C functions that are exported from a plugin
    #define callback_cc __cdecl      //For plugin callback functions
#else
    #define plugins_cc
    #define callback_cc
#endif


#endif

