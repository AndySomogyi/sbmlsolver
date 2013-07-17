#ifndef rrLoggerH
#define rrLoggerH
#include <sstream>
#include <iostream>
#include "rrExporter.h"
#include "rrLogLevel.h"

namespace Poco {
class Logger;
}

namespace rr
{

/**
 * Poco LogStream dumps to the log when a newline i.e. std::endl is encountered,
 * howeve the old proprietary logging system dumps basically when the stream object
 * goes out of scope.
 *
 * This object allows us to to use the new Poco logging system and maintain
 * compatability with all existing code.
 *
 * This object is returne from the rr::Logger, exposes a ostream interface, and
 * and dumps to the log when it goes out of scope.
 */
class RR_DECLSPEC LoggingBuffer:  public std::stringstream
{
public:
    LoggingBuffer(LogLevel);

    /**
     * dump the contents of the stringstream to the log.
     */
    ~LoggingBuffer();

private:
    LogLevel level;
};

/**
 * legacy struct, names the log file.
 */
struct LogFile
{
    LogFile(const char* name) : name(name) {}
    std::string name;
};

/**
 * Wraps the old proprietary logger interface around the Poco logging framework.
 */
class RR_DECLSPEC Logger
{
public:
    Logger();
    ~Logger();

    static void SetCutOffLogLevel(LogLevel level);

    static LogLevel GetLogLevel();

    static void StopLogging();

    static void enableLoggingToConsole();

    static void disableLoggingToConsole();

    void Init(const std::string&, int);

    void Init(const std::string&, int, LogFile*);

    static std::string GetCurrentLogLevel();

    static std::string GetLogFileName();

private:

    /**
     * pointer to legacy logfile struct given in ctor,
     * we own this.
     */
    LogFile *logFile;
};

/**
 * the real logger is actually a Poco::Logger named "RoadRunner", i.e.
 * Poco::Logger::get("RoadRunner").
 *
 * This returns that logger.
 */
RR_DECLSPEC Poco::Logger &getLogger();

/**
 * legacy macro to get the old proprietary logger interface.
 */
#define gLog Logger()

#define Log(level) std::cout

//#ifndef NO_LOGGER
//#define Log(level) \
//    if (level > rr::GetHighestLogLevel()) { ; }\
//    else if (level > gLog.GetLogLevel()) { ; } \
//    else LoggingBuffer(level)
//#else
//#define Log(level) \
//    if (true) {  }\
//    else \
//    LoggingBuffer(level)
//#endif

} /* namespace rr */
#endif /* rrLoggerH */
