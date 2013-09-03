/*
 * GetEventValuesCodeGen.cpp
 *
 *  Created on: Aug 10, 2013
 *      Author: andy
 */

#include "GetEventValuesCodeGen.h"
#include "LLVMException.h"
#include "ModelDataSymbolResolver.h"
#include "rrLogger.h"

#include <Poco/Logger.h>
#include <vector>

using namespace llvm;
using namespace std;
using namespace libsbml;

namespace rrllvm
{

const char* GetEventTriggerCodeGen::FunctionName = "getEventTrigger";
const char* GetEventTriggerCodeGen::IndexArgName = "triggerIndx";

GetEventTriggerCodeGen::GetEventTriggerCodeGen(
        const ModelGeneratorContext &mgc) :
        GetEventValueCodeGenBase<GetEventTriggerCodeGen,
        GetEventTriggerCodeGen_FunctionPtr>(mgc)
{
}

llvm::Type *GetEventTriggerCodeGen::getRetType()
{
    return llvm::Type::getInt8Ty(context);
};

llvm::Value *GetEventTriggerCodeGen::createRet(llvm::Value *value)
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

const libsbml::ASTNode* GetEventTriggerCodeGen::getMath(
        const libsbml::Event* event)
{
    const Trigger *trigger = event->getTrigger();
    return trigger->getMath();
}


const char* GetEventPriorityCodeGen::FunctionName = "getEventPriority";
const char* GetEventPriorityCodeGen::IndexArgName = "priorityIndx";

GetEventPriorityCodeGen::GetEventPriorityCodeGen(
        const ModelGeneratorContext &mgc) :
        GetEventValueCodeGenBase<GetEventPriorityCodeGen>(mgc),
        node(0)
{
}

GetEventPriorityCodeGen::~GetEventPriorityCodeGen()
{
    delete node;
}

const libsbml::ASTNode* GetEventPriorityCodeGen::getMath(
        const libsbml::Event* event)
{
    if (event->isSetPriority())
    {
        return event->getPriority()->getMath();
    }
    else
    {
        if (!node)
        {
            node = new ASTNode(AST_REAL);
            node->setValue(0);
        }
        return node;
    }
}

const char* GetEventDelayCodeGen::FunctionName = "getEventDelay";
const char* GetEventDelayCodeGen::IndexArgName = "delayIndx";

GetEventDelayCodeGen::GetEventDelayCodeGen(
        const ModelGeneratorContext &mgc) :
        GetEventValueCodeGenBase<GetEventDelayCodeGen>(mgc),
        node(0)
{
}

GetEventDelayCodeGen::~GetEventDelayCodeGen()
{
    delete node;
}

const libsbml::ASTNode* GetEventDelayCodeGen::getMath(
        const libsbml::Event* event)
{
    if (event->isSetDelay())
    {
        const Delay *delay = event->getDelay();
        return delay->getMath();
    }
    else
    {
        if (!node)
        {
            node = new ASTNode(AST_REAL);
            node->setValue(0);
        }
        return node;
    }
}



} /* namespace rr */




