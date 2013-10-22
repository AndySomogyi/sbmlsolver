#if defined(CG_UI)
    #if defined(STATIC_PLUGIN)
        #pragma comment(lib, "roadrunner-static.lib")
    #else
        #pragma comment(lib, "roadrunner.lib")
        #pragma comment(lib, "rrp_api.lib")
    #endif

    #pragma comment(lib, "poco_foundation-static.lib")
#endif


#if defined(_WIN32)
#pragma comment(lib, "IPHLPAPI.lib") //Becuase of poco needing this
#endif
