
#if defined(CG_UI)
    #if defined(STATIC_PLUGIN)
        #pragma comment(lib, "roadrunner-static.lib")
        #pragma comment(lib, "rrc_api-static.lib")
        #pragma comment(lib, "rrplugins_api-static.lib")
        #pragma comment(lib, "rrplugins_c_api-static.lib")
        #pragma comment(lib, "sundials_cvode.lib")
        #pragma comment(lib, "sundials_nvecserial.lib")
        #pragma comment(lib, "rr-libstruct-static.lib")
        #pragma comment(lib, "nleq-static.lib")
        #pragma comment(lib, "blas.lib")
        #pragma comment(lib, "lapack.lib")
        #pragma comment(lib, "libf2c.lib")
        #pragma comment(lib, "lmfit-static.lib")
        #pragma comment(lib, "libxml2_xe.lib")

    #else
        #pragma comment(lib, "roadrunner.lib")
        #pragma comment(lib, "rrplugins_api.lib")
        #pragma comment(lib, "rrc_api.lib")
        #pragma comment(lib, "rrplugins_c_api.lib")
        #pragma comment(lib, "lmfit-static.lib")
    #endif

    #pragma comment(lib, "poco_foundation-static.lib")
#endif


