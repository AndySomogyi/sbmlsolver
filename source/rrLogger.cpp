#include "rrLogger.h"
#include "rrOSSpecifics.h"

#include <Poco/Logger.h>
#include <Poco/AsyncChannel.h>
#include <Poco/ConsoleChannel.h>
#include <Poco/SimpleFileChannel.h>
#include <Poco/AutoPtr.h>
#include <Poco/LogStream.h>

#include <iostream>
#include <map>

namespace rr
{

using namespace std;
using Poco::AsyncChannel;
using Poco::ConsoleChannel;
using Poco::AutoPtr;
using Poco::Message;
using Poco::SimpleFileChannel;

//static Poco::Logger *pocoLogger = 0;

int levelToPriority(int level)
{
    int result = Message::PRIO_FATAL;
    switch (level)
    {
    case lShowAlways:     result = Message::PRIO_FATAL;
        break;
    case lError:          result = Message::PRIO_ERROR;
        break;
    case lWarning:        result = Message::PRIO_WARNING;
        break;
    case lInfo:           result = Message::PRIO_INFORMATION;
        break;
    case lDebug:          result = Message::PRIO_DEBUG;
        break;
    case lDebug1:         result = Message::PRIO_TRACE;
        break;
    case lDebug2:         result = Message::PRIO_TRACE;
        break;
    case lDebug3:         result = Message::PRIO_TRACE;
        break;
    case lDebug4:         result = Message::PRIO_TRACE;
        break;
    case lDebug5:         result = Message::PRIO_TRACE;
        break;
    case lAny:            result = Message::PRIO_TRACE;
        break;
    case lUser:           result = Message::PRIO_TRACE;
        break;
    }
    return result;
}

static Poco::Logger *pocoLogger = 0;
static LogLevel logLevel = lAny;

Poco::Logger& getLogger()
{
    if (pocoLogger == 0)
    {
        AutoPtr<ConsoleChannel> pCons(new ConsoleChannel);
        AutoPtr<AsyncChannel> pAsync(new AsyncChannel(pCons));
        Poco::Logger::root().setChannel(pAsync);

        pocoLogger = &Poco::Logger::get("RoadRunner");
    }
    return *pocoLogger;
}


void Logger::SetCutOffLogLevel(LogLevel level)
{
    logLevel = level;
}

LogLevel Logger::GetLogLevel()
{
    return logLevel;
}


void Logger::StopLogging()
{
}

void Logger::Init(const std::string& allocator, int level)
{
    logLevel = (LogLevel)level;
    Poco::Logger::root().setLevel(levelToPriority(level));
}

void Logger::Init(const std::string& allocator, int level, LogFile* logFile)
{
    // make sure we have a log...
    logLevel = (LogLevel)level;

    AutoPtr<SimpleFileChannel> pChannel(new SimpleFileChannel);
    pChannel->setProperty("path", logFile->name);
    pChannel->setProperty("rotation", "2 K");
    Poco::Logger::root().setChannel(pChannel);
    Poco::Logger::root().setLevel(levelToPriority(level));

    getLogger().trace("deleting pointer to LogFile struct...");
}

std::string Logger::GetCurrentLogLevel()
{
    return __FUNC__;
}

Logger::Logger()
{
}

Logger::~Logger()
{
}

void Logger::enableLoggingToConsole()
{
}

void Logger::disableLoggingToConsole()
{
}

std::string Logger::GetLogFileName()
{
    return __FUNC__;
}

LoggingBuffer::LoggingBuffer(LogLevel level) : level(level)
{
}

LoggingBuffer::~LoggingBuffer()
{
    Poco::Logger &logger = getLogger();
    switch (level)
    {
    case lShowAlways:
        logger.fatal(str());
        break;
    case lError:
        logger.error(str());
        break;
    case lWarning:
        logger.warning(str());
        break;
    case lInfo:
        logger.information(str());
        break;
    case lDebug:
        logger.debug(str());
        break;
    case lDebug1:
        logger.trace(str());
        break;
    case lDebug2:
        logger.trace(str());
        break;
    case lDebug3:
        logger.trace(str());
        break;
    case lDebug4:
        logger.trace(str());
        break;
    case lDebug5:
        logger.trace(str());
        break;
    case lAny:
        logger.trace(str());
        break;
    case lUser:
        logger.trace(str());
        break;
    default:
        logger.information(str());
        break;
    }
}

}


