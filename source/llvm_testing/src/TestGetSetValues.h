/*
 * TestEvalInitialConditions.h
 *
 *  Created on: Jul 20, 2013
 *      Author: andy
 */

#ifndef TESTGETSETVALUES_H_
#define TESTGETSETVALUES_H_

#include "TestBase.h"

namespace rr
{

class TestGetSetValues: public TestBase
{
public:
    TestGetSetValues(const std::string& compiler,
            const std::string& version, int caseNumber);
    virtual ~TestGetSetValues();
    bool test();
};

} /* namespace rr */
#endif /* TESTEVALINITIALCONDITIONS_H_ */
