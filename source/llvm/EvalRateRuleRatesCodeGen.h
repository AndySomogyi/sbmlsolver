/*
 * EvalRateRuleRatesCodeGen.h
 *
 *  Created on: Aug 2, 2013
 *      Author: andy
 */

#ifndef RRLLVMEVALRATERULERATESCODEGEN_H_
#define RRLLVMEVALRATERULERATESCODEGEN_H_

#include "CodeGenBase.h"
#include "ModelGeneratorContext.h"
#include "SymbolForest.h"
#include "ASTNodeFactory.h"
#include "ModelDataIRBuilder.h"
#include <sbml/Model.h>

namespace rrllvm
{

typedef void (*EvalRateRuleRates_FunctionPtr)(LLVMModelData*);

/**
 * evaluate the current model state and store the results in
 * ModelData.reactionRates
 */
class EvalRateRuleRatesCodeGen:
        public CodeGenBase<EvalRateRuleRates_FunctionPtr>
{
public:
    EvalRateRuleRatesCodeGen(const ModelGeneratorContext &mgc);
    virtual ~EvalRateRuleRatesCodeGen();

    llvm::Value *codeGen();

    static const char* FunctionName;
    typedef EvalRateRuleRates_FunctionPtr FunctionPtr;
};
} /* namespace rr */
#endif /* RRLLVMEVALRATERULERATESCODEGEN_H_ */
