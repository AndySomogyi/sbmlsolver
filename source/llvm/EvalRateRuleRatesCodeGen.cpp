/*
 * rrLLVMEvalRateRuleRatesCodeGen.cpp
 *
 *  Created on: Aug 2, 2013
 *      Author: andy
 */

#include "EvalRateRuleRatesCodeGen.h"
#include "LLVMException.h"
#include "ASTNodeCodeGen.h"
#include "ASTNodeFactory.h"
#include "ModelDataSymbolResolver.h"
#include "rrLogger.h"
#include <sbml/math/ASTNode.h>
#include <sbml/math/FormulaFormatter.h>
#include <Poco/Logger.h>


using namespace libsbml;
using namespace llvm;
using namespace std;


namespace rr
{

const char* EvalRateRuleRatesCodeGen::FunctionName = "evalRateRuleRates";

EvalRateRuleRatesCodeGen::EvalRateRuleRatesCodeGen(
        const ModelGeneratorContext &mgc) :
        CodeGenBase<EvalRateRuleRates_FunctionPtr>(mgc)
{
}

EvalRateRuleRatesCodeGen::~EvalRateRuleRatesCodeGen()
{
}

Value* EvalRateRuleRatesCodeGen::codeGen()
{
    Value *modelData = 0;

    codeGenVoidModelDataHeader(FunctionName, modelData);

    ModelDataLoadSymbolResolver resolver(modelData,model,modelSymbols,
            dataSymbols,builder);
    ModelDataIRBuilder mdbuilder(modelData, dataSymbols, builder);
    ASTNodeCodeGen astCodeGen(builder, resolver);
    ASTNodeFactory nodes;

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
