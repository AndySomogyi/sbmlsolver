/*
 * EventTriggerCodeGen.cpp
 *
 *  Created on: Aug 11, 2013
 *      Author: andy
 */

#include "EventTriggerCodeGen.h"
#include "ModelDataIRBuilder.h"
#include "ModelDataSymbolResolver.h"
#include "ASTNodeCodeGen.h"

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

bool EventTriggerCodeGen::eventCodeGen(llvm::Value *modelData, uint eventIndx,
        const libsbml::Event *event)
{
    ModelDataIRBuilder mdBuilder(modelData, dataSymbols, builder);
    ModelDataLoadSymbolResolver mdLoadResolver(modelData, model, modelSymbols,
            dataSymbols, builder);
    ModelDataStoreSymbolResolver mdStoreResolver(modelData, model, modelSymbols,
            dataSymbols, builder, mdLoadResolver);
    ASTNodeCodeGen astCodeGen(builder, mdLoadResolver);

    const ListOfEventAssignments *assignments =
            event->getListOfEventAssignments();

    for (uint id = 0; id < assignments->size(); ++id)
    {
        const EventAssignment *a = assignments->get(id);
        const ASTNode *math = a->getMath();
        Value *value = astCodeGen.codeGen(math);
        mdBuilder.createEventAssignmentStore(eventIndx, id, value);
    }

    return true;
}

} /* namespace rr */

