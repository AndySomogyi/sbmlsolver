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

static Poco::Logger *pocoLogger = 0;

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


void Logger::SetCutOffLogLevel(int level)
{
    if (level >= PRIO_FATAL && level <= PRIO_TRACE)
    {
        Poco::Logger::root().setLevel(level);
    }
}

int Logger::GetLogLevel()
{
    return Poco::Logger::root().getLevel();
}


void Logger::StopLogging()
{
}

void Logger::Init(const std::string& allocator, int level)
{
    if (level >= PRIO_FATAL && level <= PRIO_TRACE)
    {
        Poco::Logger::root().setLevel(level);
    }
}

void Logger::Init(const std::string& allocator, int level, LogFile* logFile)
{
    // make sure we have a log...
    if (level < PRIO_FATAL || level > PRIO_TRACE)
    {
        level = PRIO_FATAL;
    }

    AutoPtr<SimpleFileChannel> pChannel(new SimpleFileChannel);
    pChannel->setProperty("path", logFile->name);
    pChannel->setProperty("rotation", "2 K");
    Poco::Logger::root().setChannel(pChannel);
    Poco::Logger::root().setLevel(level);

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
    AutoPtr<ConsoleChannel> pCons(new ConsoleChannel);
    AutoPtr<AsyncChannel> pAsync(new AsyncChannel(pCons));
    Poco::Logger::root().setChannel(pAsync);
}

void Logger::disableLoggingToConsole()
{
}

std::string Logger::GetLogFileName()
{
    return __FUNC__;
}

void LoggingBufferCtor()
{
    cout << __FUNC__ << endl;
}

LoggingBuffer::LoggingBuffer(int level, const char* file, int line) :
        file(file), line(line)
{
    if (level >= Message::PRIO_FATAL && level <= Message::PRIO_TRACE)
    {
        this->level = level;
    }
    else
    {
        // wrong level, so just set to error?
        this->level = Message::PRIO_ERROR;
    }
}

LoggingBuffer::~LoggingBuffer()
{
    Poco::Logger &logger = getLogger();
    switch (level)
    {
    case Message::PRIO_FATAL:
        logger.fatal(buffer.str(), file, line);
        break;
    case Message::PRIO_CRITICAL:
        logger.critical(buffer.str(), file, line);
        break;
    case Message::PRIO_ERROR:
        logger.error(buffer.str(), file, line);
        break;
    case Message::PRIO_WARNING:
        logger.warning(buffer.str(), file, line);
        break;
    case Message::PRIO_NOTICE:
        logger.notice(buffer.str(), file, line);
        break;
    case Message::PRIO_INFORMATION:
        logger.information(buffer.str(), file, line);
        break;
    case Message::PRIO_DEBUG:
        logger.debug(buffer.str(), file, line);
        break;
    case Message::PRIO_TRACE:
        logger.trace(buffer.str(), file, line);
        break;
    default:
        logger.error(buffer.str(), file, line);
        break;
    }
}

std::ostream& LoggingBuffer::stream()
{
    return buffer;
}

}


