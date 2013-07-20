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
        symbols(new LLVMModelDataSymbols(doc->getModel(),
                computeAndAssignConsevationLaws)),
        modelSymbols(new LLVMModelSymbols(getModel(), *symbols)),
        errString(new string())
{
    // initialize LLVM
    // TODO check result
    InitializeNativeTarget();

    context = new LLVMContext();
    // Make the module, which holds all the code.
    module = new Module("LLVM Module", *context);

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
        symbols(new LLVMModelDataSymbols(doc->getModel(),
            computeAndAssignConsevationLaws)),
        modelSymbols(new LLVMModelSymbols(getModel(), *symbols)),
        errString(new string())
{
    // initialize LLVM
    // TODO check result
    InitializeNativeTarget();

    context = new LLVMContext();
    // Make the module, which holds all the code.
    module = new Module("LLVM Module", *context);

    builder = new IRBuilder<>(*context);

    EngineBuilder engineBuilder(module);
    //engineBuilder.setEngineKind(EngineKind::JIT);
    engineBuilder.setErrorStr(errString);
    executionEngine = engineBuilder.create();

    addGlobalMappings();
}

LLVMModelGeneratorContext::LLVMModelGeneratorContext() :
        ownedDoc(0),
        doc(0),
        symbols(0),
        modelSymbols(0),
        errString(new string())
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
    delete modelSymbols;
    delete symbols;
    delete builder;
    delete executionEngine;
    delete context;
    delete ownedDoc;
    delete errString;
}

llvm::LLVMContext &LLVMModelGeneratorContext::getContext() const
{
    return *context;
}

llvm::ExecutionEngine &LLVMModelGeneratorContext::getExecutionEngine() const
{
    return *executionEngine;
}

const LLVMModelDataSymbols& LLVMModelGeneratorContext::getModelDataSymbols() const
{
    return *symbols;
}

const libsbml::SBMLDocument* LLVMModelGeneratorContext::getDocument() const
{
    return doc;
}

const libsbml::Model* LLVMModelGeneratorContext::getModel() const
{
    return doc->getModel();
}


llvm::Module *LLVMModelGeneratorContext::getModule() const
{
    return module;
}

llvm::IRBuilder<> &LLVMModelGeneratorContext::getBuilder() const
{
    return *builder;
}

void LLVMModelGeneratorContext::stealThePeach(LLVMModelDataSymbols **sym,
        llvm::LLVMContext** ctx, llvm::ExecutionEngine** eng, string** err)
{
    *sym = symbols;
    symbols = 0;
    *ctx = context;
    context = 0;
    *eng = executionEngine;
    executionEngine = 0;
    *err = errString;
    errString = 0;
}

const LLVMModelSymbols& LLVMModelGeneratorContext::getModelSymbols() const
{
    return *modelSymbols;
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
    executionEngine->addGlobalMapping(LLVMModelDataIRBuilderTesting::getDispIntDecl(module), (void*)dispInt);
    executionEngine->addGlobalMapping(LLVMModelDataIRBuilderTesting::getDispDoubleDecl(module), (void*)dispDouble);
    executionEngine->addGlobalMapping(LLVMModelDataIRBuilderTesting::getDispCharDecl(module), (void*)dispChar);
}



} /* namespace rr */
