/*
 * EventTriggerCodeGen.cpp
 *
 *  Created on: Aug 11, 2013
 *      Author: andy
 */

#include "llvm/EventTriggerCodeGen.h"

namespace rr
{

using namespace llvm;
using namespace libsbml;

const char* EventTriggerCodeGen::FunctionName = "eventTrigger";

EventTriggerCodeGen::EventTriggerCodeGen(const ModelGeneratorContext& mgc) :
        EventCodeGenBase<EventTriggerCodeGen>(mgc)
{
}

EventTriggerCodeGen::~EventTriggerCodeGen()
{
}


} /* namespace rr */

