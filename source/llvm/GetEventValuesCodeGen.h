/*
 * GetEventValuesCodeGenBas.h
 *
 *  Created on: Aug 10, 2013
 *      Author: andy
 */

#ifndef RRLLVMGETEVENTVALUECODEGENBASE_H_
#define RRLLVMGETEVENTVALUECODEGENBASE_H_

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

namespace rr
{

typedef double (*GetEventValueCodeGenBase_FunctionPtr)(LLVMModelData*, int32_t);

template <typename Derived>
class GetEventValueCodeGenBase :
        public CodeGenBase<GetEventValueCodeGenBase_FunctionPtr>
{
public:
    GetEventValueCodeGenBase(const ModelGeneratorContext &mgc);
    virtual ~GetEventValueCodeGenBase();

    llvm::Value *codeGen();

    typedef GetEventValueCodeGenBase_FunctionPtr FunctionPtr;

};

template <typename Derived>
GetEventValueCodeGenBase<Derived>::GetEventValueCodeGenBase(
        const ModelGeneratorContext &mgc) :
        CodeGenBase<GetEventValueCodeGenBase_FunctionPtr>(mgc)
{

}

template <typename Derived>
GetEventValueCodeGenBase<Derived>::~GetEventValueCodeGenBase()
{
}

template <typename Derived>
llvm::Value* GetEventValueCodeGenBase<Derived>::codeGen()
{
    // make the set init value function
    llvm::Type *argTypes[] = {
        llvm::PointerType::get(ModelDataIRBuilder::getStructType(this->module), 0),
        llvm::Type::getInt32Ty(this->context)
    };

    const char *argNames[] = {
        "modelData", Derived::IndexArgName
    };

    llvm::Value *args[] = {0, 0};

    llvm::BasicBlock *entry = this->codeGenHeader(Derived::FunctionName, llvm::Type::getDoubleTy(this->context),
            argTypes, argNames, args);

    const libsbml::ListOfEvents *events = this->model->getListOfEvents();

    ModelDataLoadSymbolResolver resolver(args[0], this->model, this->modelSymbols,
            this->dataSymbols, this->builder);

    ASTNodeCodeGen astCodeGen(this->builder, resolver);

    // default, return NaN
    llvm::BasicBlock *def = llvm::BasicBlock::Create(this->context, "default", this->function);
    this->builder.SetInsertPoint(def);
    this->builder.CreateRet(llvm::ConstantFP::get(this->context, llvm::APFloat(123.456)));

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

        const libsbml::ASTNode *math = static_cast<Derived*>(this)->getMath(event);

        // the requested value
        llvm::Value *value = astCodeGen.codeGen(math);

        this->builder.CreateRet(value);
        s->addCase(llvm::ConstantInt::get(llvm::Type::getInt32Ty(this->context), i), block);
    }

    return this->verifyFunction();
}


class GetEventTriggerCodeGen: public
    GetEventValueCodeGenBase<GetEventTriggerCodeGen>
{
public:
    GetEventTriggerCodeGen(const ModelGeneratorContext &mgc);
    ~GetEventTriggerCodeGen() {};

    const libsbml::ASTNode *getMath(const libsbml::Event *);

    static const char* FunctionName;
    static const char* IndexArgName;
};

class GetEventPriorityCodeGen: public
    GetEventValueCodeGenBase<GetEventPriorityCodeGen>
{
public:
    GetEventPriorityCodeGen(const ModelGeneratorContext &mgc);
    ~GetEventPriorityCodeGen();

    const libsbml::ASTNode *getMath(const libsbml::Event *);

    static const char* FunctionName;
    static const char* IndexArgName;

private:
    libsbml::ASTNode *node;
};

class GetEventDelayCodeGen: public
    GetEventValueCodeGenBase<GetEventDelayCodeGen>
{
public:
    GetEventDelayCodeGen(const ModelGeneratorContext &mgc);
    ~GetEventDelayCodeGen();

    const libsbml::ASTNode *getMath(const libsbml::Event *);

    static const char* FunctionName;
    static const char* IndexArgName;
private:
    libsbml::ASTNode *node;
};


} /* namespace rr */




#endif /* RRLLVMGETVALUECODEGENBASE_H_ */
