#ifdef USE_PCH
#include "rr_pch.h"
#endif
#pragma hdrstop
#include <windows.h>

#if defined(__CODEGEAR__)
#pragma argsused
#endif

int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
    return 1;
}

