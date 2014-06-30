/*
 * EvalInitialValueCodeGen.h
 *
 *  Created on: Jun 29, 2013
 *      Author: andy
 */

#ifndef EvalInitialValueCodeGenH
#define EvalInitialValueCodeGenH

#include "ModelGeneratorContext.h"
#include "CodeGenBase.h"
#include "SymbolForest.h"
#include "ASTNodeFactory.h"
#include "ModelDataIRBuilder.h"
#include "SBMLInitialValueSymbolResolver.h"
#include "ModelDataSymbolResolver.h"
#include <sbml/Model.h>
#include <sbml/SBMLVisitor.h>

namespace rrllvm
{

typedef void (*EvalInitialConditions_FunctionPtr)(LLVMModelData*);

/**
 * Generates a function called 'modeldata_initialvalues_set', which evaluates
 * all of the initial conditions specified in the sbml model (initial values,
 * initial assigments, etc...) and stores these values in the appropriate
 * fields in the LLVMModelData structure.
 *
 * generated function signature:
 * void modeldata_initialvalues_set(ModelData *);
 */
class EvalInitialConditionsCodeGen:
    public  CodeGenBase<EvalInitialConditions_FunctionPtr>
{
public:
    EvalInitialConditionsCodeGen(const ModelGeneratorContext &mgc);
    virtual ~EvalInitialConditionsCodeGen();

    llvm::Value *codeGen();

    static const char* FunctionName;
    typedef EvalInitialConditions_FunctionPtr FunctionPtr;

private:

    void codeGenSpecies(StoreSymbolResolver& modelDataResolver);

    void codeGenStoichiometry(llvm::Value *modelData,
            ModelDataStoreSymbolResolver& modelDataResolver);

    void codeGenCompartments(StoreSymbolResolver& modelDataResolver);

    void codeGenGlobalParameters(StoreSymbolResolver& modelDataResolver);

    void codeGenInitSpecies(StoreSymbolResolver& modelDataResolver);

    void codeGenInitCompartments(StoreSymbolResolver& modelDataResolver);

    void codeGenInitGlobalParameters(StoreSymbolResolver& modelDataResolver);

    SBMLInitialValueSymbolResolver initialValueResolver;
};

} /* namespace rr */
#endif /* EvalInitialValueCodeGenH */
