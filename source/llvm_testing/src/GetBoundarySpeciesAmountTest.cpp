/*
 * GetBoundarySpeciesAmountTest.cpp
 *
 *  Created on: Jul 28, 2013
 *      Author: andy
 */

#include "GetBoundarySpeciesAmountTest.h"
#include "rrLLVMGetBoundarySpeciesAmountCodeGen.h"
#include "rrLLVMModelGeneratorContext.h"

namespace rr
{


GetBoundarySpeciesAmountTest::~GetBoundarySpeciesAmountTest()
{
    // TODO Auto-generated destructor stub
}

} /* namespace rr */

rr::GetBoundarySpeciesAmountTest::GetBoundarySpeciesAmountTest(
        const std::string& version, int caseNumber)
{
}

void rr::GetBoundarySpeciesAmountTest::test()
{
    LLVMModelGeneratorContext ctx;

    LLVMGetBoundarySpeciesAmountCodeGen bsa(ctx);

    LLVMGetBoundarySpeciesAmountCodeGen::FunctionPtr functionPtr =
            bsa.createFunction();

    double result = functionPtr(0, 0);


    cout << "result: " << result;

}
