#ifndef rrOSSpecificsH
#define rrOSSpecificsH

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
#define __func__ "not defined in VS"
#endif

#if defined(__CODEGEARC__)
#pragma warn -8012 			//comparing unsigned and signed
#pragma warn -8004 			//variable never used
#endif

#if defined(WIN32)
#define callConv __cdecl
#else
#define callConv
#endif


//---------------------------------------------------------------------------
#if defined (__MINGW32__) || defined(__linux)
#define __FUNC__ __FUNCTION__
#endif

#ifdef _MSC_VER

#include <stdarg.h>
#define snprintf c99_snprintf
int c99_vsnprintf(char* str, size_t size, const char* format, va_list ap);

inline int c99_snprintf(char* str, size_t size, const char* format, ...)
{
    int count;
    va_list ap;

    va_start(ap, format);
    count = c99_vsnprintf(str, size, format, ap);
    va_end(ap);

    return count;
}

inline int c99_vsnprintf(char* str, size_t size, const char* format, va_list ap)
{
    int count = -1;

    if (size != 0)
        count = _vsnprintf_s(str, size, _TRUNCATE, format, ap);
    if (count == -1)
        count = _vscprintf(format, ap);

    return count;
}

#endif // _MSC_VER
#endif
