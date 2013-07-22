#ifndef rrLogLevelH
#define rrLogLevelH
#include <string>
#include "rrExporter.h"
#include "rrLogger.h"


using std::string;
namespace rr
{

RR_DECLSPEC string ToUpperCase(const string& inStr);
RR_DECLSPEC int GetHighestLogLevel();
RR_DECLSPEC LogLevel GetLogLevel(const string& level);
RR_DECLSPEC string GetLogLevelAsString(int level);
RR_DECLSPEC LogLevel GetLogLevel(int lvl);

}
#endif
