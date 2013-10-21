#ifndef rrp_exporterH
#define rrp_exporterH

//Export/Import API functions
#if defined(_WIN32) || defined(WIN32)
    #if defined(STATIC_RRE)
        #define RRP_DECLSPEC
    #else
        #if defined(EXPORT_RRE)
            #define RRP_DECLSPEC __declspec(dllexport)
        #else
            #define RRP_DECLSPEC __declspec(dllimport)
        #endif
    #endif
#else
    #define RRP_DECLSPEC
#endif


#if defined(_MSC_VER) || defined(__CODEGEARC__)
    #define rrpCallConv __stdcall
#else
    #define rrpCallConv
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
