/*
 * TestCapabilties.h
 *
 *  Created on: Sep 19, 2013
 *      Author: andy
 */

#ifndef TESTCAPABILTIES_H_
#define TESTCAPABILTIES_H_

#include "TestBase.h"

namespace rr
{

class TestCapabilities: public TestBase
{
public:


    TestCapabilities(const std::string& compiler,
            const std::string& version, int caseNumber);
    virtual ~TestCapabilities();
    bool test();
};
}

#endif /* TESTCAPABILTIES_H_ */
