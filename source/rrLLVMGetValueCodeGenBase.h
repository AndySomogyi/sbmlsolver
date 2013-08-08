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

typedef double (*LLVMGetValueCodeGenBase_FunctionPtr)(LLVMModelData*, int32_t);

template <typename Derived, bool substanceUnits>
class LLVMGetValueCodeGenBase :
        public LLVMCodeGenBase<LLVMGetValueCodeGenBase_FunctionPtr>
{
public:
    LLVMGetValueCodeGenBase(const LLVMModelGeneratorContext &mgc);
    virtual ~LLVMGetValueCodeGenBase();

    llvm::Value *codeGen();

    typedef LLVMGetValueCodeGenBase_FunctionPtr FunctionPtr;

private:
    typedef LLVMCodeGenBase<LLVMGetValueCodeGenBase_FunctionPtr> _base;

};

template <typename Derived, bool substanceUnits>
LLVMGetValueCodeGenBase<Derived, substanceUnits>::LLVMGetValueCodeGenBase(
        const LLVMModelGeneratorContext &mgc) :
        _base(mgc)
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
    llvm::Type *argTypes[] = {
        llvm::PointerType::get(LLVMModelDataIRBuilder::getStructType(_base::module), 0),
        llvm::Type::getInt32Ty(_base::context)
    };

    const char *argNames[] = {
        "modelData", Derived::IndexArgName
    };

    llvm::Value *args[] = {0, 0};

    llvm::BasicBlock *entry = _base::codeGenHeader(Derived::FunctionName, llvm::Type::getDoubleTy(_base::context),
            argTypes, argNames, args);

    vector<string> ids = static_cast<Derived*>(this)->getIds();

    LLVMModelDataLoadSymbolResolver resolver(args[0], _base::model, _base::modelSymbols,
            _base::dataSymbols, _base::builder);

    // default, return NaN
    llvm::BasicBlock *def = llvm::BasicBlock::Create(_base::context, "default", _base::function);
    _base::builder.SetInsertPoint(def);
    _base::builder.CreateRet(llvm::ConstantFP::get(_base::context, llvm::APFloat(123.456)));

    // write the switch at the func entry point, the switch is also the
    // entry block terminator
    _base::builder.SetInsertPoint(entry);

    llvm::SwitchInst *s = _base::builder.CreateSwitch(args[1], def, ids.size());

    for (int i = 0; i < ids.size(); ++i)
    {
        llvm::BasicBlock *block = llvm::BasicBlock::Create(_base::context, ids[i] + "_block", _base::function);
        _base::builder.SetInsertPoint(block);

        // the requested value
        llvm::Value *value = resolver.loadSymbolValue(ids[i]);

        // need to check if we have an amount or concentration and check if we
        // are asked for asked for an amount or concentration and convert accordingly
        const libsbml::Species *species = dynamic_cast<const libsbml::Species*>(
                const_cast<libsbml::Model*>(_base::model)->getElementBySId(ids[i]));

        if(species)
        {
            if (species->getHasOnlySubstanceUnits())
            {
                value->setName(ids[i] + "_amt");
                // species is treated as an amount
                if (!substanceUnits)
                {
                    // convert to concentration
                    llvm::Value *comp = resolver.loadSymbolValue(species->getCompartment());
                    value = _base::builder.CreateFDiv(value, comp, ids[i] + "_conc");
                }
            }
            else
            {
                value->setName(ids[i] + "_conc");
                // species is treated as a concentration
                if (substanceUnits)
                {
                    // convert to amount
                    llvm::Value *comp = resolver.loadSymbolValue(species->getCompartment());
                    value = _base::builder.CreateFMul(value, comp, ids[i] + "_amt");
                }
            }
        }
        else
        {
            value->setName(ids[i] + "_value");
        }
        _base::builder.CreateRet(value);
        s->addCase(llvm::ConstantInt::get(llvm::Type::getInt32Ty(_base::context), i), block);
    }

    return _base::verifyFunction();
}


} /* namespace rr */




#endif /* RRLLVMGETVALUECODEGENBASE_H_ */
