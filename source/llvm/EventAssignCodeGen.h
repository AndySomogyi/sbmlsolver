/*
 * EventAssignCodeGen.h
 *
 *  Created on: Aug 11, 2013
 *      Author: andy
 */

#ifndef EVENTASSIGNCODEGEN_H_
#define EVENTASSIGNCODEGEN_H_

#include "EventCodeGenBase.h"

namespace rrllvm
{

class EventAssignCodeGen: public EventCodeGenBase<EventAssignCodeGen>
{
public:
    EventAssignCodeGen(const ModelGeneratorContext &mgc);
    ~EventAssignCodeGen();

    bool eventCodeGen(llvm::Value *modelData, llvm::Value *data,
            const libsbml::Event* event);

    static const char* FunctionName;
};

} /* namespace rr */
#endif /* EVENTASSIGNCODEGEN_H_ */
