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

#ifdef _MSC_VER
#pragma warning(disable: 4146)
#pragma warning(disable: 4141)
#pragma warning(disable: 4267)
#pragma warning(disable: 4624)
#endif
#include "llvm/IR/DataLayout.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/DynamicLibrary.h"
//#include "llvm/ExecutionEngine/OrcMCJITReplacement.h"
#include "llvm/ExecutionEngine/SectionMemoryManager.h"


#include "llvm/ExecutionEngine/ExecutionEngine.h"

#if LLVM_VERSION_MAJOR >= 13
#include "llvm/ExecutionEngine/Orc/LLJIT.h"

#endif // LLVM_VERSION_MAJOR >= 13

#ifdef _MSC_VER
#pragma warning(default: 4146)
#pragma warning(default: 4141)
#pragma warning(default: 4267)
#pragma warning(default: 4624)
#endif

#include <sbml/SBMLReader.h>
#include <memory>
#include <string>
#include <vector>
#include <math.h>
#include <memory>

#include "ModelResources.h"

#include <sbml/common/libsbml-config-packages.h>
#ifdef LIBSBML_HAS_PACKAGE_DISTRIB
#include <sbml/packages/distrib/common/DistribExtensionTypes.h>
#endif

using namespace llvm;

using namespace libsbml;

using rr::Logger;
using rr::LoadSBMLOptions;

namespace rrllvm
{

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

ModelGeneratorContext::ModelGeneratorContext(std::string const &sbml, unsigned options, std::unique_ptr<Jit> jitEngine)
    :
        ownedDoc(0),
        doc(0),
        symbols(0),
        options(options),
        random(0),
        jit(std::move(jitEngine))
{
    if(useSymbolCache()) {
        rrLog(Logger::LOG_INFORMATION) << "Using LLVM symbol/value cache";
    } else {
        rrLog(Logger::LOG_INFORMATION) << "Not using LLVM symbol/value cache";
    }

    try
    {
        ownedDoc = checkedReadSBMLFromString(sbml.c_str());

        if (options & LoadSBMLOptions::CONSERVED_MOIETIES)
        {
            if ((rr::Config::getBool(rr::Config::ROADRUNNER_DISABLE_WARNINGS) &
                    rr::Config::ROADRUNNER_DISABLE_WARNINGS_CONSERVED_MOIETY) == 0)
            {
                rrLog(Logger::LOG_NOTICE) << "performing conserved moiety conversion";
            }

            // check if already conserved doc
            if (rr::conservation::ConservationExtension::isConservedMoietyDocument(ownedDoc))
            {
                doc = ownedDoc;
            }
            else
            {
                moietyConverter = std::make_unique<rr::conservation::ConservedMoietyConverter>();

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

                rrLog(Logger::LOG_INFORMATION) << "***************** Conserved Moiety Converted Document ***************";
                rrLog(Logger::LOG_INFORMATION) << convertedStr;
                rrLog(Logger::LOG_INFORMATION) << "*********************************************************************";

                free(convertedStr);
            }
        }
        else
        {
            doc = ownedDoc;
        }

        symbols = new LLVMModelDataSymbols(doc->getModel(), options);

        modelSymbols = std::make_unique<LLVMModelSymbols>(getModel(), *symbols);

        // initialize LLVM
        // Note - this initialization is duplicated but some tests (the CAPIModelEditingTests) break
        // without it.
        InitializeNativeTarget();
		InitializeNativeTargetAsmPrinter();
		InitializeNativeTargetAsmParser();

		// These were moved up here because they require the module ptr. May need to further edit these functions
//		createCLibraryFunctions(module);
		ModelDataIRBuilder::createModelDataStructType(jit->getModuleNonOwning(), nullptr, *symbols);


		// addGlobalMappings();

        // check if doc has distrib package
        // Random adds mappings, need call after llvm objs created
#ifdef LIBSBML_HAS_PACKAGE_DISTRIB
        const DistribSBMLDocumentPlugin* distrib =
                static_cast<const DistribSBMLDocumentPlugin*>(
                        doc->getPlugin("distrib"));
        if(distrib)
        {
//            random = new Random(*this);
        }
#endif

//        initFunctionPassManager();

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

ModelGeneratorContext::ModelGeneratorContext(libsbml::SBMLDocument const *_doc,
    unsigned options) :
        ownedDoc(0),
        doc(_doc),
        symbols(NULL),
        options(options),
        random(0)
{
    if(useSymbolCache()) {
        rrLog(Logger::LOG_INFORMATION) << "Using LLVM symbol/value cache";
    } else {
        rrLog(Logger::LOG_INFORMATION) << "Not using LLVM symbol/value cache";
    }

    try
    {
        if (options & LoadSBMLOptions::CONSERVED_MOIETIES)
        {
            rrLog(Logger::LOG_NOTICE) << "performing conserved moiety conversion";

            moietyConverter = std::make_unique<rr::conservation::ConservedMoietyConverter>();

            if (moietyConverter->setDocument(_doc) != LIBSBML_OPERATION_SUCCESS)
            {
                throw_llvm_exception("error setting conserved moiety converter document");
            }

            if (moietyConverter->convert() != LIBSBML_OPERATION_SUCCESS)
            {
                throw_llvm_exception("error converting document to conserved moieties");
            }

            this->doc = moietyConverter->getDocument();

            SBMLWriter sw;
            char* convertedStr = sw.writeToString(_doc);

            rrLog(Logger::LOG_INFORMATION) << "***************** Conserved Moiety Converted Document ***************";
            rrLog(Logger::LOG_INFORMATION) << convertedStr;
            rrLog(Logger::LOG_INFORMATION) << "*********************************************************************";

            delete convertedStr;
        }
        else
        {
            this->doc = _doc;
        }

        jit->addExternalFunctionsFromSBML();

        /**
         * this call has been moved to Jit constructor
         */
        // createCLibraryFunctions(module);

        symbols = new LLVMModelDataSymbols(doc->getModel(), options);

        modelSymbols = std::make_unique<LLVMModelSymbols>(getModel(), *symbols);

        ModelDataIRBuilder::createModelDataStructType(jit->getModuleNonOwning(), nullptr, *symbols);

        // check if _doc has distrib package
        // Random adds mappings, need call after llvm objs created
#ifdef LIBSBML_HAS_PACKAGE_DISTRIB
        const DistribSBMLDocumentPlugin* distrib =
                static_cast<const DistribSBMLDocumentPlugin*>(
                        _doc->getPlugin("distrib"));
        if(distrib)
        {
            // Random uses global mappings, which is a deprecated API. We need
            // to figure out what replaces the global mapping and work out a flexible
            // way to adjust Random.
            // For now, comment out.
//            random = new Random(*this);
        }
#endif

//        initFunctionPassManager(); // handled in MCJit

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
        options(0)
//        functionPassManager(0)
{
    // initialize LLVM
    // TODO check result
    InitializeNativeTarget();
    InitializeNativeTargetAsmPrinter();
    InitializeNativeTargetAsmParser();

//    context = std::unique_ptr<LLVMContext>();
//    // Make the module, which holds all the code.
//    module_owner = std::unique_ptr<Module>(new Module("LLVM Module", *context));
//	module = module_owner.get();
//
//    builder = std::make_unique<IRBuilder<>>(*context);
//
//    errString = new std::string();
//
//	addGlobalMappings();
//
//    EngineBuilder engineBuilder(std::move(module_owner));
//    //engineBuilder.setEngineKind(EngineKind::JIT);
//    engineBuilder.setErrorStr(errString);
//    executionEngine = std::unique_ptr<llvm::ExecutionEngine>(engineBuilder.create());

}

Random* ModelGeneratorContext::getRandom() const
{
    return random;
}

void ModelGeneratorContext::cleanup()
{
    delete ownedDoc; ownedDoc = 0;
//    delete errString; errString = 0;
}


ModelGeneratorContext::~ModelGeneratorContext()
{
    cleanup();
}

//llvm::LLVMContext &ModelGeneratorContext::getContext() const
//{
//    return *context;
//}
//
//llvm::ExecutionEngine &ModelGeneratorContext::getExecutionEngine() const
//{
//    return *executionEngine;
//}

const LLVMModelDataSymbols& ModelGeneratorContext::getModelDataSymbols() const
{
    return *symbols;
}

const libsbml::SBMLDocument* ModelGeneratorContext::getDocument() const
{
    return doc;
}


Jit* ModelGeneratorContext::getJitNonOwning() const {
    return jit.get();
}


const libsbml::Model* ModelGeneratorContext::getModel() const
{
  return doc->getModel();
}


//llvm::Module *ModelGeneratorContext::getModule() const
//{
//	return jit->getModuleNonOwning();
//}
//
//llvm::IRBuilder<> &ModelGeneratorContext::getBuilder() const
//{
//    return *jit->getBuilderNonOwning();
//}

void ModelGeneratorContext::transferObjectsToResources(std::shared_ptr<rrllvm::ModelResources> rc)
{
    rc->symbols = symbols;
    symbols = nullptr;

//    rc->random = random;
//    random = nullptr;
//
//    rc->context = std::move(context);
//    context = nullptr;
//    rc->executionEngine = std::move(executionEngine);
//    executionEngine = nullptr;
//
//	rc->errStr = errString;
//    errString = nullptr;
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



    static SBMLDocument *checkedReadSBMLFromString(const char* xml)
{
    SBMLDocument *doc = readSBMLFromString(xml);

    if (doc)
    {
        if (doc->getModel() == 0)
        {
            // fatal error
            SBMLErrorLog *log = doc->getErrorLog();
            std::string errors = log ? log->toString() : " NULL SBML Error Log";
            delete doc;
            throw_llvm_exception("Fatal SBML error, no model, errors in sbml document: " + errors);
        }
        else if (doc->getNumErrors() > 0)
        {
            SBMLErrorLog *log = doc->getErrorLog();
            std::string errors = log ? log->toString() : " NULL SBML Error Log";
            rrLog(rr::Logger::LOG_WARNING) << "Warning, errors found in sbml document: " + errors;
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


