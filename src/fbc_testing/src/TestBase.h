/*
 * TestBase.h
 *
 *  Created on: Jul 20, 2013
 *      Author: andy
 */

#ifndef TESTBASE_H_
#define TESTBASE_H_

#include "llvm/LLVMModelGenerator.h"
#include "llvm/LLVMExecutableModel.h"

#include <string>

namespace rr
{

class TestBase
{
public:
    TestBase(const std::string& compiler, const std::string& version, int caseNumber);
    virtual ~TestBase();

    virtual bool test() {return true;}

    ExecutableModel *model;
    std::string version;
    int caseNumber;
    std::string fileName;
    std::string compiler;
};

} /* namespace rr */
#endif /* TESTBASE_H_ */
