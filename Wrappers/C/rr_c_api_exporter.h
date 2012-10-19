/** @file rr_c_api_exporter.h */
#ifndef rr_c_api_exporterH
#define rr_c_api_exporterH
//---------------------------------------------------------------------------

//Export/Import API functions
#if defined (STATIC_RR_C_API)
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

#if defined(_MSC_VER)
#define rrCallConv _stdcall
#else if defined(__CODEGEARC__)
#define rrCallConv _stdcall
#endif

#endif
