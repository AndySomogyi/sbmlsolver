/*
 * rrLLVMEvalReactionRatesCodeGen.cpp
 *
 *  Created on: Jul 15, 2013
 *      Author: andy
 */

#include "rrLLVMEvalReactionRatesCodeGen.h"
#include "rrLLVMException.h"
#include "rrLLVMASTNodeCodeGen.h"
#include "rrLogger.h"
#include <sbml/math/ASTNode.h>
#include <sbml/math/FormulaFormatter.h>


using namespace libsbml;
using namespace llvm;
using namespace std;


namespace rr
{

const char* LLVMEvalReactionRatesCodeGen::FunctionName = "evalReactionRates";

LLVMEvalReactionRatesCodeGen::LLVMEvalReactionRatesCodeGen(
        const LLVMModelGeneratorContext &mgc) :
        LLVMCodeGenBase(mgc),
        func(0),
        engine(mgc.getExecutionEngine())
{
}

LLVMEvalReactionRatesCodeGen::~LLVMEvalReactionRatesCodeGen()
{
}

Value* LLVMEvalReactionRatesCodeGen::codeGen()
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
    builder->SetInsertPoint(basicBlock);

    // single argument
    Value *modelData = func->arg_begin();

    Argument &arg = func->getArgumentList().front();

    Value *argVal = &arg;


    LLVMModelDataIRBuilder modelDataBuilder(dataSymbols, builder);


    builder->CreateRetVoid();

    /// verifyFunction - Check a function for errors, printing messages on stderr.
    /// Return true if the function is corrupt.
    if (verifyFunction(*func, PrintMessageAction))
    {
        throw LLVMException("Generated function is corrupt, see stderr", __FUNC__);
    }

    func->dump();


    return func;
}


llvm::Value* LLVMEvalReactionRatesCodeGen::symbolValue(const std::string& symbol)
{
    LLVMSymbolForest::ConstIterator i = modelSymbols.getInitialAssigments().find(symbol);
    if (i != modelSymbols.getInitialAssigments().end())
    {
        return LLVMASTNodeCodeGen(*builder, *this).codeGen(i->second);
    }

    i = modelSymbols.getAssigmentRules().find(symbol);
    if (i != modelSymbols.getAssigmentRules().end())
    {
        return LLVMASTNodeCodeGen(*builder, *this).codeGen(i->second);
    }

    i = modelSymbols.getInitialValues().find(symbol);
    if (i != modelSymbols.getInitialValues().end())
    {
        return LLVMASTNodeCodeGen(*builder, *this).codeGen(i->second);
    }

    else
    {
        string msg = "Could not find requested symbol \'";
        msg += symbol;
        msg += "\' in symbol forest";
        throw LLVMException(msg, __FUNC__);
    }
}


LLVMEvalReactionRatesCodeGen::FunctionPtr LLVMEvalReactionRatesCodeGen::createFunction()
{
    Function *func = (Function*)codeGen();
    return (FunctionPtr)engine->getPointerToFunction(func);
}



} /* namespace rr */
