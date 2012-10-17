#ifdef USE_PCH
#include "rr_pch.h"
#endif
#pragma hdrstop
#include <strstream>
#include <iomanip>
#include "Args.h"
//---------------------------------------------------------------------------
#if defined(__CODEGEAR__)
#pragma package(smart_init)
#endif

Args::Args()
:
UseOSTempFolder(false),
OnlyCompile(false),
Pause(false),
CurrentLogLevel(lInfo),
ModelFileName(""),
DataOutputFolder(""),
TempDataFolder("."),
TestSuiteModelsPath("."),
StartTime(0),
Duration(5),
EndTime(StartTime + Duration),
CaseNumber(1),
ErrorThreshold(1.e-6),
Steps(50)
{
}


