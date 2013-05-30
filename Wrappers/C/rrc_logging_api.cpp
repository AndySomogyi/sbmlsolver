//---------------------------------------------------------------------------
#pragma hdrstop
#include "rrLogger.h"
#include "rrException.h"
#include "rrc_api.h"
#include "rrc_logging_api.h"
#include "rrc_cpp_support.h"

namespace rrc
{
using namespace rr;

bool rrcCallConv enableLoggingToConsole()
{
    try
    {
        LogOutput::mLogToConsole = true;
        return true;
    }
    catch(const Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
        return false;
    }
}

bool rrcCallConv enableLoggingToFile(RRHandle handle)
{
    try
    {
        RoadRunner* rri = castFrom(handle);
        char* tempFolder = getTempFolder(handle);
		string logFile = joinPath(tempFolder, "RoadRunner.log") ;
        freeText(tempFolder);

        gLog.Init("", gLog.GetLogLevel(), new LogFile(logFile.c_str()));
        return true;
    }
    catch(const Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
          return false;
    }
}

bool rrcCallConv setLogLevel(const char* _lvl)
{
    try
    {
        LogLevel lvl = GetLogLevel(_lvl);
        gLog.SetCutOffLogLevel(lvl);
        return true;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
          return false;
    }
}

char* rrcCallConv getLogLevel()
{
    try
    {
        string level = gLog.GetCurrentLogLevel();
        char* lvl = createText(level.c_str());
        return lvl;
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
          return NULL;
    }
}

char* rrcCallConv getLogFileName()
{
    try
    {
        return createText(gLog.GetLogFileName().c_str());
    }
    catch(Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
          return NULL;
    }
}

void rrcCallConv logMsg(CLogLevel lvl, const char* msg)
{
    try
    {
        Log((LogLevel) lvl)<<msg;
    }
    catch(const Exception& ex)
    {
        stringstream msg;
        msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
    }
}

}
