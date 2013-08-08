/*
 * rrLLVMEvalRateRuleRatesCodeGen.cpp
 *
 *  Created on: Aug 2, 2013
 *      Author: andy
 */

#include "rrLLVMEvalRateRuleRatesCodeGen.h"
#include "rrLLVMException.h"
#include "rrLLVMASTNodeCodeGen.h"
#include "rrLLVMASTNodeFactory.h"
#include "rrLLVMModelDataSymbolResolver.h"
#include "rrLogger.h"
#include <sbml/math/ASTNode.h>
#include <sbml/math/FormulaFormatter.h>
#include <Poco/Logger.h>


using namespace libsbml;
using namespace llvm;
using namespace std;


namespace rr
{

const char* LLVMEvalRateRuleRatesCodeGen::FunctionName = "evalRateRuleRates";

LLVMEvalRateRuleRatesCodeGen::LLVMEvalRateRuleRatesCodeGen(
        const LLVMModelGeneratorContext &mgc) :
        LLVMCodeGenBase<LLVMEvalRateRuleRates_FunctionPtr>(mgc)
{
}

LLVMEvalRateRuleRatesCodeGen::~LLVMEvalRateRuleRatesCodeGen()
{
}

Value* LLVMEvalRateRuleRatesCodeGen::codeGen()
{
    Value *modelData = 0;

    codeGenVoidModelDataHeader(FunctionName, modelData);

    LLVMModelDataLoadSymbolResolver resolver(modelData,model,modelSymbols,
            dataSymbols,builder);
    LLVMModelDataIRBuilder mdbuilder(modelData, dataSymbols, builder);
    LLVMASTNodeCodeGen astCodeGen(builder, resolver);
    LLVMASTNodeFactory nodes;

    // iterate through all of the reaction, and generate code based on thier
    // kinetic rules.

    model->getListOfRules();
    const ListOfRules *rules = model->getListOfRules();

    for (int i = 0; i < rules->size(); ++i)
    {
        const RateRule *rateRule = dynamic_cast<const RateRule*>(rules->get(i));

        if (rateRule)
        {
            const ASTNode *math = rateRule->getMath();
            assert(math);

            Value *value = astCodeGen.codeGen(math);

            mdbuilder.createRateRuleRateStore(rateRule->getVariable(), value);
        }
    }

    builder.CreateRetVoid();

    return verifyFunction();
}

} /* namespace rr */
