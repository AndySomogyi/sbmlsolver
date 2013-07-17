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
 * Generates a function called 'modeldata_initialvalues_set', which evaluates
 * all of the initial conditions specified in the sbml model (initial values,
 * initial assigments, etc...) and stores these values in the appropriate
 * fields in the ModelData structure.
 *
 * generated function signature:
 * void modeldata_initialvalues_set(ModelData *);
 */
class LLVMEvalInitialConditionsCodeGen: private SBMLVisitor,
        private LLVMCodeGenBase,
        private LLVMSymbolResolver
{
    using SBMLVisitor::visit;

public:
    LLVMEvalInitialConditionsCodeGen(const LLVMModelGeneratorContext &mgc);
    ~LLVMEvalInitialConditionsCodeGen();

    llvm::Value *codeGen();

    static const char* FunctionName;
    typedef void (*FunctionPtr)(ModelData*);

    FunctionPtr createFunction();

private:


    virtual llvm::Value *symbolValue(const std::string& symbol);


    void codeGenFloatingSpecies(llvm::Value *modelData,
            LLVMModelDataIRBuilder &modelDataBuilder);

    void codeGenStoichiometry(llvm::Value *modelData,
            LLVMModelDataIRBuilder &modelDataBuilder);

    llvm::Function *initialValuesFunc;

    llvm::ExecutionEngine *engine;
};

} /* namespace rr */
#endif /* LLVMInitialValueCodeGenH */
