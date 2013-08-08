/*
 * rrLLVMEvalRateRuleRatesCodeGen.h
 *
 *  Created on: Aug 2, 2013
 *      Author: andy
 */

#ifndef RRLLVMEVALRATERULERATESCODEGEN_H_
#define RRLLVMEVALRATERULERATESCODEGEN_H_

#include "rrLLVMCodeGenBase.h"
#include "rrLLVMModelGeneratorContext.h"
#include "rrLLVMCodeGen.h"
#include "rrLLVMSymbolForest.h"
#include "rrLLVMASTNodeFactory.h"
#include "rrLLVMModelDataIRBuilder.h"
#include <sbml/Model.h>

namespace rr
{

typedef void (*LLVMEvalRateRuleRates_FunctionPtr)(LLVMModelData*);

/**
 * evaluate the current model state and store the results in
 * ModelData.reactionRates
 */
class LLVMEvalRateRuleRatesCodeGen:
        public LLVMCodeGenBase<LLVMEvalRateRuleRates_FunctionPtr>
{
public:
    LLVMEvalRateRuleRatesCodeGen(const LLVMModelGeneratorContext &mgc);
    virtual ~LLVMEvalRateRuleRatesCodeGen();

    llvm::Value *codeGen();

    static const char* FunctionName;
    typedef LLVMEvalRateRuleRates_FunctionPtr FunctionPtr;
};
} /* namespace rr */
#endif /* RRLLVMEVALRATERULERATESCODEGEN_H_ */
