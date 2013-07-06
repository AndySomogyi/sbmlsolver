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

/**
 * All LLVM code generating objects basically need at a minimum three things
 * to operate:
 *
 * 1: sbml Model - what to process
 * 2: llvm Context - all llvm functions need this, manages llvm memory
 * 3: llvm Module - where the generated code will go
 *
 *
 * Code generators also need to know the symbols that are in the sbml doc.
 * These *could* be re-determined in each code gen, but wastefull. The
 * symbols can be thought of as the first pass of the compiler, so all symbols
 * are determined ahead of time before any code generation.
 *
 * They also make use of an llvm IRBuilder, one of these *could* be allocated
 * in each code gen object, but that would be wastefull, so we use a single one
 * per sbml compilation, and each code gen just resets the builder insert point,
 * that is what is was designed to do.
 *
 * So, that leaves us with 5 object that are needed by code generators.
 * Should all code gens have all 5 args passed to thier ctors? There is much
 * talk about how context objects are evil, however this one is NOT inteneded to
 * grow, only store the absolute minimun information that all code generators
 * need.
 *
 * This class basically functions as a set of ivars if all code generators
 * were methods on a single code gen class.
 *
 * TODO doc wise discuss the problems with context objects, and why this one
 * address all the concerns.
 *
 * TODO document memory managment, engine owns model, ...
 */
class LLVMModelGeneratorContext
{
public:
    /**
     * load the sbml document from a string.
     */
    LLVMModelGeneratorContext(std::string const &sbml,
            bool computeAndAssignConsevationLaws);

    /**
     * attach to an existing sbml document, we borrow a reference to this
     * doc and DO NOT take ownership of it.
     */
    LLVMModelGeneratorContext(libsbml::SBMLDocument const *doc,
            bool computeAndAssignConsevationLaws);

    ~LLVMModelGeneratorContext();

    const LLVMModelDataSymbols &getModelDataSymbols() const;

    const libsbml::SBMLDocument *getDocument() const;

    /**
     * TODO: this really really should be const, but const correctness is not 100% correct in libsbml.
     */
    libsbml::Model *getModel() const;

    llvm::LLVMContext &getContext() const;

    llvm::ExecutionEngine *getExecutionEngine() const;

    llvm::Module *getModule() const;

    llvm::IRBuilder<> *getBuilder() const;

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
    const libsbml::SBMLDocument *doc;

    LLVMModelDataSymbols symbols;

    llvm::IRBuilder<> *builder;
};

} /* namespace rr */
#endif /* rrLLVMModelGeneratorContextH */
