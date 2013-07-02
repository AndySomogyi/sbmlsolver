/*
 * rrLLVMModelGeneratorContext.cpp
 *
 *  Created on: Jun 18, 2013
 *      Author: andy
 */

#include "rrLLVMModelGeneratorContext.h"
#include <sbml/SBMLReader.h>
#include "rrLLVMIncludes.h"


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
    //engineBuilder.setEngineKind(EngineKind::JIT);
    engineBuilder.setErrorStr(errString);
    executionEngine = engineBuilder.create();
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

    errString = new std::string();

    EngineBuilder engineBuilder(module);
    //engineBuilder.setEngineKind(EngineKind::JIT);
    engineBuilder.setErrorStr(errString);
    executionEngine = engineBuilder.create();
}


LLVMModelGeneratorContext::~LLVMModelGeneratorContext()
{
    delete executionEngine;
    delete context;
}

llvm::LLVMContext& LLVMModelGeneratorContext::getContext()
{
    return *context;
}

llvm::ExecutionEngine* LLVMModelGeneratorContext::getExecutionEngine()
{
    return executionEngine;
}

LLVMModelDataSymbols& LLVMModelGeneratorContext::getModelDataSymbols()
{
    return symbols;
}

const libsbml::SBMLDocument* LLVMModelGeneratorContext::getDocument()
{
    return doc;
}

const libsbml::Model* LLVMModelGeneratorContext::getModel()
{
    return doc->getModel();
}

llvm::Module* LLVMModelGeneratorContext::getModule()
{
    return module;
}

llvm::IRBuilder<>* LLVMModelGeneratorContext::getBuilder()
{
    return builder;
}

} /* namespace rr */
