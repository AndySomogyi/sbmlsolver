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

        rr::saveBinary(out, jit->getModuleAsString(sbmlMD5));

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

        //Get the object file binary string from the input stream
        rr::loadBinary(in, moduleStr);

        //Set up a buffer to read the object code from
        std::unique_ptr<llvm::MemoryBuffer> memBuffer(llvm::MemoryBuffer::getMemBuffer(moduleStr));

        llvm::Expected<std::unique_ptr<llvm::object::ObjectFile> > objFile =
                llvm::object::ObjectFile::createObjectFile(
                        llvm::MemoryBufferRef(moduleStr, "id"));
        if (!objFile){
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
