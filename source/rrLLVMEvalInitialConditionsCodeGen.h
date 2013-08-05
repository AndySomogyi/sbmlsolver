/*
 * rrLLVMInitialValueCodeGen.h
 *
 *  Created on: Jun 29, 2013
 *      Author: andy
 */

#ifndef LLVMInitialValueCodeGenH
#define LLVMInitialValueCodeGenH

#include "rrLLVMModelGeneratorContext.h"
#include "rrLLVMCodeGen.h"
#include "rrLLVMCodeGenBase.h"
#include "rrLLVMSymbolForest.h"
#include "rrLLVMASTNodeFactory.h"
#include "rrLLVMModelDataIRBuilder.h"
#include "rrLLVMInitialValueSymbolResolver.h"
#include "rrLLVMModelDataSymbolResolver.h"
#include <sbml/Model.h>
#include <sbml/SBMLVisitor.h>

namespace rr
{

typedef void (*LLVMEvalInitialConditions_FunctionPtr)(ModelData*);

/**
 * Generates a function called 'modeldata_initialvalues_set', which evaluates
 * all of the initial conditions specified in the sbml model (initial values,
 * initial assigments, etc...) and stores these values in the appropriate
 * fields in the ModelData structure.
 *
 * generated function signature:
 * void modeldata_initialvalues_set(ModelData *);
 */
class LLVMEvalInitialConditionsCodeGen:
    public  LLVMCodeGenBase<LLVMEvalInitialConditions_FunctionPtr>
{
public:
    LLVMEvalInitialConditionsCodeGen(const LLVMModelGeneratorContext &mgc);
    virtual ~LLVMEvalInitialConditionsCodeGen();

    llvm::Value *codeGen();

    static const char* FunctionName;
    typedef LLVMEvalInitialConditions_FunctionPtr FunctionPtr;

private:

    void codeGenSpecies(LLVMModelDataStoreSymbolResolver& modelDataResolver);

    void codeGenStoichiometry(llvm::Value *modelData,
            LLVMModelDataStoreSymbolResolver& modelDataResolver);

    void codeGenCompartments(LLVMModelDataStoreSymbolResolver& modelDataResolver);

    void codeGenParameters(LLVMModelDataStoreSymbolResolver& modelDataResolver);

    LLVMInitialValueSymbolResolver initialValueResolver;
};

} /* namespace rr */
#endif /* LLVMInitialValueCodeGenH */
