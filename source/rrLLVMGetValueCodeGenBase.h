/*
 * rrLLVMGetValueCodeGenBase.h
 *
 *  Created on: Jul 29, 2013
 *      Author: andy
 */

#ifndef RRLLVMGETVALUECODEGENBASE_H_
#define RRLLVMGETVALUECODEGENBASE_H_

#include <rrLLVMCodeGenBase.h>
#include "rrLLVMModelGeneratorContext.h"
#include "rrLLVMCodeGen.h"
#include "rrLLVMCodeGenBase.h"
#include "rrLLVMSymbolForest.h"
#include "rrLLVMASTNodeFactory.h"
#include "rrLLVMModelDataIRBuilder.h"
#include <sbml/Model.h>
#include <Poco/Logger.h>
#include <vector>

namespace rr
{

template <typename Derived, bool isConcentration>
class LLVMGetValueCodeGenBase : public rr::LLVMCodeGenBase
{
public:
    LLVMGetValueCodeGenBase(const LLVMModelGeneratorContext &mgc);
    ~LLVMGetValueCodeGenBase();

    llvm::Value *codeGen();

    typedef double (*FunctionPtr)(ModelData*, int32_t);

    FunctionPtr createFunction();

protected:
    llvm::Function *functionValue;
};

template <typename Derived, bool isConcentration>
llvm::Value* LLVMGetValueCodeGenBase<Derived, isConcentration>::codeGen()
{
    // make the set init value function
    llvm::Type *argTypes[] =
    {
        llvm::PointerType::get(LLVMModelDataIRBuilder::getStructType(module), 0),
        llvm::Type::getInt32Ty(context)
    };

    llvm::FunctionType *funcType = llvm::FunctionType::get(llvm::Type::getDoubleTy(context),
            argTypes, false);
    functionValue = llvm::Function::Create(funcType, llvm::Function::InternalLinkage,
            Derived::FunctionName, module);

    // Create a new basic block to start insertion into.
    llvm::BasicBlock *entry = llvm::BasicBlock::Create(context, "entry", functionValue);

    // TODO: remove this
    builder.SetInsertPoint(entry);

    std::vector<llvm::Argument*> args;
    for (llvm::Function::arg_iterator ai = functionValue->arg_begin();
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
    llvm::BasicBlock *def = llvm::BasicBlock::Create(context, "default", functionValue);
    builder.SetInsertPoint(def);
    builder.CreateRet(llvm::ConstantFP::get(context, llvm::APFloat::getQNaN(llvm::APFloat::IEEEdouble, false)));

    // write the switch at the func entry point, the switch is also the
    // entry block terminator
    builder.SetInsertPoint(entry);

    llvm::SwitchInst *s = builder.CreateSwitch(args[1], def, ids.size());

    for (int i = 0; i < ids.size(); ++i)
    {
        llvm::BasicBlock *block = llvm::BasicBlock::Create(context, ids[i] + "_block", functionValue);
        builder.SetInsertPoint(block);
        llvm::Value *retval = resolver.symbolValue(ids[i]);
        retval->setName(ids[i] + "_value");
        builder.CreateRet(retval);
        s->addCase(llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), i), block);
    }

    poco_information(getLogger(), string(Derived::FunctionName) + string(": ") +
            to_string(functionValue));

    /// verifyFunction - Check a function for errors, printing messages on stderr.
    /// Return true if the function is corrupt.
    if (llvm::verifyFunction(*functionValue, llvm::PrintMessageAction))
    {
        poco_error(getLogger(), "Corrupt Generated Function, " +
                string(Derived::FunctionName) + string(": ") + to_string(functionValue));

        throw LLVMException("Generated function is corrupt, see stderr", __FUNC__);
    }

    return functionValue;
}

template <typename Derived, bool isConcentration>
typename LLVMGetValueCodeGenBase<Derived, isConcentration>::FunctionPtr
    LLVMGetValueCodeGenBase<Derived, isConcentration>::createFunction()
{
    llvm::Function *func = (llvm::Function*)codeGen();
    return (FunctionPtr)engine.getPointerToFunction(func);
}

} /* namespace rr */




#endif /* RRLLVMGETVALUECODEGENBASE_H_ */
