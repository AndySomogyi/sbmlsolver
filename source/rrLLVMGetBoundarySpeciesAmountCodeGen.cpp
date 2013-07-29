/*
 * rrLLVMGetBoundarySpeciesAmountsCodeGen.cpp
 *
 *  Created on: Jul 27, 2013
 *      Author: andy
 */

#include "rrLLVMGetBoundarySpeciesAmountCodeGen.h"
#include "rrLLVMException.h"
#include "rrLLVMModelDataSymbolResolver.h"
#include "rrLLVMGetValueCodeGenBase.h"
#include "rrLogger.h"

#include <Poco/Logger.h>
#include <vector>

using namespace llvm;
using namespace std;

namespace rr
{

const char* LLVMGetBoundarySpeciesAmountCodeGen::FunctionName = "getBoundarySpecies";

LLVMGetBoundarySpeciesAmountCodeGen::LLVMGetBoundarySpeciesAmountCodeGen(
        const LLVMModelGeneratorContext &mgc) :
        LLVMCodeGenBase(mgc),
        functionValue(0)
{
}

LLVMGetBoundarySpeciesAmountCodeGen::~LLVMGetBoundarySpeciesAmountCodeGen()
{
}

Value* LLVMGetBoundarySpeciesAmountCodeGen::codeGen()
{
    // make the set init value function
    Type *argTypes[] =
    {
        PointerType::get(LLVMModelDataIRBuilder::getStructType(module), 0),
        Type::getInt32Ty(context)
    };

    FunctionType *funcType = FunctionType::get(Type::getDoubleTy(context),
            argTypes, false);
    functionValue = Function::Create(funcType, Function::InternalLinkage,
            FunctionName, module);

    // Create a new basic block to start insertion into.
    BasicBlock *entry = BasicBlock::Create(context, "entry", functionValue);

    // TODO: remove this
    builder.SetInsertPoint(entry);

    vector<Argument*> args;
    for (Function::arg_iterator ai = functionValue->arg_begin();
            ai != functionValue->arg_end(); ++ai)
    {
        args.push_back(ai);
    }
    assert(args.size() == 2);

    args[0]->setName("modelData");
    args[1]->setName("boundarySpeciesIndx");


    vector<string> ids = dataSymbols.getBoundarySpeciesIds();

    LLVMModelDataSymbolResolver resolver(args[0], model, modelSymbols,
            dataSymbols, builder);

    // default, return NaN
    BasicBlock *def = BasicBlock::Create(context, "default", functionValue);
    builder.SetInsertPoint(def);
    builder.CreateRet(ConstantFP::get(context, APFloat::getQNaN(APFloat::IEEEdouble, false)));

    // write the switch at the func entry point, the switch is also the
    // entry block terminator
    builder.SetInsertPoint(entry);

    llvm::SwitchInst *s = builder.CreateSwitch(args[1], def, ids.size());

    for (int i = 0; i < ids.size(); ++i)
    {
        BasicBlock *block = BasicBlock::Create(context, ids[i] + "_block", functionValue);
        builder.SetInsertPoint(block);
        Value *retval = resolver.symbolValue(ids[i]);
        retval->setName(ids[i] + "_value");
        builder.CreateRet(retval);
        s->addCase(ConstantInt::get(Type::getInt32Ty(context), i), block);
    }

    poco_information(getLogger(), string(FunctionName) + string(": ") +
            to_string(functionValue));

    /// verifyFunction - Check a function for errors, printing messages on stderr.
    /// Return true if the function is corrupt.
    if (verifyFunction(*functionValue, PrintMessageAction))
    {
        poco_error(getLogger(), "Corrupt Generated Function, " +
                string(FunctionName) + string(": ") + to_string(functionValue));

        throw LLVMException("Generated function is corrupt, see stderr", __FUNC__);
    }

    return functionValue;
}


LLVMGetBoundarySpeciesAmountCodeGen::FunctionPtr
    LLVMGetBoundarySpeciesAmountCodeGen::createFunction()
{
    Function *func = (Function*)codeGen();
    return (FunctionPtr)engine.getPointerToFunction(func);
}


} /* namespace rr */

