/*
 * rrLLVMGetRateRuleValuesCodeGen.h
 *
 *  Created on: Jul 15, 2013
 *      Author: andy
 */

#ifndef rrLLVMGetRateRuleValuesCodeGen_H
#define rrLLVMGetRateRuleValuesCodeGen_H

#include <rrLLVMCodeGenBase.h>

namespace rr
{

class LLVMGetRateRuleValuesCodeGen: private rr::LLVMCodeGenBase
{
public:
    LLVMGetRateRuleValuesCodeGen(const LLVMModelGeneratorContext &mgc);
    virtual ~LLVMGetRateRuleValuesCodeGen();
};

} /* namespace rr */
#endif /* rrLLVMGetRateRuleValuesCodeGen_H */
