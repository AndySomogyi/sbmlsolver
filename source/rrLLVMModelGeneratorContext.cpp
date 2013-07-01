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

LLVMModelGeneratorContext::LLVMModelGeneratorContext(std::string const &sbml) :
    ownedDoc(readSBMLFromString((sbml.c_str()))),
    doc(ownedDoc)
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

LLVMModelGeneratorContext::LLVMModelGeneratorContext(libsbml::SBMLDocument const *doc) :
        ownedDoc(0),
        doc(doc)
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

}

llvm::LLVMContext& LLVMModelGeneratorContext::getContext()
{
    return *context;
}

llvm::ExecutionEngine* LLVMModelGeneratorContext::getExecutionEngine()
{
    return executionEngine;
}



LLVMModelDataValue& LLVMModelGeneratorContext::getModelDataSymbols()
{
}

const libsbml::SBMLDocument* LLVMModelGeneratorContext::getDocument()
{
}

const libsbml::Model* LLVMModelGeneratorContext::getModel()
{
}

llvm::Module* LLVMModelGeneratorContext::getModule()
{
    return module;
}



} /* namespace rr */
