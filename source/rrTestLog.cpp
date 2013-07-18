/*
 * rrTestLog.cpp
 *
 *  Created on: Jul 17, 2013
 *      Author: andy
 */

#include "rrTestLog.h"

#include <Poco/Logger.h>
#include <Poco/AsyncChannel.h>
#include <Poco/ConsoleChannel.h>
#include <Poco/AutoPtr.h>
#include "Poco/LogStream.h"
#include "Poco/Logger.h"

#include <sstream>

using Poco::Logger;
using Poco::AsyncChannel;
using Poco::ConsoleChannel;
using Poco::AutoPtr;

namespace rr
{

TestLog::TestLog()
{
    // TODO Auto-generated constructor stub

}

TestLog::~TestLog()
{
    // TODO Auto-generated destructor stub
}

void TestLog::test()
{
    AutoPtr <ConsoleChannel> pCons (new ConsoleChannel);
    AutoPtr <AsyncChannel> pAsync (new AsyncChannel (pCons));
    Logger::root().setChannel(pAsync);
    Logger & logger = Logger :: get ("TestLogger");
    for (int i = 0; i <10; ++i)
        logger.information ("This is a test");
}

} /* namespace rr */
