#if defined(CG_UI) || defined(_MSC_VER)
    #if defined(STATIC_PLUGIN)
        #pragma comment(lib, "roadrunner-static.lib")
        #pragma comment(lib, "rrplugins_api-static.lib")
        #pragma comment(lib, "libsbml-static")
		#if defined(CG_UI)
			#pragma comment(lib, "libxml2_xe.lib")
		#else
			#pragma comment(lib, "libxml2.lib")
		#endif
    #else
        #pragma comment(lib, "roadrunner.lib")
        #pragma comment(lib, "rrplugins_api.lib")
    #endif

    #pragma comment(lib, "poco_foundation-static.lib")
#endif


#if defined(_MSC_VER)
#pragma comment(lib, "IPHLPAPI.lib") //Becuase of poco needing this
#endif
