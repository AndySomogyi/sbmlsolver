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
        LLVMCodeGenBase(mgc),
        func(0),
        modelData(0)
{
}

LLVMEvalRateRuleRatesCodeGen::~LLVMEvalRateRuleRatesCodeGen()
{
}

Value* LLVMEvalRateRuleRatesCodeGen::codeGen()
{
    // make the set init value function
    vector<Type*> argTypes;
    StructType *modelDataStructType = LLVMModelDataIRBuilder::getStructType(
            module);
    PointerType *structTypePtr = llvm::PointerType::get(modelDataStructType, 0);
    argTypes.push_back(structTypePtr);

    FunctionType *funcType = FunctionType::get(Type::getVoidTy(context), argTypes,
            false);
    func = Function::Create(funcType, Function::InternalLinkage,
            FunctionName, module);

    // Create a new basic block to start insertion into.
    BasicBlock *basicBlock = BasicBlock::Create(context, "entry", func);
    builder.SetInsertPoint(basicBlock);

    // single argument
    modelData = func->arg_begin();

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

    /// verifyFunction - Check a function for errors, printing messages on stderr.
    /// Return true if the function is corrupt.
    if (verifyFunction(*func, PrintMessageAction))
    {
        poco_error(getLogger(), "Corrupt Generated Function, " +
                string(FunctionName) + string(": ") + to_string(func));

        throw LLVMException("Generated function is corrupt, see stderr", __FUNC__);
    }

    poco_information(getLogger(), string(FunctionName) + string(": ") +
            to_string(func));

    return func;
}

LLVMEvalRateRuleRatesCodeGen::FunctionPtr LLVMEvalRateRuleRatesCodeGen::createFunction()
{
    Function *func = (Function*)codeGen();
    return (FunctionPtr)engine.getPointerToFunction(func);
}

} /* namespace rr */
