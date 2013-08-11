/*
 * EventAssignCodeGen.cpp
 *
 *  Created on: Aug 11, 2013
 *      Author: andy
 */

#include "llvm/EventAssignCodeGen.h"

namespace rr
{

using namespace llvm;
using namespace libsbml;

const char* EventAssignCodeGen::FunctionName = "eventAssign";

EventAssignCodeGen::EventAssignCodeGen(const ModelGeneratorContext& mgc) :
        EventCodeGenBase<EventAssignCodeGen>(mgc)
{
}

EventAssignCodeGen::~EventAssignCodeGen()
{
}



} /* namespace rr */
