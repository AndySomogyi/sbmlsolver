/*
 * LLVMSetRateRuleValuesCodeGen.h
 *
 *  Created on: Jul 15, 2013
 *      Author: andy
 */

#ifndef LLVMSETRATERULEVALUESCODEGEN_H_
#define LLVMSETRATERULEVALUESCODEGEN_H_

#include "rrLLVMCodeGenBase.h"

namespace rr
{

class LLVMSetRateRuleValuesCodeGen:
        public LLVMCodeGenBase<void*>
{
public:
    LLVMSetRateRuleValuesCodeGen(const LLVMModelGeneratorContext &mgc);
    virtual ~LLVMSetRateRuleValuesCodeGen();

    typedef void (*FunctionPtr)(LLVMModelData*);
};

} /* namespace rr */
#endif /* LLVMSETRATERULEVALUESCODEGEN_H_ */
