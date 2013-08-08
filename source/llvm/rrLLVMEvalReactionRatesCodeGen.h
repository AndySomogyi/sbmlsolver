/*
 * rrLLVMEvalReactionRatesCodeGen.h
 *
 *  Created on: Jul 15, 2013
 *      Author: andy
 */


#ifndef rrLLVMEvalReactionRatesCodeGenH
#define rrLLVMEvalReactionRatesCodeGenH

#include "rrLLVMModelGeneratorContext.h"
#include "rrLLVMCodeGen.h"
#include "rrLLVMCodeGenBase.h"
#include "rrLLVMSymbolForest.h"
#include "rrLLVMASTNodeFactory.h"
#include "rrLLVMModelDataIRBuilder.h"
#include <sbml/Model.h>

namespace rr
{

typedef void (*LLVMEvalReactionRates_FunctionPtr)(LLVMModelData*);

/**
 * evaluate the current model state and store the results in
 * ModelData.reactionRates
 */
class LLVMEvalReactionRatesCodeGen:
    public LLVMCodeGenBase<LLVMEvalReactionRates_FunctionPtr>
{
public:
    LLVMEvalReactionRatesCodeGen(const LLVMModelGeneratorContext &mgc);
    virtual ~LLVMEvalReactionRatesCodeGen();

    llvm::Value *codeGen();

    static const char* FunctionName;
    typedef LLVMEvalReactionRates_FunctionPtr FunctionPtr;

};

} /* namespace rr */
#endif /* rrLLVMEvalReactionRatesCodeGen */
