/*
 * GetRateRuleValuesCodeGen.h
 *
 *  Created on: Jul 15, 2013
 *      Author: andy
 */

#ifndef GetRateRuleValuesCodeGen_H
#define GetRateRuleValuesCodeGen_H

#include "CodeGenBase.h"

namespace rrllvm
{

class GetRateRuleValuesCodeGen:
        private CodeGenBase<void*>
{
public:
    GetRateRuleValuesCodeGen(const ModelGeneratorContext &mgc);
    virtual ~GetRateRuleValuesCodeGen();

    typedef void (*FunctionPtr)(LLVMModelData*);
};

} /* namespace rr */
#endif /* GetRateRuleValuesCodeGen_H */
