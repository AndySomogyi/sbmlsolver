/*
 * TestEvalModel.h
 *
 *  Created on: Jul 23, 2013
 *      Author: andy
 */

#ifndef TESTEVALMODEL_H_
#define TESTEVALMODEL_H_

#include <TestBase.h>

namespace rr
{

class TestEvalModel: public TestBase
{
public:
    TestEvalModel(const std::string& version, int caseNumber);
    virtual ~TestEvalModel();
    bool test();
};

} /* namespace rr */
#endif /* TESTEVALMODEL_H_ */
