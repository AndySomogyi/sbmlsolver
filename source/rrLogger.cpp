#pragma hdrstop
#include "rrLogger.h"
#include "rrOSSpecifics.h"

#include <Poco/Logger.h>
#include <Poco/AsyncChannel.h>
#include <Poco/ConsoleChannel.h>
#include <Poco/SimpleFileChannel.h>
#include <Poco/SplitterChannel.h>
#include <Poco/AutoPtr.h>
#include <Poco/LogStream.h>
#include <Poco/Mutex.h>

#include <iostream>
#include <map>
#include <assert.h>

namespace rr
{

using namespace std;
using Poco::AsyncChannel;
using Poco::ConsoleChannel;
using Poco::AutoPtr;
using Poco::Message;
using Poco::SimpleFileChannel;
using Poco::SplitterChannel;
using Poco::Mutex;

// owned by poco, it takes care of clearing in static dtor.
static Poco::Logger *pocoLogger = 0;
volatile int logLevel = -1;
const Logger::Level defaultLogLevel = Logger::PRIO_NOTICE;
static std::string logFileName;

// owned by the poco splitter channel which in turn is owned by the
// poco logger.
static SimpleFileChannel *simpleFileChannel = 0;
static ConsoleChannel *consoleChannel = 0;

static Mutex loggerMutex;

Poco::Logger& getLogger()
{
    if (pocoLogger == 0)
    {
        Mutex::ScopedLock lock(loggerMutex);

        pocoLogger = &Poco::Logger::get("RoadRunner");

        // first time this is called, channels better be null
        assert(consoleChannel == 0 && "consoleChannel is not null at init time");
        assert(simpleFileChannel == 0 && "simpleFileChannel is not null at init time");

        SplitterChannel *splitter = new SplitterChannel();
        pocoLogger->setChannel(splitter);
        splitter->release();

        splitter = dynamic_cast<SplitterChannel*>(pocoLogger->getChannel());
        assert(splitter && "could not get splitter channel from logger");

        // default is console channel
        consoleChannel = new ConsoleChannel();

        // let the logger manage ownership of the channels, we keep then around
        // so we can know when to add or remove them.
        splitter->addChannel(consoleChannel);
        consoleChannel->release();

        pocoLogger->setLevel(defaultLogLevel);

        logLevel = pocoLogger->getLevel();
    }
    return *pocoLogger;
}


void Logger::setLevel(int level)
{
    Mutex::ScopedLock lock(loggerMutex);

    // make sure we have a logger, other funcs rely on this behavior.
    getLogger();

    if (level >= PRIO_FATAL && level <= PRIO_TRACE)
    {
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


void Logger::disableLogging()
{
    Mutex::ScopedLock lock(loggerMutex);

    getLogger();

    SplitterChannel *splitter = dynamic_cast<SplitterChannel*>(pocoLogger->getChannel());
    assert(splitter && "could not get splitter channel from logger");

    splitter->close();

    consoleChannel = 0;
    simpleFileChannel = 0;
    logFileName = "";
}

void Logger::enableConsoleLogging(int level)
{
    Mutex::ScopedLock lock(loggerMutex);

    Logger::setLevel(level);

    if (!consoleChannel)
    {
        SplitterChannel *channel = dynamic_cast<SplitterChannel*>(pocoLogger->getChannel());

        assert(channel && "could not get splitter channel from logger");

        // default is console channel
        consoleChannel = new ConsoleChannel();

        // let the logger manage ownership of the channels, we keep then around
        // so we can know when to add or remove them.
        channel->addChannel(consoleChannel);

        consoleChannel->release();
    }
}

void Logger::disableFileLogging()
{
    Mutex::ScopedLock lock(loggerMutex);

    if (simpleFileChannel)
    {
        SplitterChannel *splitter = dynamic_cast<SplitterChannel*>(pocoLogger->getChannel());
        assert(splitter && "could not get splitter channel from logger");

        splitter->removeChannel(simpleFileChannel);
        simpleFileChannel = 0;
        logFileName = "";
    }
}

void Logger::enableFileLogging(const std::string& fileName, int level)
{
    Mutex::ScopedLock lock(loggerMutex);

    Logger::setLevel(level);

    if (!simpleFileChannel)
    {
        SplitterChannel *splitter = dynamic_cast<SplitterChannel*>(pocoLogger->getChannel());
        assert(splitter && "could not get splitter channel from logger");

        simpleFileChannel = new SimpleFileChannel();
        simpleFileChannel->setProperty("path", fileName);
        simpleFileChannel->setProperty("rotation", "never");

        logFileName = simpleFileChannel->getProperty("path");

        splitter->addChannel(simpleFileChannel);
        simpleFileChannel->release();
    }
}

Logger::Level Logger::stringToLevel(const std::string& str)
{
    std::string upstr = str;
    std::transform(upstr.begin(), upstr.end(), upstr.begin(), ::toupper);

    if (upstr == "PRIO_FATAL")
    {
        return PRIO_FATAL;
    }
    else if(upstr == "PRIO_CRITICAL")
    {
        return PRIO_CRITICAL;
    }
    else if(upstr == "PRIO_ERROR")
    {
        return PRIO_ERROR;
    }
    else if(upstr == "PRIO_WARNING")
    {
        return PRIO_WARNING;
    }
    else if(upstr == "PRIO_NOTICE")
    {
        return PRIO_NOTICE;
    }
    else if(upstr == "PRIO_INFORMATION")
    {
        return PRIO_INFORMATION;
    }
    else if(upstr == "PRIO_DEBUG")
    {
        return PRIO_DEBUG;
    }
    else if(upstr == "PRIO_TRACE")
    {
        return PRIO_DEBUG;
    }
    else
    {
        return PRIO_CURRENT;
    }
}

std::string Logger::levelToString(int level)
{
    switch (level)
    {
    case Message::PRIO_FATAL:
        return "PRIO_FATAL";
    case Message::PRIO_CRITICAL:
        return "PRIO_CRITICAL";
    case Message::PRIO_ERROR:
        return "PRIO_ERROR";
    case Message::PRIO_WARNING:
        return "PRIO_WARNING";
    case Message::PRIO_NOTICE:
        return "PRIO_NOTICE";
    case Message::PRIO_INFORMATION:
        return "PRIO_INFORMATION";
    case Message::PRIO_DEBUG:
        return "PRIO_DEBUG";
    case Message::PRIO_TRACE:
        return "PRIO_DEBUG";
    default:
        return "UNKNOWN";
    }
    return "UNKNOWN";
}

std::string Logger::getCurrentLevelAsString()
{
    return Logger::levelToString(logLevel);
}

void Logger::disableConsoleLogging()
{
    Mutex::ScopedLock lock(loggerMutex);

    if (consoleChannel)
    {
        SplitterChannel *splitter = dynamic_cast<SplitterChannel*>(pocoLogger->getChannel());
        assert(splitter && "could not get splitter channel from logger");

        splitter->removeChannel(consoleChannel);
        consoleChannel = 0;
    }
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


