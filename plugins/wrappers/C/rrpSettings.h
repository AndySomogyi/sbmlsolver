#ifndef rrpSettingsH
#define rrpSettingsH

//C function calling convention
#if defined(_MSC_VER) || defined(__CODEGEARC__)
    #define rrp_cc      __cdecl
    #define event_cc __cdecl      //For plugin event functions
#else
    #define rrp_cc
    #define event_cc
#endif

#if !defined(__cplusplus)
    #if !defined(_MSC_VER)
        #include "stdbool.h"
    #else   //VS don't have stdbool.h
        /*!
         \brief Typedef for boolean type in C
        */
        typedef int bool;
        #define false 0
        #define true 1
    #endif
#endif

#endif