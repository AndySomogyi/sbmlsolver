#ifndef rrp_exporterH
#define rrp_exporterH

//Export/Import API functions
#if defined(_WIN32) || defined(__WIN32__)
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

#endif

