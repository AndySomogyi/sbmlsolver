#ifdef USE_PCH
#include "rr_pch.h"
#endif
#pragma hdrstop

#if defined(_WIN32) || defined(WIN32)
#include <windows.h>

#if defined(__CODEGEAR__)
#pragma argsused
#endif

int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
    return 1;
}

#endif
