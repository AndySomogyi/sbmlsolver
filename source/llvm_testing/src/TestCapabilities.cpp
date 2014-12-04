/*
 * TestCapabilties.cpp
 *
 *  Created on: Sep 19, 2013
 *      Author: andy
 */
#pragma hdrstop
#include "TestCapabilities.h"
#include "rrRoadRunner.h"
#include "rrLogger.h"

namespace rr {

TestCapabilities::TestCapabilities(const std::string& compiler,
        const std::string& version, int caseNumber)
         : TestBase(compiler, version, caseNumber)
{

}

TestCapabilities::~TestCapabilities() {
    // TODO Auto-generated destructor stub
}

bool TestCapabilities::test()
{



    return true;
}

}
