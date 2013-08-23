/*
 * ModelGeneratorContext.cpp
 *
 *  Created on: Jun 18, 2013
 *      Author: andy
 */

#include "ModelGeneratorContext.h"
#include "rrSparse.h"
#include "LLVMIncludes.h"
#include "ModelDataIRBuilder.h"
#include "LLVMException.h"

#include <sbml/SBMLReader.h>


#include <string>
#include <vector>


using namespace llvm;
using namespace std;
using namespace libsbml;

namespace rr
{

static void createLibraryFunctions(Module* module);

static void createLibraryFunction(llvm::LibFunc::Func funcId,
        llvm::FunctionType *funcType, Module* module);

ModelGeneratorContext::ModelGeneratorContext(std::string const &sbml,
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

    // engine take ownership of module
    EngineBuilder engineBuilder(module);

    engineBuilder.setErrorStr(errString);
    executionEngine = engineBuilder.create();

    addGlobalMappings();

    createLibraryFunctions(module);
}

ModelGeneratorContext::ModelGeneratorContext(libsbml::SBMLDocument const *doc,
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

    // engine take ownership of module
    EngineBuilder engineBuilder(module);

    //engineBuilder.setEngineKind(EngineKind::JIT);
    engineBuilder.setErrorStr(errString);
    executionEngine = engineBuilder.create();

    addGlobalMappings();

    createLibraryFunctions(module);
}

static SBMLDocument *createEmptyDocument()
{
    SBMLDocument *doc = new SBMLDocument();
    doc->createModel("");
    return doc;
}

ModelGeneratorContext::ModelGeneratorContext() :
        ownedDoc(createEmptyDocument()),
        doc(ownedDoc),
        symbols(new LLVMModelDataSymbols(doc->getModel(), false)),
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

    errString = new std::string();

    EngineBuilder engineBuilder(module);
    //engineBuilder.setEngineKind(EngineKind::JIT);
    engineBuilder.setErrorStr(errString);
    executionEngine = engineBuilder.create();

    addGlobalMappings();
}


ModelGeneratorContext::~ModelGeneratorContext()
{
    delete modelSymbols;
    delete symbols;
    delete builder;
    delete executionEngine;
    delete context;
    delete ownedDoc;
    delete errString;
}

llvm::LLVMContext &ModelGeneratorContext::getContext() const
{
    return *context;
}

llvm::ExecutionEngine &ModelGeneratorContext::getExecutionEngine() const
{
    return *executionEngine;
}

const LLVMModelDataSymbols& ModelGeneratorContext::getModelDataSymbols() const
{
    return *symbols;
}

const libsbml::SBMLDocument* ModelGeneratorContext::getDocument() const
{
    return doc;
}

const libsbml::Model* ModelGeneratorContext::getModel() const
{
    return doc->getModel();
}


llvm::Module *ModelGeneratorContext::getModule() const
{
    return module;
}

llvm::IRBuilder<> &ModelGeneratorContext::getBuilder() const
{
    return *builder;
}

void ModelGeneratorContext::stealThePeach(LLVMModelDataSymbols **sym,
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

const LLVMModelSymbols& ModelGeneratorContext::getModelSymbols() const
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

void ModelGeneratorContext::addGlobalMappings()
{
    executionEngine->addGlobalMapping(ModelDataIRBuilder::getCSRMatrixSetNZDecl(module), (void*)csr_matrix_set_nz);
    executionEngine->addGlobalMapping(ModelDataIRBuilder::getCSRMatrixGetNZDecl(module), (void*)csr_matrix_get_nz);
    executionEngine->addGlobalMapping(LLVMModelDataIRBuilderTesting::getDispIntDecl(module), (void*)dispInt);
    executionEngine->addGlobalMapping(LLVMModelDataIRBuilderTesting::getDispDoubleDecl(module), (void*)dispDouble);
    executionEngine->addGlobalMapping(LLVMModelDataIRBuilderTesting::getDispCharDecl(module), (void*)dispChar);
}

static void createLibraryFunctions(Module* module)
{
    LLVMContext& context = module->getContext();
    Type *double_type = Type::getDoubleTy(context);
    Type* args_d1[] = { double_type };
    Type* args_d2[] = { double_type, double_type };

    /// double pow(double x, double y);
    createLibraryFunction(LibFunc::pow,
            FunctionType::get(double_type, args_d2, false), module);

    /// double fabs(double x);
    createLibraryFunction(LibFunc::fabs,
            FunctionType::get(double_type, args_d1, false), module);

    /// double acos(double x);
    createLibraryFunction(LibFunc::acos,
            FunctionType::get(double_type, args_d1, false), module);

    /// double acosh(double x);
    createLibraryFunction(LibFunc::acosh,
            FunctionType::get(double_type, args_d1, false), module);

    /// double asin(double x);
    createLibraryFunction(LibFunc::asin,
            FunctionType::get(double_type, args_d1, false), module);

    /// double asinh(double x);
    createLibraryFunction(LibFunc::asinh,
            FunctionType::get(double_type, args_d1, false), module);

    /// double atan(double x);
    createLibraryFunction(LibFunc::atan,
            FunctionType::get(double_type, args_d1, false), module);

    /// double atanh(double x);
    createLibraryFunction(LibFunc::atanh,
            FunctionType::get(double_type, args_d1, false), module);

    /// double ceil(double x);
    createLibraryFunction(LibFunc::ceil,
            FunctionType::get(double_type, args_d1, false), module);

    /// double cos(double x);
    createLibraryFunction(LibFunc::cos,
            FunctionType::get(double_type, args_d1, false), module);

    /// double cosh(double x);
    createLibraryFunction(LibFunc::cosh,
            FunctionType::get(double_type, args_d1, false), module);

    /// double exp(double x);
    createLibraryFunction(LibFunc::exp,
            FunctionType::get(double_type, args_d1, false), module);

    /// double floor(double x);
    createLibraryFunction(LibFunc::floor,
            FunctionType::get(double_type, args_d1, false), module);

    /// double log(double x);
    createLibraryFunction(LibFunc::log,
            FunctionType::get(double_type, args_d1, false), module);

    /// double log10(double x);
    createLibraryFunction(LibFunc::log10,
            FunctionType::get(double_type, args_d1, false), module);

    /// double sin(double x);
    createLibraryFunction(LibFunc::sin,
            FunctionType::get(double_type, args_d1, false), module);

    /// double sinh(double x);
    createLibraryFunction(LibFunc::sinh,
            FunctionType::get(double_type, args_d1, false), module);

    /// double tan(double x);
    createLibraryFunction(LibFunc::tan,
            FunctionType::get(double_type, args_d1, false), module);

    /// double tanh(double x);
    createLibraryFunction(LibFunc::tanh,
            FunctionType::get(double_type, args_d1, false), module);
}

static void createLibraryFunction(llvm::LibFunc::Func funcId,
        llvm::FunctionType *funcType, Module* module)
{
    TargetLibraryInfo targetLib;

    if (targetLib.has(funcId))
    {
        Function::Create(funcType, Function::ExternalLinkage,
                targetLib.getName(funcId), module);
    }
    else
    {
        string msg = "native target does not have library function for ";
        msg += targetLib.getName(funcId);
        throw_llvm_exception(msg);
    }
}

static void createGlobalMappingFunction(const char* funcName,
        llvm::FunctionType *funcType, Module *module)
{
    Function::Create(funcType, Function::InternalLinkage, funcName, module);
}

} /* namespace rr */
