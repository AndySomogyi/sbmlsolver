/*
 * GetBoundarySpeciesAmountTest.h
 *
 *  Created on: Jul 28, 2013
 *      Author: andy
 */

#ifndef GETBOUNDARYSPECIESAMOUNTTEST_H_
#define GETBOUNDARYSPECIESAMOUNTTEST_H_

#include "TestBase.h"
#include <string>

namespace rr
{

class GetBoundarySpeciesAmountTest: public TestBase
{
public:
    GetBoundarySpeciesAmountTest(const std::string& compiler,
            const std::string& version, int caseNumber);
    ~GetBoundarySpeciesAmountTest();

    bool test();
};

} /* namespace rr */
#endif /* GETBOUNDARYSPECIESAMOUNTTEST_H_ */
