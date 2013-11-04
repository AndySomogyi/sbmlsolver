
#if defined(CG_UI)
    #if defined(STATIC_PLUGIN)
        #pragma comment(lib, "roadrunner-static.lib")
        #pragma comment(lib, "libsbml-static.lib")
        #pragma comment(lib, "libxml2_xe.lib")
        #pragma comment(lib, "rrp_api-static.lib")
    #else
        #pragma comment(lib, "roadrunner.lib")
        #pragma comment(lib, "rrplugins_api.lib")
        #pragma comment(lib, "rrc_api.lib")
        #pragma comment(lib, "rrplugins_c_api.lib")
        #pragma comment(lib, "lmfit-static.lib)
    #endif

    #pragma comment(lib, "poco_foundation-static.lib")
#endif


