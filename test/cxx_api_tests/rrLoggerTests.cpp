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

TEST_F(rrLoggerTests, getSplitterChannel) {
    Poco::Logger &logger = getLogger();
    std::cout << typeid(*logger.getChannel().get()).name() << std::endl;
//    auto & logger = pocoLogger->getChannel();
}

TEST_F(rrLoggerTests, CheckLoggerConsoleInformation) {
    Poco::Logger &logger = getLogger();
    logger.setLevel(Logger::LOG_INFORMATION);
    logger.information("For your information");

}


TEST_F(rrLoggerTests, CheckLoggerConsolCURRENT) {

}

TEST_F(rrLoggerTests, CheckLoggerConsolFATAL) {

}

TEST_F(rrLoggerTests, CheckLoggerConsolCRITICAL) {

}

TEST_F(rrLoggerTests, CheckLoggerConsolERROR) {

}

TEST_F(rrLoggerTests, CheckLoggerConsolWARNING) {

}

TEST_F(rrLoggerTests, CheckLoggerConsolNOTICE) {

}

TEST_F(rrLoggerTests, CheckLoggerConsolINFORMATION) {

}

TEST_F(rrLoggerTests, CheckLoggerConsolDEBUG) {

}

TEST_F(rrLoggerTests, CheckLoggerConsolTRACE) {

}

TEST_F(rrLoggerTests, CheckLoggerFileCURRENT) {
}

TEST_F(rrLoggerTests, CheckLoggerFileFATAL) {
}

TEST_F(rrLoggerTests, CheckLoggerFileCRITICAL) {
}

TEST_F(rrLoggerTests, CheckLoggerFileERROR) {
}

TEST_F(rrLoggerTests, CheckLoggerFileWARNING) {
}

TEST_F(rrLoggerTests, CheckLoggerFileNOTICE) {
}

TEST_F(rrLoggerTests, CheckLoggerFileINFORMATION) {
}

TEST_F(rrLoggerTests, CheckLoggerFileDEBUG) {
}

TEST_F(rrLoggerTests, CheckLoggerFileTRACE) {
}





























