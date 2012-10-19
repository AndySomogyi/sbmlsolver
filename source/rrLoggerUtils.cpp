#ifdef USE_PCH
#include "rr_pch.h"
#endif
#pragma hdrstop
#include <algorithm>
#include <windows.h>
#include "rrLoggerUtils.h"
//---------------------------------------------------------------------------

namespace rr
{
//---------------------------------------------------------------------------
string GetLogTime(bool show_milli_sec)
{
	const int MAX_LEN = 200;
	char buffer[MAX_LEN];
	if (GetTimeFormatA(LOCALE_USER_DEFAULT, 0, 0, "HH':'mm':'ss", buffer, MAX_LEN) == 0)
	{
		return "Error in rr::GetTime()";
	}

	char result[100] = {0};
	if(show_milli_sec)
	{
		static DWORD first = GetTickCount();
		std::sprintf(result, "%s.%03ld", buffer, (long)(GetTickCount() - first) % 1000);
		return string(result);
		}
	else
	{
		return string(buffer);
	}
}

}

