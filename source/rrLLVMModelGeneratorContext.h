/*
 * rrLLVMModelGeneratorContext.h
 *
 *  Created on: Jun 18, 2013
 *
 * Author: Andy Somogyi,
 *     email decode: V1 = "."; V2 = "@"; V3 = V1;
 *     andy V1 somogyi V2 gmail V3 com
 */

#ifndef rrLLVMModelGeneratorContextH
#define rrLLVMModelGeneratorContextH

#include "rrLLVMModelDataSymbols.h"
#include "rrLLVMIncludes.h"
#include <sbml/Model.h>
#include <sbml/SBMLDocument.h>
#include <string>

namespace libsbml {
class SBMLDocument;
class Model;
}

namespace rr
{

class LLVMModelGeneratorContext
{
public:
    LLVMModelGeneratorContext();
    LLVMModelGeneratorContext(std::string const &sbml, bool computeAndAssignConsevationLaws);
    LLVMModelGeneratorContext(libsbml::SBMLDocument const *doc, bool computeAndAssignConsevationLaws);
    ~LLVMModelGeneratorContext();

    LLVMModelDataSymbols &getModelDataSymbols();

    const libsbml::SBMLDocument *getDocument();

    const libsbml::Model *getModel();

    /**
     * get the llvm context for this thread.
     */
    llvm::LLVMContext &getContext();

    llvm::ExecutionEngine *getExecutionEngine();

    llvm::Module *getModule();

    llvm::IRBuilder<> *getBuilder();

private:
    llvm::LLVMContext *context;
    llvm::ExecutionEngine *executionEngine;
    llvm::Module *module;
    std::string *errString;



    /**
     * these point to the same location, ownedDoc is set if we create the doc,
     * otherwise its 0, meaning we're borrowign the the doc.
     */
    libsbml::SBMLDocument *ownedDoc;

    /**
     * allways references the sbml doc.
     */
    libsbml::SBMLDocument const *doc;

    LLVMModelDataSymbols symbols;

    llvm::IRBuilder<> *builder;
};

} /* namespace rr */
#endif /* rrLLVMModelGeneratorContextH */
