#pragma hdrstop
#include "telLogger.h"
#include <Poco/Logger.h>
#include <Poco/AsyncChannel.h>
#include <Poco/ConsoleChannel.h>
#include <Poco/SimpleFileChannel.h>
#include <Poco/SplitterChannel.h>
#include <Poco/FormattingChannel.h>
#include <Poco/PatternFormatter.h>
#include <Poco/Formatter.h>
#include <Poco/AutoPtr.h>
#include <Poco/LogStream.h>
#include <Poco/Mutex.h>

#include <iostream>
#include <map>
#include <assert.h>

namespace tlp
{

using namespace std;
using Poco::AsyncChannel;
using Poco::ConsoleChannel;
using Poco::AutoPtr;
using Poco::Message;
using Poco::SimpleFileChannel;
using Poco::SplitterChannel;
using Poco::Formatter;
using Poco::FormattingChannel;
using Poco::PatternFormatter;
using Poco::Mutex;

// owned by poco, it takes care of clearing in static dtor.
static Poco::Logger *pocoLogger = 0;
volatile int logLevel = -1;
const Logger::Level defaultLogLevel = Logger::LOG_NOTICE;
static std::string logFileName;

// ******** The RoadRunner Logging Chain *************//
// pocoLogger
//     |-> FormattingChannel
//             |-> SplitterChannel
//                     |-> ConsoleChannel : consoleChannel
//                     |-> SimpleFileChannel: simpleFileChannel
//
// ****************************************************//

// owned by the poco splitter channel which in turn is owned by the
// poco logger.
static SimpleFileChannel *simpleFileChannel = 0;
static ConsoleChannel *consoleChannel = 0;

static Mutex loggerMutex;

/**
 * get the splitter channel that is in our logging chain.
 */
static SplitterChannel* getSplitterChannel();

/**
 * get the pattern formatter that is in our logging chain.
 */
static PatternFormatter *getPatternFormatter();

Poco::Logger& getLogger()
{
    Mutex::ScopedLock lock(loggerMutex);

    if (pocoLogger == 0)
    {
        pocoLogger = &Poco::Logger::get("Tellurium");

        // first time this is called, channels better be null
        assert(consoleChannel       == 0 && "consoleChannel is not null at init time");
        assert(simpleFileChannel    == 0 && "simpleFileChannel is not null at init time");

        // split the messages into console and file
        AutoPtr<SplitterChannel> splitter(new SplitterChannel());

        // default is console channel,
        // one of two possible terminal channels
        consoleChannel = new ConsoleChannel();

        // let the logger manage ownership of the channels, we keep then around
        // so we can know when to add or remove them.
        splitter->addChannel(consoleChannel);
        consoleChannel->release();

        AutoPtr<PatternFormatter> pf(new PatternFormatter());
        pf->setProperty("pattern", "%p: %t");

        AutoPtr<FormattingChannel> fc(new FormattingChannel(pf, splitter));

        // first in chain of channels
        pocoLogger->setChannel(fc);

        // sanity check here, make sure we're set up right
        getSplitterChannel();
        getPatternFormatter();

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

    if (level >= LOG_FATAL && level <= LOG_TRACE)
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

    SplitterChannel *splitter = getSplitterChannel();

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
        SplitterChannel *splitter = getSplitterChannel();

        // default is console channel
        consoleChannel = new ConsoleChannel();

        // let the logger manage ownership of the channels, we keep then around
        // so we can know when to add or remove them.
        splitter->addChannel(consoleChannel);

        consoleChannel->release();
    }
}

void Logger::disableFileLogging()
{
    Mutex::ScopedLock lock(loggerMutex);

    if (simpleFileChannel)
    {
        SplitterChannel *splitter = getSplitterChannel();

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
        SplitterChannel *splitter = getSplitterChannel();

        simpleFileChannel = new SimpleFileChannel();
        simpleFileChannel->setProperty("path", fileName);
        simpleFileChannel->setProperty("rotation", "never");

        logFileName = simpleFileChannel->getProperty("path");

        splitter->addChannel(simpleFileChannel);
        simpleFileChannel->release();
    }
}

void Logger::setFormattingPattern(const std::string &pattern)
{
    Mutex::ScopedLock lock(loggerMutex);

    PatternFormatter *p = getPatternFormatter();
    if (p)
    {
        p->setProperty(PatternFormatter::PROP_PATTERN, pattern);
    }
}

std::string Logger::getFormattingPattern()
{
    Mutex::ScopedLock lock(loggerMutex);

    PatternFormatter *p = getPatternFormatter();
    return p ? p->getProperty(PatternFormatter::PROP_PATTERN) : string();
}

static SplitterChannel* getSplitterChannel()
{
    getLogger();
    FormattingChannel *fc = dynamic_cast<FormattingChannel*>(pocoLogger->getChannel());
    assert(fc && "the first channel in the roadrunner logger should be a formatting channel");
    SplitterChannel *sc = dynamic_cast<SplitterChannel*>(fc->getChannel());
    assert(sc && "could not get SplitterChannel from FormattingChannel");
    return sc;
}

static PatternFormatter *getPatternFormatter()
{
    getLogger();
    FormattingChannel *fc = dynamic_cast<FormattingChannel*>(pocoLogger->getChannel());
    assert(fc && "the first channel in the roadrunner logger should be a formatting channel");
    PatternFormatter *pf = dynamic_cast<PatternFormatter*>(fc->getFormatter());
    assert(pf && "formatter attached to pattern formatter is not a PatternFormatter");
    return pf;
}

Logger::Level Logger::stringToLevel(const std::string& str)
{
    std::string upstr = str;
    std::transform(upstr.begin(), upstr.end(), upstr.begin(), ::toupper);

    if (upstr == "LOG_FATAL")
    {
        return LOG_FATAL;
    }
    else if(upstr == "LOG_CRITICAL")
    {
        return LOG_CRITICAL;
    }
    else if(upstr == "LOG_ERROR" || upstr == "ERROR")
    {
        return LOG_ERROR;
    }
    else if(upstr == "LOG_WARNING" || upstr == "WARNING")
    {
        return LOG_WARNING;
    }
    else if(upstr == "LOG_NOTICE")
    {
        return LOG_NOTICE;
    }
    else if(upstr == "LOG_INFORMATION" || upstr == "INFO")
    {
        return LOG_INFORMATION;
    }
    else if(upstr == "LOG_DEBUG")
    {
        return LOG_DEBUG;
    }
    else if(upstr == "LOG_TRACE" || upstr == "DEBUG")
    {
        return LOG_TRACE;
    }
    else
    {
        return LOG_CURRENT;
    }
}

std::string Logger::levelToString(int level)
{
    switch (level)
    {
    case Message::PRIO_FATAL:
        return "LOG_FATAL";
        break;
    case Message::PRIO_CRITICAL:
        return "LOG_CRITICAL";
        break;
    case Message::PRIO_ERROR:
        return "LOG_ERROR";
        break;
    case Message::PRIO_WARNING:
        return "LOG_WARNING";
        break;
    case Message::PRIO_NOTICE:
        return "LOG_NOTICE";
        break;
    case Message::PRIO_INFORMATION:
        return "LOG_INFORMATION";
        break;
    case Message::PRIO_DEBUG:
        return "LOG_DEBUG";
        break;
    case Message::PRIO_TRACE:
        return "LOG_TRACE";
        break;
    default:
        return "LOG_CURRENT";
    }
    return "LOG_CURRENT";
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
        SplitterChannel *splitter = getSplitterChannel();
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
    //cout << __FUNC__ << endl;
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


