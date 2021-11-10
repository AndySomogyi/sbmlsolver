/*
 * CachedModel.cpp
 *
 *  Created on: Aug 28, 2013
 *      Author: andy
 */
#pragma hdrstop

#include "ModelResources.h"
#include "Random.h"

#include <rrLogger.h>
#include <rrStringUtils.h>
#include "rrRoadRunnerOptions.h"
#include "MCJit.h"
#include "LLJit.h"

#include <memory>

#undef min
#undef max

#include "llvm/SBMLSupportFunctions.h"
#include "rrRoadRunnerOptions.h"


#ifdef _MSC_VER
#pragma warning(disable: 4146)
#pragma warning(disable: 4141)
#pragma warning(disable: 4267)
#pragma warning(disable: 4624)
#endif

#include "llvm/Bitcode/BitcodeWriter.h"
#include "llvm/Bitcode/BitcodeReader.h"
#include "llvm/Support/Error.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/ExecutionEngine/SectionMemoryManager.h"
#include "llvm/Support/DynamicLibrary.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Target/TargetMachine.h"

#ifdef _MSC_VER
#pragma warning(default: 4146)
#pragma warning(default: 4141)
#pragma warning(default: 4267)
#pragma warning(default: 4624)
#endif

using rr::Logger;
using rr::getLogger;
using rr::LoadSBMLOptions;

namespace rrllvm {

    ModelResources::ModelResources() :
            symbols(nullptr), executionEngine(nullptr), context(nullptr), random(nullptr), errStr(nullptr) {
        // the reset of the ivars are assigned by the generator,
        // and in an exception they are not, does not matter as
        // we don't have to delete them.
    }

    ModelResources::~ModelResources() {
        rrLog(Logger::LOG_DEBUG) << __FUNC__;

        if (errStr && !errStr->empty()) {
            rrLog(Logger::LOG_WARNING) << "Non-empty LLVM ExecutionEngine error std::string: " << *errStr;
        }

        delete symbols;

        // the exe engine owns all the functions
//    delete executionEngine;
//    delete context;
        delete random;
    }

    void ModelResources::saveState(std::ostream &out) const {
        symbols->saveState(out);

        rr::saveBinary(out, moduleStr);
    }

    void ModelResources::addGlobalMapping(std::string name, void *addr) {
        llvm::sys::DynamicLibrary::AddSymbol(name, addr);
    }

    void ModelResources::addGlobalMappings() {
        using namespace llvm;
        Type *double_type = Type::getDoubleTy(*context);
        Type *args_d2[] = {double_type, double_type};

        llvm::sys::DynamicLibrary::LoadLibraryPermanently(nullptr);

        addGlobalMapping("rr_csr_matrix_set_nz", (void *) rr::csr_matrix_set_nz);

        addGlobalMapping("rr_csr_matrix_get_nz", (void *) rr::csr_matrix_get_nz);

        // AST_FUNCTION_ARCCOT:
        llvm::RTDyldMemoryManager::getSymbolAddressInProcess("arccot");
        addGlobalMapping(
                "arccot",
                (void *) sbmlsupport::arccot);

        addGlobalMapping(
                "rr_arccot_negzero",
                (void *) sbmlsupport::arccot_negzero);

        // AST_FUNCTION_ARCCOTH:
        addGlobalMapping(
                "arccoth",
                (void *) sbmlsupport::arccoth);

        // AST_FUNCTION_ARCCSC:
        addGlobalMapping(
                "arccsc",
                (void *) sbmlsupport::arccsc);

        // AST_FUNCTION_ARCCSCH:
        addGlobalMapping(
                "arccsch",
                (void *) sbmlsupport::arccsch);

        // AST_FUNCTION_ARCSEC:
        addGlobalMapping(
                "arcsec",
                (void *) sbmlsupport::arcsec);

        // AST_FUNCTION_ARCSECH:
        addGlobalMapping(
                "arcsech",
                (void *) sbmlsupport::arcsech);

        // AST_FUNCTION_COT:
        addGlobalMapping(
                "cot",
                (void *) sbmlsupport::cot);

        // AST_FUNCTION_COTH:
        addGlobalMapping(
                "coth",
                (void *) sbmlsupport::coth);

        // AST_FUNCTION_CSC:
        addGlobalMapping(
                "csc",
                (void *) sbmlsupport::csc);

        // AST_FUNCTION_CSCH:
        addGlobalMapping(
                "csch",
                (void *) sbmlsupport::csch);

        // AST_FUNCTION_FACTORIAL:
        addGlobalMapping(
                "rr_factoriali",
                (void *) sbmlsupport::factoriali);

        addGlobalMapping(
                "rr_factoriald",
                (void *) sbmlsupport::factoriald);

        // AST_FUNCTION_LOG:
        addGlobalMapping(
                "rr_logd",
                (void *) sbmlsupport::logd);

        // AST_FUNCTION_ROOT:
        addGlobalMapping(
                "rr_rootd",
                (void *) sbmlsupport::rootd);

        // AST_FUNCTION_SEC:
        addGlobalMapping(
                "sec",
                (void *) sbmlsupport::sec);

        // AST_FUNCTION_SECH:
        addGlobalMapping(
                "sech",
                (void *) sbmlsupport::sech);

        // AST_FUNCTION_ARCCOSH:
        addGlobalMapping(
                "arccosh",
                (void *) static_cast<double (*)(double)>(acosh));

        // AST_FUNCTION_ARCSINH:
        addGlobalMapping(
                "arcsinh",
                (void *) static_cast<double (*)(double)>(asinh));

        // AST_FUNCTION_ARCTANH:
        addGlobalMapping(
                "arctanh",
                (void *) static_cast<double (*)(double)>(atanh));

        // AST_FUNCTION_QUOTIENT:
        addGlobalMapping("quotient", (void *) sbmlsupport::quotient);
        // AST_FUNCTION_MAX:
        addGlobalMapping("rr_max", (void *) sbmlsupport::max);
        // AST_FUNCTION_MIN:
        addGlobalMapping("rr_min", (void *) sbmlsupport::min);
    }


    void ModelResources::loadState(std::istream &in, uint modelGeneratorOpt) {
        // todo do we need the ModelGeneratorContext? Seems like we only need a Jit.

        if (modelGeneratorOpt & LoadSBMLOptions::MCJIT) {
            jit = std::move(std::make_unique<MCJit>(modelGeneratorOpt));
        }

#if LLVM_VERSION_MAJOR >= 13
            else if (modelGeneratorOpt & LoadSBMLOptions::LLJIT) {
                jit = std::move(std::make_unique<LLJit>(modelGeneratorOpt));
            }

#endif // LLVM_VERSION_MAJOR >= 13

        else {
            rrLogWarn << "Requested compiler not found. Defaulting to MCJit.";
            jit = std::move(std::make_unique<MCJit>(modelGeneratorOpt));
        }

        // get rid of sumbols, if not nullptr.
        // todo make symbols a unique_ptr
        delete symbols;
        //load the model data symbols from the stream
        symbols = new LLVMModelDataSymbols(in);

        //Get the object file from the input stream
        rr::loadBinary(in, moduleStr);
        rrLogCriticalCiaran << "commented out check for empty module string";
        if (moduleStr.empty()) {
            std::string err = "Cannot load state because the roadrunner object that should be stored as a string "
                              "is empty";
            rrLogErr << err;
            throw_llvm_exception(err);
        }
        //Set up the llvm context
//        context = std::make_unique<llvm::LLVMContext>();
//        //Set up a buffer to read the object code from
        auto memBuffer(llvm::MemoryBuffer::getMemBuffer(moduleStr));

        llvm::Expected<std::unique_ptr<llvm::object::ObjectFile> > objectFileExpected =
                llvm::object::ObjectFile::createObjectFile(
                        llvm::MemoryBufferRef(moduleStr, "id"));
        if (!objectFileExpected) {
            throw std::invalid_argument("Failed to load object data");
        }

        std::unique_ptr<llvm::object::ObjectFile> objectFile(std::move(objectFileExpected.get()));

        llvm::object::OwningBinary<llvm::object::ObjectFile> owningObject(std::move(objectFile), std::move(memBuffer));

        jit->addObjectFile(std::move(owningObject));
        jit->mapFunctionsToAddresses(this, modelGeneratorOpt);
    }


} /* namespace rrllvm */
