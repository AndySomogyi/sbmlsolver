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
#define rrCallConv __stdcall
#else
#define rrCallConv
#endif

#ifndef DEPRECATED
#ifdef __GNUC__
#define DEPRECATED(func) func __attribute__ ((deprecated))
#elif defined(_MSC_VER)
#define DEPRECATED(func) __declspec(deprecated) func
#else
#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#define DEPRECATED(func) func
#endif
#endif

#endif
