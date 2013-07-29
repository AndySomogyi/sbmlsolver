/*
 * rrLLVMGetBoundarySpeciesAmountsCodeGen.cpp
 *
 *  Created on: Jul 27, 2013
 *      Author: andy
 */

#include "rrLLVMGetBoundarySpeciesAmountCodeGen.h"
#include "rrLLVMException.h"
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
    builder.SetInsertPoint(entry);

    vector<Argument*> args;
    for (Function::arg_iterator ai = functionValue->arg_begin();
            ai != functionValue->arg_end(); ++ai)
    {
        args.push_back(ai);
    }
    assert(args.size() == 2);

    BasicBlock *def = BasicBlock::Create(context, "default", functionValue);
    builder.SetInsertPoint(def);
    builder.CreateRet(ConstantFP::get(context, APFloat(13.0)));

    builder.SetInsertPoint(entry);

    llvm::SwitchInst *s = builder.CreateSwitch(args[1], def, 2);

    cout << "num cases: " << s->getNumCases() << endl;

    BasicBlock *one = BasicBlock::Create(context, "one", functionValue);
    builder.SetInsertPoint(one);
    builder.CreateRet(ConstantFP::get(context, APFloat(1.0)));

    BasicBlock *two = BasicBlock::Create(context, "two", functionValue);
        builder.SetInsertPoint(two);
        builder.CreateRet(ConstantFP::get(context, APFloat(2.0)));

    s->addCase(ConstantInt::get(Type::getInt32Ty(context), 1), one);
    s->addCase(ConstantInt::get(Type::getInt32Ty(context), 2), two);








    //builder.SetInsertPoint(entry);

    //Value *ret = ConstantFP::get(context, APFloat(42.0));

    //builder.CreateRet(ret);

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

    poco_information(getLogger(), string(FunctionName) + string(": ") +
            to_string(functionValue));

    return functionValue;
}


LLVMGetBoundarySpeciesAmountCodeGen::FunctionPtr
    LLVMGetBoundarySpeciesAmountCodeGen::createFunction()
{
    Function *func = (Function*)codeGen();
    return (FunctionPtr)engine.getPointerToFunction(func);
}


} /* namespace rr */

