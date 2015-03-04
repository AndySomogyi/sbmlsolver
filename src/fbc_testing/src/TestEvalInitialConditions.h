/*
 * TestEvalInitialConditions.h
 *
 *  Created on: Jul 20, 2013
 *      Author: andy
 */

#ifndef TESTEVALINITIALCONDITIONS_H_
#define TESTEVALINITIALCONDITIONS_H_

#include "TestBase.h"

namespace rr
{

class TestEvalInitialConditions: public TestBase
{
public:
    TestEvalInitialConditions(const std::string& compiler,
            const std::string& version, int caseNumber);
    virtual ~TestEvalInitialConditions();
    bool test();
};

} /* namespace rr */
#endif /* TESTEVALINITIALCONDITIONS_H_ */
