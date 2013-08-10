/*
 * SetRateRuleValuesCodeGen.h
 *
 *  Created on: Jul 15, 2013
 *      Author: andy
 */

#ifndef LLVMSETRATERULEVALUESCODEGEN_H_
#define LLVMSETRATERULEVALUESCODEGEN_H_

#include "CodeGenBase.h"

namespace rr
{

class SetRateRuleValuesCodeGen:
        public CodeGenBase<void*>
{
public:
    SetRateRuleValuesCodeGen(const ModelGeneratorContext &mgc);
    virtual ~SetRateRuleValuesCodeGen();

    typedef void (*FunctionPtr)(LLVMModelData*);
};

} /* namespace rr */
#endif /* LLVMSETRATERULEVALUESCODEGEN_H_ */
