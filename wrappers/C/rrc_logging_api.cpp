#pragma hdrstop
#include "rrLogger.h"
#include "rrException.h"
#include "rrc_api.h"
#include "rrc_logging_api.h"
#include "rrc_cpp_support.h"
#include "rrStringUtils.h"
//---------------------------------------------------------------------------

namespace rrc
{
using namespace rr;
using namespace std;

bool rrcCallConv enableLoggingToConsole()
{
    try
    {
        Logger::enableConsoleLogging();
        return true;
    }
    catch_bool_macro
}

bool rrcCallConv disableLoggingToConsole()
{
    try
    {
        Logger::disableConsoleLogging();
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

        Logger::enableFileLogging(logFile);
        return true;
    }
    catch_bool_macro
}

bool rrcCallConv disableLoggingToFile()
{
    try
    {
        Logger::disableFileLogging();
        return true;
    }
    catch_bool_macro
}

bool rrcCallConv setLogLevel(const char* _lvl)
{
    try
    {
        Logger::Level lvl = Logger::stringToLevel(_lvl);
        Logger::setLevel(lvl);
        return true;
    }
    catch_bool_macro
}

char* rrcCallConv getLogLevel()
{
    try
    {
        string level = Logger::getCurrentLevelAsString();
        char* lvl = createText(level.c_str());
        return lvl;
    }
    catch_ptr_macro
}

char* rrcCallConv getLogFileName()
{
    try
    {
        return createText(Logger::getFileName().c_str());
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
