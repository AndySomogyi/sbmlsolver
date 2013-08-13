/*
 * EventAssignCodeGen.cpp
 *
 *  Created on: Aug 11, 2013
 *      Author: andy
 */

#include "EventAssignCodeGen.h"
#include "ModelDataIRBuilder.h"
#include "ModelDataSymbolResolver.h"
#include "ASTNodeCodeGen.h"

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

bool EventAssignCodeGen::eventCodeGen(llvm::Value *modelData,
        uint eventIndx, const libsbml::Event *event)
{
    ModelDataIRBuilder mdBuilder(modelData, dataSymbols, builder);
    ModelDataLoadSymbolResolver mdLoadResolver(modelData, model, modelSymbols,
            dataSymbols, builder);
    ModelDataStoreSymbolResolver mdStoreResolver(modelData, model, modelSymbols,
            dataSymbols, builder, mdLoadResolver);

    const ListOfEventAssignments *assignments = event->getListOfEventAssignments();

    for(uint id = 0; id < assignments->size(); ++id)
    {
        const EventAssignment *a = assignments->get(id);
        Value *value = mdBuilder.createEventAssignmentLoad(eventIndx, id);
        mdStoreResolver.storeSymbolValue(a->getVariable(), value);
    }

    return true;
}


} /* namespace rr */


