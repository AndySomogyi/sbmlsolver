#pragma hdrstop
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
volatile int logLevel = -1;
const Logger::Level defaultLogLevel = Logger::PRIO_NOTICE;
static std::string logFileName;

Poco::Logger& getLogger()
{
    if (pocoLogger == 0)
    {
        AutoPtr<ConsoleChannel> pCons(new ConsoleChannel);
        Poco::Logger::root().setChannel(pCons);

        pocoLogger = &Poco::Logger::get("RoadRunner");

        // set the default level
        Poco::Logger::root().setLevel(defaultLogLevel);
        pocoLogger->setLevel(defaultLogLevel);

        logLevel = Poco::Logger::root().getLevel();
    }
    return *pocoLogger;
}


void Logger::setLevel(int level)
{
    if (level >= PRIO_FATAL && level <= PRIO_TRACE)
    {
        Poco::Logger::root().setLevel(level);
        getLogger().setLevel(level);
        logLevel = level;
    }
}

int Logger::getLevel()
{
    if (logLevel < 0)
    {
        getLogger();
    }
    return logLevel;
}


void Logger::stopLogging()
{
}

void Logger::init(const std::string& allocator, int level)
{
    if (level >= PRIO_FATAL && level <= PRIO_TRACE)
    {
        Poco::Logger::root().setLevel(level);
        logLevel = Poco::Logger::root().getLevel();
    }

    AutoPtr<ConsoleChannel> pCons(new ConsoleChannel);
    Poco::Logger::root().setChannel(pCons);
    Poco::Logger::root().setLevel(logLevel);

    Poco::Logger &logger = getLogger();
    AutoPtr<ConsoleChannel> pCons2(new ConsoleChannel);
    logger.setChannel(pCons);
    logger.setLevel(logLevel);

    logFileName = "";
}



void Logger::init(const std::string& allocator, int level, const std::string& fileName)
{
    // make sure we have a log...
    if (level < PRIO_FATAL || level > PRIO_TRACE)
    {
        level = PRIO_FATAL;
    }

    AutoPtr<SimpleFileChannel> pChannel(new SimpleFileChannel);
    pChannel->setProperty("path", fileName);
    pChannel->setProperty("rotation", "never");


    Poco::Logger::root().setLevel(level);

    Poco::Logger &logger = getLogger();

    logger.setChannel(pChannel);
    logger.setLevel(level);

    logLevel = level;
    logFileName = pChannel->getProperty("path");
}

std::string Logger::getLevelAsString()
{
    switch (logLevel)
    {
    case Message::PRIO_FATAL:
        return "PRIO_FATAL";
        break;
    case Message::PRIO_CRITICAL:
        return "PRIO_CRITICAL";
        break;
    case Message::PRIO_ERROR:
        return "PRIO_ERROR";
        break;
    case Message::PRIO_WARNING:
        return "PRIO_WARNING";
        break;
    case Message::PRIO_NOTICE:
        return "PRIO_NOTICE";
        break;
    case Message::PRIO_INFORMATION:
        return "PRIO_INFORMATION";
        break;
    case Message::PRIO_DEBUG:
        return "PRIO_DEBUG";
        break;
    case Message::PRIO_TRACE:
        return "PRIO_DEBUG";
        break;
    default:
        return "UNKNOWN";
        break;
    }
    return "UNKNOWN";
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

std::string Logger::getFileName()
{
    return logFileName;
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


