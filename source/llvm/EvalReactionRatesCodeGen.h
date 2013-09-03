/*
 * EvalReactionRatesCodeGen.h
 *
 *  Created on: Jul 15, 2013
 *      Author: andy
 */


#ifndef EvalReactionRatesCodeGenH
#define EvalReactionRatesCodeGenH

#include "ModelGeneratorContext.h"
#include "CodeGenBase.h"
#include "SymbolForest.h"
#include "ASTNodeFactory.h"
#include "ModelDataIRBuilder.h"
#include <sbml/Model.h>

namespace rrllvm
{

typedef double (*EvalReactionRates_FunctionPtr)(LLVMModelData*);

/**
 * evaluate the current model state and store the results in
 * ModelData.reactionRates
 */
class EvalReactionRatesCodeGen:
    public CodeGenBase<EvalReactionRates_FunctionPtr>
{
public:
    EvalReactionRatesCodeGen(const ModelGeneratorContext &mgc);
    virtual ~EvalReactionRatesCodeGen();

    llvm::Value *codeGen();

    static const char* FunctionName;
    typedef EvalReactionRates_FunctionPtr FunctionPtr;

};

} /* namespace rr */
#endif /* rrLLVMEvalReactionRatesCodeGen */
