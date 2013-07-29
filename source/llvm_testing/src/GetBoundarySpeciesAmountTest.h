/*
 * GetBoundarySpeciesAmountTest.h
 *
 *  Created on: Jul 28, 2013
 *      Author: andy
 */

#ifndef GETBOUNDARYSPECIESAMOUNTTEST_H_
#define GETBOUNDARYSPECIESAMOUNTTEST_H_

#include <string>

namespace rr
{

class GetBoundarySpeciesAmountTest
{
public:
    GetBoundarySpeciesAmountTest(const std::string& version, int caseNumber);
    ~GetBoundarySpeciesAmountTest();

    void test();
};

} /* namespace rr */
#endif /* GETBOUNDARYSPECIESAMOUNTTEST_H_ */
