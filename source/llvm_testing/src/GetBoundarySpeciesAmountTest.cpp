/*
 * GetBoundarySpeciesAmountTest.cpp
 *
 *  Created on: Jul 28, 2013
 *      Author: andy
 */
#pragma hdrstop
#include "GetBoundarySpeciesAmountTest.h"
#include "llvm/GetValuesCodeGen.h"
#include "llvm/ModelGeneratorContext.h"
#include "rrLogger.h"
#include <vector>

namespace rr
{
using namespace std;


GetBoundarySpeciesAmountTest::~GetBoundarySpeciesAmountTest()
{
}


GetBoundarySpeciesAmountTest::GetBoundarySpeciesAmountTest(
        const std::string& compiler, const std::string& version, int caseNumber)
            : TestBase(compiler, version, caseNumber)
{
}


bool rr::GetBoundarySpeciesAmountTest::test()
{
    /*
    LLVMModelGeneratorContext ctx;

    LLVMGetBoundarySpeciesAmountCodeGen bsa(ctx);

    LLVMGetBoundarySpeciesAmountCodeGen::FunctionPtr functionPtr =
            bsa.createFunction();

    double result = functionPtr(0, 0);


    cout << "result: " << result;

    return true;
    */

    model->evalInitialConditions();

    Log(Logger::LOG_INFORMATION) << model;



    int n = model->getNumBoundarySpecies();
    vector<double> values(n, 0);

    model->getBoundarySpeciesAmounts(n, 0, &values[0]);

    for(int i = 0; i < n; ++i)
    {
        Log(Logger::LOG_INFORMATION) << "boundary species " <<
                model->getBoundarySpeciesId(i) << ": " << values[i] << endl;
    }

    return true;



}


} /* namespace rr */

