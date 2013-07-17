/*
 * rrLLVMEvalReactionRatesCodeGen.h
 *
 *  Created on: Jul 15, 2013
 *      Author: andy
 */

#ifndef rrLLVMEvalReactionRatesCodeGen_H
#define rrLLVMEvalReactionRatesCodeGen_H

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

class LLVMEvalReactionRatesCodeGen: private SBMLVisitor,
        private LLVMCodeGenBase,
        private LLVMSymbolResolver
{
    using SBMLVisitor::visit;
public:
    LLVMEvalReactionRatesCodeGen(const LLVMModelGeneratorContext &mgc);
    virtual ~LLVMEvalReactionRatesCodeGen();

    llvm::Value *codeGen();

    static const char* FunctionName;
    typedef void (*FunctionPtr)(ModelData*);

    FunctionPtr createFunction();
};

} /* namespace rr */
#endif /* rrLLVMEvalReactionRatesCodeGen_H */
