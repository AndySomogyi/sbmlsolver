#ifndef rrExporterH
#define rrExporterH

#if defined(WIN32)
#if defined(STATIC_RR)
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


#if defined (__MINGW32__)
#undef RR_DECLSPEC
#endif

#if defined(_MSC_VER)
#pragma warning(disable : 4996) // _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4018) // int to unsigned int comparison
#pragma warning(disable : 4482) // prefixing enums...
#pragma warning(disable : 4251) // _CRT_SECURE_NO_WARNINGS
#pragma warning(disable : 4221) // empty cpp file

#define __FUNC__ "not defined in VS"
#endif

#if defined(__CODEGEARC__)
#pragma warn -8012 			//comparing unsigned and signed
#pragma warn -8004 			//variable never used
#endif

#endif
