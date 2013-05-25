#ifndef rrExporterH
#define rrExporterH

#if defined(_WIN32) || defined(WIN32)
    #if defined(STATIC_RR) || defined(RR_STATIC)
        #define RR_DECLSPEC
    #else
        #if defined(EXPORT_RR)
            #define RR_DECLSPEC __declspec(dllexport)
        #else
            #define RR_DECLSPEC __declspec(dllimport)
        #endif
    #endif
#else
    #define RR_DECLSPEC
#endif

//#include "rrOSSpecifics.h"

#if defined(_MSC_VER) || defined(__CODEGEARC__)
	#define rrCallConv __stdcall
#else
	#define rrCallConv
#endif


#endif
