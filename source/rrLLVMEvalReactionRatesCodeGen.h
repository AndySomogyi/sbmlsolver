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
#include <sbml/SBMLVisitor.h>

namespace rr
{
using libsbml::Model;
using libsbml::Compartment;
using libsbml::SBMLVisitor;
using libsbml::Species;
using libsbml::Parameter;

/**
 * evaluate the current model state and store the results in
 * ModelData.reactionRates
 */
class LLVMEvalReactionRatesCodeGen: private LLVMCodeGenBase
{
public:
    LLVMEvalReactionRatesCodeGen(const LLVMModelGeneratorContext &mgc);
    virtual ~LLVMEvalReactionRatesCodeGen();

    llvm::Value *codeGen();

    static const char* FunctionName;
    typedef void (*FunctionPtr)(ModelData*);

    FunctionPtr createFunction();

    virtual llvm::Value *symbolValue(const std::string& symbol);

private:

    llvm::Function *func;

    llvm::ExecutionEngine *engine;

    /**
     * pointer to the model data param whilst we are code generating.
     */
    llvm::Value *modelData;
};

} /* namespace rr */
#endif /* rrLLVMEvalReactionRatesCodeGen */
