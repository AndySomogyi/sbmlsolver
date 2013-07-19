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
        modelData(0),
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
    modelData = func->arg_begin();


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



LLVMEvalReactionRatesCodeGen::FunctionPtr LLVMEvalReactionRatesCodeGen::createFunction()
{
    Function *func = (Function*)codeGen();
    return (FunctionPtr)engine->getPointerToFunction(func);
}

llvm::Value* LLVMEvalReactionRatesCodeGen::symbolValue(
        const std::string& symbol)
{
    return LLVMCodeGenBase::symbolValue(symbol, modelData);
}

} /* namespace rr */
