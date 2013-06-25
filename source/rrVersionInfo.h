#ifndef rrVersionInfoH
#define rrVersionInfoH


const char* RR_VERSION =  "0.9.5";

#define RR_STRINGIZE2(s) #s
#define RR_STRINGIZE(s) RR_STRINGIZE2(s)


#if defined(__llvm__)
    #define RR_COMPILER "clang " RR_STRINGIZE(__clang_version__)
#endif

#if defined(__GNUC__) && !defined(__llvm__)
    #define RR_COMPILER "gcc " RR_STRINGIZE(__GNUC__) "." RR_STRINGIZE(__GNUC_MINOR__) "." RR_STRINGIZE(__GNUC_PATCHLEVEL__)
#endif

#if defined (_MSC_VER)
    #if (_MSC_VER == 1700)
        #define RR_COMPILER "Microsoft Visual Studio 2012"
    #elif (_MSC_VER == 1600)
        #define RR_COMPILER "Microsoft Visual Studio 2010"
    #elif (_MSC_VER == 1500)
        #define RR_COMPILER "Microsoft Visual Studio 2008"
    #elif (_MSC_VER == 1400)
        #define RR_COMPILER "Microsoft Visual Studio 2005"
    #elif (_MSC_VER == 1310)
        #define RR_COMPILER "Microsoft Visual Studio 2003"
    #else
        #define RR_COMPILER "Microsoft Visual Studio Unknown Version"
    #endif
#endif

#if defined (__BORLANDC__)
    #define RR_COMPILER "Borland"
#endif

#if !defined(RR_COMPILER)
    #define RR_COMPILER "Unknown, but evidently working compiler"
#endif


#endif
