/*
 * rrLLVMGetRateRuleValuesCodeGen.h
 *
 *  Created on: Jul 15, 2013
 *      Author: andy
 */

#ifndef rrLLVMGetRateRuleValuesCodeGen_H
#define rrLLVMGetRateRuleValuesCodeGen_H

#include "rrLLVMCodeGenBase.h"

namespace rr
{

class LLVMGetRateRuleValuesCodeGen:
        private rr::LLVMCodeGenBase<void*>
{
public:
    LLVMGetRateRuleValuesCodeGen(const LLVMModelGeneratorContext &mgc);
    virtual ~LLVMGetRateRuleValuesCodeGen();

    typedef void (*FunctionPtr)(LLVMModelData*);
};

} /* namespace rr */
#endif /* rrLLVMGetRateRuleValuesCodeGen_H */
