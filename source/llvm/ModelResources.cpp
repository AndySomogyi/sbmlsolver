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

        rr::saveBinary(out, sbmlMD5);

        if (moduleStr.empty()) {
            std::unique_ptr<llvm::MemoryBuffer> memBuf = jit->getCompiledModelFromCache(sbmlMD5);
            MemoryBufferRef memBufRef = memBuf->getMemBufferRef();
            std::string moduleStr2 = memBufRef.getBuffer().str();
            if (moduleStr2.empty()){
                std::string err = "Cannot save module";
                rrLogErr << err;
                throw_llvm_exception(err);
            }
            rr::saveBinary(out, moduleStr2);
        } else {
            rr::saveBinary(out, moduleStr);
        }

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

        rr::loadBinary(in, sbmlMD5);
        assert(!sbmlMD5.empty() && "sbml md5 is empty");

        //Get the object file binary string from the input stream
        rr::loadBinary(in, moduleStr);


        /**
         * Roadrunner currently has two jit compilers, MCJit
         * which is the old API that's been used for years and LLJit, which is
         * the new OOTB prepackaged Jit engine from llvm. Save and load state
         */



        //Set up a buffer to read the object code from
        std::unique_ptr<llvm::MemoryBuffer> memBuffer(llvm::MemoryBuffer::getMemBuffer(moduleStr));
//
//        llvm::Expected<std::unique_ptr<llvm::object::ObjectFile> > objectFileExpected =
//                llvm::object::ObjectFile::createObjectFile(
//                        llvm::MemoryBufferRef(moduleStr, "id"));
//        if (!objectFileExpected) {
//            throw std::invalid_argument("Failed to load object data");
//        }
//        std::unique_ptr<llvm::object::ObjectFile> objectFile(std::move(objectFileExpected.get()));
//        llvm::object::OwningBinary<llvm::object::ObjectFile> owningObject(std::move(objectFile),
//
        jit->addObjectFile(std::move(memBuffer));
        jit->mapFunctionsToAddresses(this, modelGeneratorOpt);

    }


} /* namespace rrllvm */
