/*
 * GetPiecewiseTriggersCodeGen.cpp
 *
 *  Created on: Aug 10, 2013
 *      Author: andy
 */
#pragma hdrstop
#include "GetPiecewiseTriggersCodeGen.h"
#include "LLVMException.h"
#include "ModelDataSymbolResolver.h"
#include "rrLogger.h"

#include <Poco/Logger.h>
#include <llvm/GetPiecewiseTriggersCodeGen.h>
#include <vector>

using namespace llvm;

using namespace libsbml;

namespace rrllvm
{

    llvm::Value* GetPiecewiseTriggersCodeGen::codeGen()
    {
        // make the set init value function
        llvm::Type* argTypes[] = {
            llvm::PointerType::get(ModelDataIRBuilder::getStructType(this->module), 0),
            llvm::Type::getInt32Ty(this->context)
        };

        const char* argNames[] = {
            "modelData", IndexArgName
        };

        llvm::Value* args[] = { 0, 0 };

        llvm::Type* retType = getRetType();

        llvm::BasicBlock* entry = this->codeGenHeader(FunctionName, retType,
            argTypes, argNames, args);

        ModelDataLoadSymbolResolver resolver(args[0], this->modelGenContext);

        ASTNodeCodeGen astCodeGen(this->builder, resolver, this->modelGenContext, args[0]);

        // default, return NaN
        llvm::BasicBlock* def = llvm::BasicBlock::Create(this->context, "default", this->function);
        this->builder.SetInsertPoint(def);

        llvm::Value* defRet = createRet(0);
        this->builder.CreateRet(defRet);

        // write the switch at the func entry point, the switch is also the
        // entry block terminator
        this->builder.SetInsertPoint(entry);

        llvm::SwitchInst* s = this->builder.CreateSwitch(args[1], def, this->piecewiseTriggers->size());

        for (uint i = 0; i < piecewiseTriggers->size(); ++i)
        {
            char block_name[64];
            std::sprintf(block_name, "piecewiseTrigger_%i_block", i);
            llvm::BasicBlock* block = llvm::BasicBlock::Create(this->context, block_name, this->function);
            this->builder.SetInsertPoint(block);
            resolver.flushCache();

            llvm::Value* value = astCodeGen.codeGenBoolean(this->piecewiseTriggers[i]);

            // convert type to return type
            value = createRet(value);

            this->builder.CreateRet(value);
            s->addCase(llvm::ConstantInt::get(llvm::Type::getInt32Ty(this->context), i), block);
        }

        return this->verifyFunction();
    }

    const char* GetPiecewiseTriggersCodeGen::FunctionName = "getPiecewiseTriggers";
    const char* GetPiecewiseTriggersCodeGen::IndexArgName = "triggerIndx";

    llvm::Type* GetPiecewiseTriggersCodeGen::getRetType()
    {
        return llvm::Type::getInt8Ty(context);
    };

    llvm::Value* GetPiecewiseTriggersCodeGen::createRet(llvm::Value* value)
    {
        if (!value)
        {
            return llvm::ConstantInt::get(getRetType(), 0xff, false);
        }
        else
        {
            return builder.CreateIntCast(value, getRetType(), false);
        }
    }


} /* namespace rr */




