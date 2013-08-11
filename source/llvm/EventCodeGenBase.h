/*
 * EventCodeGenBase.h
 *
 *  Created on: Aug 11, 2013
 *      Author: andy
 */

#ifndef EVENTCODEGENBASE_H_
#define EVENTCODEGENBASE_H_

#include "CodeGenBase.h"
#include "ModelGeneratorContext.h"
#include "SymbolForest.h"
#include "ASTNodeCodeGen.h"
#include "ASTNodeFactory.h"
#include "ModelDataIRBuilder.h"
#include "ModelDataSymbolResolver.h"
#include "LLVMException.h"
#include "rrLogger.h"
#include <sbml/Model.h>
#include <Poco/Logger.h>
#include <vector>
#include <cstdio>

namespace rr {


typedef void (*EventCodeGenBase_FunctionPtr)(LLVMModelData*, int32_t);

template <typename Derived>
class EventCodeGenBase :
        public CodeGenBase<EventCodeGenBase_FunctionPtr>
{
public:
    EventCodeGenBase(const ModelGeneratorContext &mgc) :
            CodeGenBase<EventCodeGenBase_FunctionPtr>(mgc)
    {
    };

    virtual ~EventCodeGenBase() {};

    llvm::Value *codeGen();

    typedef EventCodeGenBase_FunctionPtr FunctionPtr;
};

template <typename Derived>
llvm::Value *EventCodeGenBase<Derived>::codeGen()
{
    // make the set init value function
    llvm::Type *argTypes[] = {
        llvm::PointerType::get(ModelDataIRBuilder::getStructType(this->module), 0),
        llvm::Type::getInt32Ty(this->context)
    };

    const char *argNames[] = {
        "modelData", "eventIndx"
    };

    llvm::Value *args[] = {0, 0};

    llvm::Type *retType = llvm::Type::getVoidTy(context);

    llvm::BasicBlock *entry = this->codeGenHeader(Derived::FunctionName, retType,
            argTypes, argNames, args);

    const libsbml::ListOfEvents *events = this->model->getListOfEvents();

    ModelDataLoadSymbolResolver resolver(args[0], this->model, this->modelSymbols,
            this->dataSymbols, this->builder);

    ASTNodeCodeGen astCodeGen(this->builder, resolver);

    // default, return NaN
    llvm::BasicBlock *def = llvm::BasicBlock::Create(this->context, "default", this->function);
    this->builder.SetInsertPoint(def);

    this->builder.CreateRetVoid();

    // write the switch at the func entry point, the switch is also the
    // entry block terminator
    this->builder.SetInsertPoint(entry);

    llvm::SwitchInst *s = this->builder.CreateSwitch(args[1], def, events->size());

    for (uint i = 0; i < events->size(); ++i)
    {
        char block_name[64];
        std::sprintf(block_name, "event_%i_block", i);
        llvm::BasicBlock *block = llvm::BasicBlock::Create(this->context, block_name, this->function);
        this->builder.SetInsertPoint(block);

        const libsbml::Event *event = events->get(i);


        this->builder.CreateRetVoid();
        s->addCase(llvm::ConstantInt::get(llvm::Type::getInt32Ty(this->context), i), block);
    }

    return this->verifyFunction();
}

}





#endif /* EVENTCODEGENBASE_H_ */
