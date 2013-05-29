/** @file rrc_exporter.h */
#ifndef rrc_exporterH
#define rrc_exporterH

//Export/Import API functions
#if defined(_WIN32) || defined(WIN32)
    #if defined(STATIC_RRC)
        #define C_DECL_SPEC
    #else
        #if defined(EXPORT_RRC)
            #define C_DECL_SPEC __declspec(dllexport)
        #else
            #define C_DECL_SPEC __declspec(dllimport)
        #endif
    #endif
#else
    #define C_DECL_SPEC
#endif


#if defined(_MSC_VER) || defined(__CODEGEARC__)
	#define rrcCallConv __stdcall
#else
	#define rrcCallConv
#endif

#if !defined(__cplusplus)
    #if !defined(_MSC_VER)
    	#include "stdbool.h"
    #else	//VS don't have stdbool.h
    	typedef int bool;
    	#define false 0
    	#define true 1
    #endif
#endif

#endif
