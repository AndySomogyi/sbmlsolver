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


/**
 * evaluate the current model state and store the results in
 * ModelData.reactionRates
 */
class LLVMEvalRateRuleRatesCodeGen: private LLVMCodeGenBase
{
public:
    LLVMEvalRateRuleRatesCodeGen(const LLVMModelGeneratorContext &mgc);
    virtual ~LLVMEvalRateRuleRatesCodeGen();

    llvm::Value *codeGen();

    static const char* FunctionName;
    typedef void (*FunctionPtr)(ModelData*);

    FunctionPtr createFunction();

private:

    llvm::Function *func;

    /**
     * pointer to the model data param whilst we are code generating.
     */
    llvm::Value *modelData;
};
} /* namespace rr */
#endif /* RRLLVMEVALRATERULERATESCODEGEN_H_ */
