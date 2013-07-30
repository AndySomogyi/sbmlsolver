/*
 * rrLLVMGetValueCodeGenBase.h
 *
 *  Created on: Jul 29, 2013
 *      Author: andy
 */

#ifndef RRLLVMGETVALUECODEGENBASE_H_
#define RRLLVMGETVALUECODEGENBASE_H_

#include "rrLLVMCodeGenBase.h"
#include "rrLLVMModelGeneratorContext.h"
#include "rrLLVMCodeGen.h"
#include "rrLLVMCodeGenBase.h"
#include "rrLLVMSymbolForest.h"
#include "rrLLVMASTNodeFactory.h"
#include "rrLLVMModelDataIRBuilder.h"
#include "rrLLVMModelDataSymbolResolver.h"
#include "rrLLVMException.h"
#include "rrLogger.h"
#include <sbml/Model.h>
#include <Poco/Logger.h>
#include <vector>

namespace rr
{

template <typename Derived, bool substanceUnits>
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

template <typename Derived, bool substanceUnits>
LLVMGetValueCodeGenBase<Derived, substanceUnits>::LLVMGetValueCodeGenBase(
        const LLVMModelGeneratorContext &mgc) :
        LLVMCodeGenBase(mgc),
        functionValue(0)
{
}

template <typename Derived, bool substanceUnits>
LLVMGetValueCodeGenBase<Derived, substanceUnits>::~LLVMGetValueCodeGenBase()
{
}

template <typename Derived, bool substanceUnits>
llvm::Value* LLVMGetValueCodeGenBase<Derived, substanceUnits>::codeGen()
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

    std::vector<llvm::Argument*> args;
    for (llvm::Function::arg_iterator ai = functionValue->arg_begin();
            ai != functionValue->arg_end(); ++ai)
    {
        args.push_back(ai);
    }
    assert(args.size() == 2);

    args[0]->setName("modelData");
    args[1]->setName(Derived::IndexArgName);

    vector<string> ids = static_cast<Derived*>(this)->getIds();

    LLVMModelDataSymbolResolver resolver(args[0], model, modelSymbols,
            dataSymbols, builder);

    // default, return NaN
    llvm::BasicBlock *def = llvm::BasicBlock::Create(context, "default", functionValue);
    builder.SetInsertPoint(def);
    //builder.CreateRet(llvm::ConstantFP::get(context, llvm::APFloat::getQNaN(llvm::APFloat::IEEEdouble, false)));
    builder.CreateRet(llvm::ConstantFP::get(context, llvm::APFloat(123.456)));

    // write the switch at the func entry point, the switch is also the
    // entry block terminator
    builder.SetInsertPoint(entry);

    llvm::SwitchInst *s = builder.CreateSwitch(args[1], def, ids.size());

    for (int i = 0; i < ids.size(); ++i)
    {
        llvm::BasicBlock *block = llvm::BasicBlock::Create(context, ids[i] + "_block", functionValue);
        builder.SetInsertPoint(block);

        // the requested value
        llvm::Value *value = resolver.symbolValue(ids[i]);

        // need to check if we have an amount or concentration and check if we
        // are asked for asked for an amount or concentration and convert accordingly
        const libsbml::Species *species = dynamic_cast<const libsbml::Species*>(
                const_cast<libsbml::Model*>(model)->getElementBySId(ids[i]));

        if(species)
        {
            if (species->getHasOnlySubstanceUnits())
            {
                value->setName(ids[i] + "_amt");
                // species is treated as an amount
                if (!substanceUnits)
                {
                    // convert to concentration
                    llvm::Value *comp = resolver.symbolValue(species->getCompartment());
                    value = builder.CreateFDiv(value, comp, ids[i] + "_conc");
                }
            }
            else
            {
                value->setName(ids[i] + "_conc");
                // species is treated as a concentration
                if (substanceUnits)
                {
                    // convert to amount
                    llvm::Value *comp = resolver.symbolValue(species->getCompartment());
                    value = builder.CreateFMul(value, comp, ids[i] + "_amt");
                }
            }
        }
        else
        {
            value->setName(ids[i] + "_value");
        }
        builder.CreateRet(value);
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

template <typename Derived, bool substanceUnits>
typename LLVMGetValueCodeGenBase<Derived, substanceUnits>::FunctionPtr
    LLVMGetValueCodeGenBase<Derived, substanceUnits>::createFunction()
{
    llvm::Function *func = (llvm::Function*)codeGen();
    return (FunctionPtr)engine.getPointerToFunction(func);
}

} /* namespace rr */




#endif /* RRLLVMGETVALUECODEGENBASE_H_ */
