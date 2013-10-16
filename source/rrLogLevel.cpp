#pragma hdrstop
#include <algorithm>
#include "rrLogLevel.h"
#include <Poco/Message.h>

using namespace std;
using Poco::Message;

namespace rr
{

int GetHighestLogLevel()
{
    return lAny;
}

LogLevel GetLogLevel(const string& lvl)
{
    string level = ToUpperCase(lvl);
    if (level == "ANY")           return lAny;
    if (level == "DEBUG5")        return lDebug5;
    if (level == "DEBUG4")        return lDebug4;
    if (level == "DEBUG3")        return lDebug3;
    if (level == "DEBUG2")        return lDebug2;
    if (level == "DEBUG1")        return lDebug1;
    if (level == "DEBUG")         return lDebug;
    if (level == "INFO")          return lInfo;
    if (level == "WARNING")       return lWarning;
    if (level == "ERROR")         return lError;

    return lAny;
}

string GetLogLevelAsString(int level)
{
    const char* result;
    switch (level)
    {
    case Message::PRIO_FATAL:
        result = "PRIO_FATAL";
        break;
    case Message::PRIO_CRITICAL:
        result = "PRIO_CRITICAL";
        break;
    case Message::PRIO_ERROR:
        result = "PRIO_ERROR";
        break;
    case Message::PRIO_WARNING:
        result = "PRIO_WARNING";
        break;
    case Message::PRIO_NOTICE:
        result = "PRIO_NOTICE";
        break;
    case Message::PRIO_INFORMATION:
        result = "PRIO_INFORMATION";
        break;
    case Message::PRIO_DEBUG:
        result = "PRIO_DEBUG";
        break;
    case Message::PRIO_TRACE:
        result = "PRIO_DEBUG";
        break;
    default:
        result = "INVALID_LEVEL";
        break;
    }
    return result;
}

LogLevel GetLogLevel(int level)
{
    switch (level)
    {
    case -1:  return lShowAlways;
    case 0:   return lError;
    case 1:   return lWarning;
    case 2:   return lInfo;
    case 3:   return lDebug;
    case 4:   return lDebug1;
    case 5:   return lDebug2;
    case 6:   return lDebug3;
    case 7:   return lDebug4;
    case 8:   return lDebug5;
    case 9:   return lAny;
    default:
        return lAny;
    }
}

string ToUpperCase(const string& inStr)
{
    string rString(inStr);
    std::transform(rString.begin(), rString.end(), rString.begin(), ::toupper);
    return rString;
}

}

