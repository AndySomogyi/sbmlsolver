/*
 * ModelGeneratorContext.cpp
 *
 *  Created on: Jun 18, 2013
 *      Author: andy
 */
#pragma hdrstop
#include "ModelGeneratorContext.h"
#include "rrSparse.h"
#include "LLVMIncludes.h"
#include "ModelDataIRBuilder.h"
#include "LLVMException.h"
#include "SBMLSupportFunctions.h"
#include "conservation/ConservedMoietyConverter.h"
#include "conservation/ConservationExtension.h"
#include "rrRoadRunnerOptions.h"
#include "rrConfig.h"

#include <sbml/SBMLReader.h>
#include <string>
#include <vector>
#include <math.h>

#include <sbml/common/libsbml-config-packages.h>
#ifdef LIBSBML_HAS_PACKAGE_DISTRIB
#include <sbml/packages/distrib/common/DistribExtensionTypes.h>
#include "DistribFunctionResolver.h"
#endif

using namespace llvm;
using namespace std;
using namespace libsbml;

using rr::Logger;
using rr::LoadSBMLOptions;

namespace rrllvm
{

static void createLibraryFunctions(Module* module);

static void createLibraryFunction(llvm::LibFunc::Func funcId,
        llvm::FunctionType *funcType, Module* module);

static Function* createGlobalMappingFunction(const char* funcName,
        llvm::FunctionType *funcType, Module *module);


/**
 * returns a VALID sbml document, if the doc has any error,
 * an exception is thrown.
 */
static SBMLDocument *checkedReadSBMLFromString(const char* xml);

// MSVC 2010 and earlier do not include the hyperbolic functions, define there here
// MSVC++ 11.0 _MSC_VER == 1700 (Visual Studio 2012)
// Note, evidently including the <amp_math.h> causes issues in 2012,
// so it seems to work just defining these here as static.
#if defined(_MSC_VER)

static double asinh(double value)
{
    return log(value + sqrt(value * value + 1.));
}

static double acosh(double value)
{
    return log(value + sqrt(value * value - 1.));
}

double atanh(double value)
{
    return (std::log(1.0 + value) - std::log(1.0 - value)) / 2.0;
}

#endif

ModelGeneratorContext::ModelGeneratorContext(std::string const &sbml,
    unsigned options) :
        ownedDoc(0),
        doc(0),
        symbols(0),
        modelSymbols(0),
        errString(new string()),
        context(0),
        executionEngine(0),
        module(0),
        builder(0),
        functionPassManager(0),
        options(options),
        moietyConverter(0),
        random(0)
{
    if(useSymbolCache()) {
        Log(Logger::LOG_INFORMATION) << "Using LLVM symbol/value cache";
    } else {
        Log(Logger::LOG_INFORMATION) << "Not using LLVM symbol/value cache";
    }

    try
    {
        ownedDoc = checkedReadSBMLFromString(sbml.c_str());

        if (options & LoadSBMLOptions::CONSERVED_MOIETIES)
        {
            if ((rr::Config::getInt(rr::Config::ROADRUNNER_DISABLE_WARNINGS) &
                    rr::Config::ROADRUNNER_DISABLE_WARNINGS_CONSERVED_MOIETY) == 0)
            {
                Log(Logger::LOG_NOTICE) << "performing conserved moiety conversion";
            }

            // check if already conserved doc
            if (rr::conservation::ConservationExtension::isConservedMoietyDocument(ownedDoc))
            {
                doc = ownedDoc;
            }
            else
            {
                moietyConverter = new rr::conservation::ConservedMoietyConverter();

                if (moietyConverter->setDocument(ownedDoc) != LIBSBML_OPERATION_SUCCESS)
                {
                    throw_llvm_exception("error setting conserved moiety converter document");
                }

                if (moietyConverter->convert() != LIBSBML_OPERATION_SUCCESS)
                {
                    throw_llvm_exception("error converting document to conserved moieties");
                }

                doc = moietyConverter->getDocument();

                SBMLWriter sw;
                char* convertedStr = sw.writeToString(doc);

                Log(Logger::LOG_INFORMATION) << "***************** Conserved Moiety Converted Document ***************";
                Log(Logger::LOG_INFORMATION) << convertedStr;
                Log(Logger::LOG_INFORMATION) << "*********************************************************************";

                free(convertedStr);
            }
        }
        else
        {
            doc = ownedDoc;
        }

        symbols = new LLVMModelDataSymbols(doc->getModel(), options);

        modelSymbols = new LLVMModelSymbols(getModel(), *symbols);

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

        ModelDataIRBuilder::createModelDataStructType(module, executionEngine, *symbols);

        // check if doc has distrib package
        // Random adds mappings, need call after llvm objs created
#ifdef LIBSBML_HAS_PACKAGE_DISTRIB
        const DistribSBMLDocumentPlugin* distrib =
                dynamic_cast<const DistribSBMLDocumentPlugin*>(
                        doc->getPlugin("distrib"));
        if(distrib)
        {
            random = new Random(*this);
        }
#endif

        initFunctionPassManager();

    }
    catch(const std::exception&)
    {
        // we might have allocated memory in a failed ctor,
        // clean it up here.
        // destructors are not called on *this* class when exception is raised
        // in the ctor.
        cleanup();
        throw;
    }
}




ModelGeneratorContext::ModelGeneratorContext(libsbml::SBMLDocument const *doc,
    unsigned options) :
        ownedDoc(0),
        doc(0),
        symbols(new LLVMModelDataSymbols(doc->getModel(), options)),
        modelSymbols(new LLVMModelSymbols(getModel(), *symbols)),
        errString(new string()),
        context(0),
        executionEngine(0),
        module(0),
        builder(0),
        functionPassManager(0),
        options(options),
        moietyConverter(0),
        random(0)
{
    if(useSymbolCache()) {
        Log(Logger::LOG_INFORMATION) << "Using LLVM symbol/value cache";
    } else {
        Log(Logger::LOG_INFORMATION) << "Not using LLVM symbol/value cache";
    }

    try
    {
        if (options & LoadSBMLOptions::CONSERVED_MOIETIES)
        {
            Log(Logger::LOG_NOTICE) << "performing conserved moiety conversion";

            moietyConverter = new rr::conservation::ConservedMoietyConverter();

            if (moietyConverter->setDocument(doc) != LIBSBML_OPERATION_SUCCESS)
            {
                throw_llvm_exception("error setting conserved moiety converter document");
            }

            if (moietyConverter->convert() != LIBSBML_OPERATION_SUCCESS)
            {
                throw_llvm_exception("error converting document to conserved moieties");
            }

            this->doc = moietyConverter->getDocument();

            SBMLWriter sw;
            char* convertedStr = sw.writeToString(doc);

            Log(Logger::LOG_INFORMATION) << "***************** Conserved Moiety Converted Document ***************";
            Log(Logger::LOG_INFORMATION) << convertedStr;
            Log(Logger::LOG_INFORMATION) << "*********************************************************************";

            delete convertedStr;
        }
        else
        {
            this->doc = doc;
        }

        symbols = new LLVMModelDataSymbols(doc->getModel(), options);

        modelSymbols = new LLVMModelSymbols(getModel(), *symbols);


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

        ModelDataIRBuilder::createModelDataStructType(module, executionEngine, *symbols);

        // check if doc has distrib package
        // Random adds mappings, need call after llvm objs created
#ifdef LIBSBML_HAS_PACKAGE_DISTRIB
        const DistribSBMLDocumentPlugin* distrib =
                dynamic_cast<const DistribSBMLDocumentPlugin*>(
                        doc->getPlugin("distrib"));
        if(distrib)
        {
            random = new Random(*this);
        }
#endif

        initFunctionPassManager();

    }
    catch(const std::exception&)
    {
        cleanup();
        throw;
    }
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
        symbols(new LLVMModelDataSymbols(doc->getModel(), 0)),
        modelSymbols(new LLVMModelSymbols(getModel(), *symbols)),
        errString(new string()),
        options(0),
        functionPassManager(0)
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

Random* ModelGeneratorContext::getRandom() const
{
    return random;
}

void ModelGeneratorContext::cleanup()
{
    delete functionPassManager; functionPassManager = 0;
    delete modelSymbols; modelSymbols = 0;
    delete symbols; symbols = 0;
    delete builder; builder = 0;
    delete executionEngine; executionEngine = 0;
    delete context; context = 0;
    delete moietyConverter; moietyConverter = 0;
    delete ownedDoc; ownedDoc = 0;
    delete errString; errString = 0;
}


ModelGeneratorContext::~ModelGeneratorContext()
{
    cleanup();
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

void ModelGeneratorContext::stealThePeach(const LLVMModelDataSymbols **sym,
        const llvm::LLVMContext** ctx, const llvm::ExecutionEngine** eng,
        const Random** rnd, const string** err)
{
    *sym = symbols;
    symbols = 0;
    *ctx = context;
    context = 0;
    *eng = executionEngine;
    executionEngine = 0;
    *rnd = random;
    random = 0;
    *err = errString;
    errString = 0;
}

const LLVMModelSymbols& ModelGeneratorContext::getModelSymbols() const
{
    return *modelSymbols;
}

bool ModelGeneratorContext::getConservedMoietyAnalysis() const
{
    return (options & LoadSBMLOptions::CONSERVED_MOIETIES) != 0;
}

bool ModelGeneratorContext::useSymbolCache() const
{
    return (options & LoadSBMLOptions::LLVM_SYMBOL_CACHE) != 0;
}

llvm::FunctionPassManager* ModelGeneratorContext::getFunctionPassManager() const
{
    return functionPassManager;
}

void ModelGeneratorContext::initFunctionPassManager()
{
    if (options & LoadSBMLOptions::OPTIMIZE)
    {
        functionPassManager = new FunctionPassManager(module);

    // Set up the optimizer pipeline.  Start with registering info about how the
    // target lays out data structures.

    // we only support LLVM >= 3.1
#if (LLVM_VERSION_MAJOR == 3) && (LLVM_VERSION_MINOR == 1)
    functionPassManager->add(new TargetData(*executionEngine->getTargetData()));
#elif (LLVM_VERSION_MINOR <= 4)
    functionPassManager->add(new DataLayout(*executionEngine->getDataLayout()));
#else // LLVM_VERSION_MINOR > 4
    // Needed for LLVM 3.5 regardless of architecture
    // also, should use DataLayoutPass(module) per Renato (http://reviews.llvm.org/D4607)
    functionPassManager->add(new DataLayoutPass(module));
#endif

         // Provide basic AliasAnalysis support for GVN.
        functionPassManager->add(createBasicAliasAnalysisPass());


        if (options & LoadSBMLOptions::OPTIMIZE_INSTRUCTION_SIMPLIFIER)
        {
            Log(Logger::LOG_INFORMATION) << "using OPTIMIZE_INSTRUCTION_SIMPLIFIER";
            functionPassManager->add(createInstructionSimplifierPass());
        }

        if (options & LoadSBMLOptions::OPTIMIZE_INSTRUCTION_COMBINING)
        {
            Log(Logger::LOG_INFORMATION) << "using OPTIMIZE_INSTRUCTION_COMBINING";
            functionPassManager->add(createInstructionCombiningPass());
        }

        if(options & LoadSBMLOptions::OPTIMIZE_GVN)
        {
            Log(Logger::LOG_INFORMATION) << "using GVN optimization";
            functionPassManager->add(createGVNPass());
        }

        if (options & LoadSBMLOptions::OPTIMIZE_CFG_SIMPLIFICATION)
        {
            Log(Logger::LOG_INFORMATION) << "using OPTIMIZE_CFG_SIMPLIFICATION";
            functionPassManager->add(createCFGSimplificationPass());
        }

        if (options & LoadSBMLOptions::OPTIMIZE_DEAD_INST_ELIMINATION)
        {
            Log(Logger::LOG_INFORMATION) << "using OPTIMIZE_DEAD_INST_ELIMINATION";
            functionPassManager->add(createDeadInstEliminationPass());
        }

        if (options & LoadSBMLOptions::OPTIMIZE_DEAD_CODE_ELIMINATION)
        {
            Log(Logger::LOG_INFORMATION) << "using OPTIMIZE_DEAD_CODE_ELIMINATION";
            functionPassManager->add(createDeadCodeEliminationPass());
        }


        functionPassManager->doInitialization();
    }
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
    LLVMContext& context = module->getContext();
    Type *double_type = Type::getDoubleTy(context);
    Type *int_type = Type::getInt32Ty(context);
    Type* args_i1[] = { int_type };
    Type* args_d1[] = { double_type };
    Type* args_d2[] = { double_type, double_type };

    executionEngine->addGlobalMapping(ModelDataIRBuilder::getCSRMatrixSetNZDecl(module), (void*)rr::csr_matrix_set_nz);
    executionEngine->addGlobalMapping(ModelDataIRBuilder::getCSRMatrixGetNZDecl(module), (void*)rr::csr_matrix_get_nz);
    executionEngine->addGlobalMapping(LLVMModelDataIRBuilderTesting::getDispIntDecl(module), (void*)dispInt);
    executionEngine->addGlobalMapping(LLVMModelDataIRBuilderTesting::getDispDoubleDecl(module), (void*)dispDouble);
    executionEngine->addGlobalMapping(LLVMModelDataIRBuilderTesting::getDispCharDecl(module), (void*)dispChar);

    // AST_FUNCTION_ARCCOT:
    executionEngine->addGlobalMapping(
            createGlobalMappingFunction("arccot",
                    FunctionType::get(double_type, args_d1, false), module),
                        (void*) sbmlsupport::arccot);

    executionEngine->addGlobalMapping(
            createGlobalMappingFunction("rr_arccot_negzero",
                    FunctionType::get(double_type, args_d1, false), module),
                        (void*) sbmlsupport::arccot_negzero);

    // AST_FUNCTION_ARCCOTH:
    executionEngine->addGlobalMapping(
            createGlobalMappingFunction("arccoth",
                    FunctionType::get(double_type, args_d1, false), module),
                        (void*) sbmlsupport::arccoth);

    // AST_FUNCTION_ARCCSC:
    executionEngine->addGlobalMapping(
            createGlobalMappingFunction("arccsc",
                    FunctionType::get(double_type, args_d1, false), module),
                        (void*) sbmlsupport::arccsc);

    // AST_FUNCTION_ARCCSCH:
    executionEngine->addGlobalMapping(
            createGlobalMappingFunction("arccsch",
                    FunctionType::get(double_type, args_d1, false), module),
                        (void*) sbmlsupport::arccsch);

    // AST_FUNCTION_ARCSEC:
    executionEngine->addGlobalMapping(
            createGlobalMappingFunction("arcsec",
                    FunctionType::get(double_type, args_d1, false), module),
                        (void*) sbmlsupport::arcsec);

    // AST_FUNCTION_ARCSECH:
    executionEngine->addGlobalMapping(
            createGlobalMappingFunction("arcsech",
                    FunctionType::get(double_type, args_d1, false), module),
                        (void*) sbmlsupport::arcsech);

    // AST_FUNCTION_COT:
    executionEngine->addGlobalMapping(
            createGlobalMappingFunction("cot",
                    FunctionType::get(double_type, args_d1, false), module),
                        (void*) sbmlsupport::cot);

    // AST_FUNCTION_COTH:
    executionEngine->addGlobalMapping(
            createGlobalMappingFunction("coth",
                    FunctionType::get(double_type, args_d1, false), module),
                        (void*) sbmlsupport::coth);

    // AST_FUNCTION_CSC:
    executionEngine->addGlobalMapping(
            createGlobalMappingFunction("csc",
                    FunctionType::get(double_type, args_d1, false), module),
                        (void*) sbmlsupport::csc);

    // AST_FUNCTION_CSCH:
    executionEngine->addGlobalMapping(
            createGlobalMappingFunction("csch",
                    FunctionType::get(double_type, args_d1, false), module),
                        (void*) sbmlsupport::csch);

    // AST_FUNCTION_FACTORIAL:
    executionEngine->addGlobalMapping(
            createGlobalMappingFunction("rr_factoriali",
                    FunctionType::get(int_type, args_i1, false), module),
                        (void*) sbmlsupport::factoriali);

    executionEngine->addGlobalMapping(
            createGlobalMappingFunction("rr_factoriald",
                    FunctionType::get(double_type, args_d1, false), module),
                        (void*) sbmlsupport::factoriald);

    // case AST_FUNCTION_LOG:
    executionEngine->addGlobalMapping(
            createGlobalMappingFunction("rr_logd",
                    FunctionType::get(double_type, args_d2, false), module),
                        (void*) sbmlsupport::logd);

    // AST_FUNCTION_ROOT:
    executionEngine->addGlobalMapping(
            createGlobalMappingFunction("rr_rootd",
                    FunctionType::get(double_type, args_d2, false), module),
                        (void*) sbmlsupport::rootd);

    // AST_FUNCTION_SEC:
    executionEngine->addGlobalMapping(
            createGlobalMappingFunction("sec",
                    FunctionType::get(double_type, args_d1, false), module),
                        (void*) sbmlsupport::sec);

    // AST_FUNCTION_SECH:
    executionEngine->addGlobalMapping(
            createGlobalMappingFunction("sech",
                    FunctionType::get(double_type, args_d1, false), module),
                        (void*) sbmlsupport::sech);

    // AST_FUNCTION_ARCCOSH:
    executionEngine->addGlobalMapping(
            createGlobalMappingFunction("arccosh",
                    FunctionType::get(double_type, args_d1, false), module),
                        (void*)static_cast<double (*)(double)>(acosh));

    // AST_FUNCTION_ARCSINH:
    executionEngine->addGlobalMapping(
            createGlobalMappingFunction("arcsinh",
                    FunctionType::get(double_type, args_d1, false), module),
                        (void*)static_cast<double (*)(double)>(asinh));

    // AST_FUNCTION_ARCTANH:
    executionEngine->addGlobalMapping(
            createGlobalMappingFunction("arctanh",
                    FunctionType::get(double_type, args_d1, false), module),
                        (void*)static_cast<double (*)(double)>(atanh));

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

    /// double asin(double x);
    createLibraryFunction(LibFunc::asin,
            FunctionType::get(double_type, args_d1, false), module);

    /// double atan(double x);
    createLibraryFunction(LibFunc::atan,
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

static Function* createGlobalMappingFunction(const char* funcName,
        llvm::FunctionType *funcType, Module *module)
{
    return Function::Create(funcType, Function::InternalLinkage, funcName, module);
}

static SBMLDocument *checkedReadSBMLFromString(const char* xml)
{
    SBMLDocument *doc = readSBMLFromString(xml);

    if (doc)
    {
        if (doc->getModel() == 0)
        {
            // fatal error
            SBMLErrorLog *log = doc->getErrorLog();
            string errors = log ? log->toString() : " NULL SBML Error Log";
            delete doc;
            throw_llvm_exception("Fatal SBML error, no model, errors in sbml document: " + errors);
        }
        else if (doc->getNumErrors() > 0)
        {
            SBMLErrorLog *log = doc->getErrorLog();
            string errors = log ? log->toString() : " NULL SBML Error Log";
            Log(rr::Logger::LOG_WARNING) << "Warning, errors found in sbml document: " + errors;
        }
    }
    else
    {
        delete doc;
        throw_llvm_exception("readSBMLFromString returned NULL, no further information available");
    }
    return doc;
}





} /* namespace rr */


