#include <Poco/ConsoleChannel.h>
#include <Poco/AutoPtr.h>
#include "gtest/gtest.h"

#include "rrLogger.h"
#include "Poco/Logger.h"

using namespace rr;

class rrLoggerTests : public ::testing::Test {
public:
    rrLoggerTests() = default;

//    /**
//     * @brief returns a complete list of logging levels
//     * in a std::vector
//     */
//    static std::vector<Logger::Level> getLoggingLevels() {
//        return std::vector<Logger::Level>(
//                {
//                        Logger::LOG_CURRENT,
//                        Logger::LOG_FATAL,
//                        Logger::LOG_CRITICAL,
//                        Logger::LOG_ERROR,
//                        Logger::LOG_WARNING,
//                        Logger::LOG_NOTICE,
//                        Logger::LOG_INFORMATION,
//                        Logger::LOG_DEBUG,
//                        Logger::LOG_TRACE,
//                });
//    }
};

TEST(rrLerTests, s) {
//    Poco::AutoPtr <Poco::ConsoleChannel> pChannel(new Poco::ConsoleChannel);
//    Poco::Logger::root().setChannel(pChannel);
//    Poco::Logger &logger = Poco::Logger::get("TestLogger"); // inherits root channel

    Poco::Logger &logger = getLogger();
    logger.setLevel("information");
    for (int i = 0; i < 100; ++i)
        logger.information("Testing Roadrunner logger channel");
//    getLogger().log(Poco::Message(__FILE__, "asdfasdf", Poco::Message::PRIO_FATAL));
//    LoggingBuffer loggingBuffer(Logger::LOG_ERROR, __FILE__, __LINE__);
//    loggingBuffer.stream() << "words" << std::endl;
}

































