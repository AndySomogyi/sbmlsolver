/** @file rr_c_api_exporter.h */
#ifndef rr_c_api_exporterH
#define rr_c_api_exporterH

//Export/Import API functions
#if defined(STATIC_RR_C_API) || defined(__linux)
#define C_DECL_SPEC
#else
	#if defined(EXPORT_RR_C_API)
		/** \brief A reference to __declspec(dllexport) */
		#define C_DECL_SPEC __declspec(dllexport)
	#else
		/** \brief A reference to __declspec(dllexport) */
		#define C_DECL_SPEC __declspec(dllimport)
	#endif

#endif	//STATIC_RR_C_API

#if defined(_MSC_VER) || defined(__CODEGEARC__)
#define rrCallConv __stdcall
#elif defined(__linux)
#define rrCallConv
#endif

#endif
