#pragma hdrstop
#include "rrLogger.h"
#include "rrException.h"
#include "rrc_api.h"
#include "rrc_logging_api.h"
#include "rrc_cpp_support.h"
//---------------------------------------------------------------------------

namespace rrc
{
using namespace rr;

bool rrcCallConv enableLoggingToConsole()
{
    try
    {
        Logger::enableLoggingToConsole();
        return true;
    }
    catch_bool_macro
}

bool rrcCallConv disableLoggingToConsole()
{
    try
    {
        Logger::disableLoggingToConsole();
        return true;
    }
    catch_bool_macro
}

bool rrcCallConv enableLoggingToFile(RRHandle handle)
{
    try
    {
        char* tempFolder = getTempFolder(handle);
        string logFile = joinPath(tempFolder, "RoadRunner.log") ;
        rr::freeText(tempFolder);

        gLog.Init("", gLog.GetLogLevel(), new LogFile(logFile.c_str()));
        return true;
    }
    catch_bool_macro
}

bool rrcCallConv disableLoggingToFile()
{
    try
    {
        gLog.StopLogging();
        return true;
    }
    catch_bool_macro
}

bool rrcCallConv setLogLevel(const char* _lvl)
{
    try
    {
        LogLevel lvl = GetLogLevel(_lvl);
        gLog.SetCutOffLogLevel(lvl);
        return true;
    }
    catch_bool_macro
}

char* rrcCallConv getLogLevel()
{
    try
    {
        string level = gLog.GetCurrentLogLevel();
        char* lvl = createText(level.c_str());
        return lvl;
    }
    catch_ptr_macro
}

char* rrcCallConv getLogFileName()
{
    try
    {
        return createText(gLog.GetLogFileName().c_str());
    }
    catch_ptr_macro
}

void rrcCallConv logMsg(CLogLevel lvl, const char* msg)
{
    try
    {
        Log((LogLevel) lvl)<<msg;
    }
    catch_void_macro
}

}
