/*
 * rrLLVMModelGeneratorContext.cpp
 *
 *  Created on: Jun 18, 2013
 *      Author: andy
 */

#include "rrLLVMModelGeneratorContext.h"
#include "rrSparse.h"
#include "rrLLVMIncludes.h"
#include "rrLLVMModelDataIRBuilder.h"

#include <sbml/SBMLReader.h>


#include <string>
#include <vector>


using namespace llvm;
using namespace std;
using namespace libsbml;

namespace rr
{

LLVMModelGeneratorContext::LLVMModelGeneratorContext(std::string const &sbml,
        bool computeAndAssignConsevationLaws) :
    ownedDoc(readSBMLFromString((sbml.c_str()))),
    doc(ownedDoc),
    symbols(doc->getModel(), computeAndAssignConsevationLaws)
{
    // initialize LLVM
    // TODO check result
    InitializeNativeTarget();

    context = new LLVMContext();
    // Make the module, which holds all the code.
    module = new Module("LLVM Module", *context);

    errString = new std::string();

    builder = new IRBuilder<>(*context);

    EngineBuilder engineBuilder(module);

    engineBuilder.setErrorStr(errString);
    executionEngine = engineBuilder.create();

    addGlobalMappings();
}

LLVMModelGeneratorContext::LLVMModelGeneratorContext(libsbml::SBMLDocument const *doc,
        bool computeAndAssignConsevationLaws) :
        ownedDoc(0),
        doc(doc),
        symbols(doc->getModel(), computeAndAssignConsevationLaws)
{
    // initialize LLVM
    // TODO check result
    InitializeNativeTarget();

    context = new LLVMContext();
    // Make the module, which holds all the code.
    module = new Module("LLVM Module", *context);

    builder = new IRBuilder<>(*context);

    errString = new std::string();

    EngineBuilder engineBuilder(module);
    //engineBuilder.setEngineKind(EngineKind::JIT);
    engineBuilder.setErrorStr(errString);
    executionEngine = engineBuilder.create();

    addGlobalMappings();
}

LLVMModelGeneratorContext::LLVMModelGeneratorContext() :
        ownedDoc(0),
        doc(0)
{
    // initialize LLVM
    // TODO check result
    InitializeNativeTarget();

    context = new LLVMContext();
    // Make the module, which holds all the code.
    module = new Module("LLVM Module", *context);

    builder = new IRBuilder<>(*context);

    errString = new std::string();

    EngineBuilder engineBuilder(module);
    //engineBuilder.setEngineKind(EngineKind::JIT);
    engineBuilder.setErrorStr(errString);
    executionEngine = engineBuilder.create();

    addGlobalMappings();
}


LLVMModelGeneratorContext::~LLVMModelGeneratorContext()
{
    delete executionEngine;
    delete context;
}

llvm::LLVMContext& LLVMModelGeneratorContext::getContext() const
{
    return *context;
}

llvm::ExecutionEngine* LLVMModelGeneratorContext::getExecutionEngine() const
{
    return executionEngine;
}

const LLVMModelDataSymbols& LLVMModelGeneratorContext::getModelDataSymbols() const
{
    return symbols;
}

const libsbml::SBMLDocument* LLVMModelGeneratorContext::getDocument() const
{
    return doc;
}


// TODO: this is freaking LAME, libsbml is not 100% const correct, so get around the
// issues with these pragmas.
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-qual"
#endif
libsbml::Model* LLVMModelGeneratorContext::getModel() const
{
    return (Model*)doc->getModel();
}
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

llvm::Module* LLVMModelGeneratorContext::getModule() const
{
    return module;
}

llvm::IRBuilder<>* LLVMModelGeneratorContext::getBuilder() const
{
    return builder;
}



/*********************** TESTING STUFF WILL GO AWAY EVENTUALLY ***********************/

static void dispDouble(double d) {
    cout << __FUNC__ << ": " << d << "\n";
}

static void dispInt(int i) {
    cout << __FUNC__ << ": " << i << "\n";
}

static void dispChar(char c) {
    cout << __FUNC__ << ": " << (int)c << "\n";
}

/*************************************************************************************/

void LLVMModelGeneratorContext::addGlobalMappings()
{
    executionEngine->addGlobalMapping(LLVMModelDataIRBuilder::getCSRMatrixSetNZDecl(module), (void*)csr_matrix_set_nz);
    executionEngine->addGlobalMapping(LLVMModelDataIRBuilder::getCSRMatrixGetNZDecl(module), (void*)csr_matrix_get_nz);
    executionEngine->addGlobalMapping(LLVMModelDataIRBuilder::getDispIntDecl(module), (void*)dispInt);
    executionEngine->addGlobalMapping(LLVMModelDataIRBuilder::getDispDoubleDecl(module), (void*)dispDouble);
    executionEngine->addGlobalMapping(LLVMModelDataIRBuilder::getDispCharDecl(module), (void*)dispChar);
}



} /* namespace rr */
