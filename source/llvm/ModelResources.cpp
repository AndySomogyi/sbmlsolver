/*
 * CachedModel.cpp
 *
 *  Created on: Aug 28, 2013
 *      Author: andy
 */
#pragma hdrstop

#include "ModelResources.h"
#include "Random.h"
#include "JitFactory.h"
#include <rrLogger.h>
#include <rrStringUtils.h>
#include "rrRoadRunnerOptions.h"
#include "MCJit.h"
#include "LLJit.h"
#include "llvm/SBMLSupportFunctions.h"
#include "rrRoadRunnerOptions.h"

#include <memory>

#undef min
#undef max


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

        std::string moduleAsString = jit->getModuleAsString(sbmlMD5);
        rr::saveBinary(out, moduleAsString);

        // MCJit uses this variable. LLJit does not.
        bool usingCompiledModuleBinaryStream = false;

        // see comment in loadState
        if (jit->compiledModuleBinaryStream) {
            usingCompiledModuleBinaryStream = true;
        }
        rr::saveBinary(out, usingCompiledModuleBinaryStream);

        if (usingCompiledModuleBinaryStream) {
            rr::saveBinary(out, jit->compiledModuleBinaryStream.get());
        }

    }

    void ModelResources::loadState(std::istream &in, uint modelGeneratorOpt) {

        jit = std::move(JitFactory::makeJitEngine(modelGeneratorOpt));

        // get rid of sumbols, if not nullptr.
        // todo make symbols a unique_ptr
        delete symbols;
        //load the model data symbols from the stream
        symbols = new LLVMModelDataSymbols(in);

        rr::loadBinary(in, sbmlMD5);
        assert(!sbmlMD5.empty() && "sbml md5 is empty");

        rr::loadBinary(in, moduleStr);

        // flag which determines whether we are using
        // compiledModuleBinaryStream as moduleStr storage (MCJit)
        // or whether this is automatically cached by LLVM (LLJit)
        // meaning we can directly access what we need from the cache.
        bool usingCompiledModuleBinaryStream = false;
        rr::loadBinary(in, usingCompiledModuleBinaryStream);


        /**
         * Although we have a LLVM IR module as binary string (moduleStr)
         * and this is all that is needed to create another model from the
         * input stream, we still need to have a populated
         * jit->compiledModuleBinaryStream variable, otherwise a copy of
         * a roadrunner instance cannot saveState.
         */
        if (usingCompiledModuleBinaryStream) {
            //Get the object file binary string from the input stream
            rr::loadBinary(in, jit->compiledModuleBinaryStream.get());
        }

        //Set up a buffer to read the object code from
        std::unique_ptr<llvm::MemoryBuffer> memBuffer(llvm::MemoryBuffer::getMemBuffer(moduleStr));

        llvm::Expected<std::unique_ptr<llvm::object::ObjectFile> > objFile =
                llvm::object::ObjectFile::createObjectFile(
                        llvm::MemoryBufferRef(moduleStr, sbmlMD5));
        if (!objFile) {
            std::string err = "Failed to load object data";
            rrLogErr << err;
            llvm::logAllUnhandledErrors(objFile.takeError(), llvm::errs(), err);
        }
        llvm::object::OwningBinary<llvm::object::ObjectFile> owningObject(
                std::move(*objFile), std::move(memBuffer));

        jit->addObjectFile(std::move(owningObject));
        jit->mapLLVMGeneratedFunctionsToSymbols(this, modelGeneratorOpt);

    }


} /* namespace rrllvm */
