#ifndef rrp_exporterH
#define rrp_exporterH

//Export/Import API functions
#if defined(_WIN32)
    #if defined(STATIC_RRP)
        #define RRP_DECLSPEC
    #else
        #if defined(EXPORT_RRP)
            #define RRP_DECLSPEC __declspec(dllexport)
        #else
            #define RRP_DECLSPEC __declspec(dllimport)
        #endif
    #endif
#else
    #define RRP_DECLSPEC
#endif

//C function calling convention
#if defined(_MSC_VER) || defined(__CODEGEARC__)
    #define rrp_cc __stdcall
    #define callback_cc __cdecl      //For plugin callback functions
#else
    #define rrp_cc
    #define callback_cc
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
