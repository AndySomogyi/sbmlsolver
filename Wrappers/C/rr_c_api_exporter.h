/** @file rr_c_api_exporter.h */
#ifndef rr_c_api_exporterH
#define rr_c_api_exporterH

//Export/Import API functions
#if defined(_WIN32) || defined(WIN32)
    #if defined(STATIC_RR_C_API) 
        #define C_DECL_SPEC
    #else
        #if defined(EXPORT_RR_C_API)
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

#endif
