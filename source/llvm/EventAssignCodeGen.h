/*
 * EventAssignCodeGen.h
 *
 *  Created on: Aug 11, 2013
 *      Author: andy
 */

#ifndef EVENTASSIGNCODEGEN_H_
#define EVENTASSIGNCODEGEN_H_

#include "EventCodeGenBase.h"

namespace rr
{

class EventAssignCodeGen:
        public EventCodeGenBase<EventAssignCodeGen>
{
public:
    EventAssignCodeGen(const ModelGeneratorContext &mgc);
    ~EventAssignCodeGen();

    static const char* FunctionName;
};

} /* namespace rr */
#endif /* EVENTASSIGNCODEGEN_H_ */
