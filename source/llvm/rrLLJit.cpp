//
// Created by Ciaran on 25/10/2021.
//

#include "rrLLJit.h"
#include "rrLogger.h"
#include "llvm/IR/Function.h"
#include "SBMLSupportFunctions.h"
#include "llvm/ExecutionEngine/Orc/EPCDynamicLibrarySearchGenerator.h"
#include "ModelDataIRBuilder.h"

using namespace llvm;
using namespace rr;
using namespace sbmlsupport;

namespace rrllvm {

    rrLLJit::rrLLJit(std::uint32_t options)
            : Jit(options) {
        llvm::orc::LLJITBuilder llJitBuilder;
        auto llJitExpected = llJitBuilder.create();
        if (!llJitExpected) {
            std::string err = "failed to create an LLJit instance";
            rrLogErr << err;
            llvm::logAllUnhandledErrors(
                    std::move(llJitExpected.takeError()), llvm::errs(), "[LLJit creation failure] ");
        }
        // if success, grab the LLJit obj from the llvm::Expected object.
        llJit = std::move(*llJitExpected);

        // tell the LLJit instance to look in the current process
        // for symbols before complaining that they can't be found,
        auto DLSG = llvm::orc::DynamicLibrarySearchGenerator::GetForCurrentProcess(
                llJit->getDataLayout().getGlobalPrefix());
        if (!DLSG) {
            llvm::logAllUnhandledErrors(
                    std::move(DLSG.takeError()),
                    llvm::errs(),
                    "DynamicLibrarySearchGenerator not built successfully"
            );
        }
        llJit->getMainJITDylib().addGenerator(std::move(*DLSG));

        rrLLJit::mapFunctionsToJitSymbols();


    }

    void rrLLJit::mapFunctionsToJitSymbols() {

        mapFunctionToJitAbsoluteSymbol("rr_csr_matrix_get_nz", reinterpret_cast<std::uint64_t>(&rr::csr_matrix_get_nz));
        mapFunctionToJitAbsoluteSymbol("rr_csr_matrix_set_nz", reinterpret_cast<std::uint64_t>(&rr::csr_matrix_set_nz));
        mapFunctionToJitAbsoluteSymbol("arccot", reinterpret_cast<std::uint64_t>(&sbmlsupport::arccot));
        mapFunctionToJitAbsoluteSymbol("rr_arccot_negzero", reinterpret_cast<std::uint64_t>(&sbmlsupport::arccot_negzero));
        mapFunctionToJitAbsoluteSymbol("arccoth", reinterpret_cast<std::uint64_t>(&sbmlsupport::arccoth));
        mapFunctionToJitAbsoluteSymbol("arccsc", reinterpret_cast<std::uint64_t>(&sbmlsupport::arccsc));
        mapFunctionToJitAbsoluteSymbol("arccsch", reinterpret_cast<std::uint64_t>(&sbmlsupport::arccsch));
        mapFunctionToJitAbsoluteSymbol("arcsec", reinterpret_cast<std::uint64_t>(&sbmlsupport::arcsec));
        mapFunctionToJitAbsoluteSymbol("arcsech", reinterpret_cast<std::uint64_t>(&sbmlsupport::arcsech));
        mapFunctionToJitAbsoluteSymbol("cot", reinterpret_cast<std::uint64_t>(&sbmlsupport::cot));
        mapFunctionToJitAbsoluteSymbol("coth", reinterpret_cast<std::uint64_t>(&sbmlsupport::coth));
        mapFunctionToJitAbsoluteSymbol("csc", reinterpret_cast<std::uint64_t>(&sbmlsupport::csc));
        mapFunctionToJitAbsoluteSymbol("csch", reinterpret_cast<std::uint64_t>(&sbmlsupport::csch));
        mapFunctionToJitAbsoluteSymbol("rr_factoriali", reinterpret_cast<std::uint64_t>(&sbmlsupport::factoriali));
        mapFunctionToJitAbsoluteSymbol("rr_factoriald", reinterpret_cast<std::uint64_t>(&sbmlsupport::factoriald));
        mapFunctionToJitAbsoluteSymbol("rr_logd", reinterpret_cast<std::uint64_t>(&sbmlsupport::logd));
        mapFunctionToJitAbsoluteSymbol("rr_rootd", reinterpret_cast<std::uint64_t>(&sbmlsupport::rootd));
        mapFunctionToJitAbsoluteSymbol("sec", reinterpret_cast<std::uint64_t>(&sbmlsupport::sec));
        mapFunctionToJitAbsoluteSymbol("sech", reinterpret_cast<std::uint64_t>(&sbmlsupport::sech));
        mapFunctionToJitAbsoluteSymbol("arccosh", reinterpret_cast<std::uint64_t>(&sbmlsupport::arccosh));
        mapFunctionToJitAbsoluteSymbol("arcsinh", reinterpret_cast<std::uint64_t>(&sbmlsupport::arcsinh));
        mapFunctionToJitAbsoluteSymbol("arctanh", reinterpret_cast<std::uint64_t>(&sbmlsupport::arctanh));
        mapFunctionToJitAbsoluteSymbol("quotient", reinterpret_cast<std::uint64_t>(&sbmlsupport::quotient));
        mapFunctionToJitAbsoluteSymbol("rr_max", reinterpret_cast<std::uint64_t>(&sbmlsupport::max));
        mapFunctionToJitAbsoluteSymbol("rr_min", reinterpret_cast<std::uint64_t>(&sbmlsupport::min));

    }

    std::uint64_t rrLLJit::getFunctionAddress(const std::string &name) {
        auto expectedSymbol = llJit->lookup(name);
        if (!expectedSymbol) {
            std::string err = "Could not find symbol " + name;
            rrLogErr << err;
            llvm::logAllUnhandledErrors(
                    std::move(expectedSymbol.takeError()),
                    llvm::errs(),
                    "[symbol lookup error] ");
        }
        llvm::JITEvaluatedSymbol symbol = *expectedSymbol;
        return symbol.getAddress();
    }

    llvm::TargetMachine *rrLLJit::getTargetMachine() {
        // I can't seem to find a handle to TargetMachine
        // from LLJit
        return nullptr;
    }

    void rrLLJit::addObjectFile(llvm::object::OwningBinary<llvm::object::ObjectFile> owningObject) {

//        llJit->addObjectFile(owningObject);
    }

    void rrLLJit::finalizeObject() {

    }

    const llvm::DataLayout &rrLLJit::getDataLayout() {
        return llJit->getDataLayout();
    }

    void rrLLJit::addModule(llvm::orc::ThreadSafeModule tsm) {
        llJit->addIRModule(std::move(tsm));
    }

    void rrLLJit::addModule(llvm::Module* m) {
//        llJit->addIRModule(m);
    }

    void rrLLJit::addModule() {

    }

    void rrLLJit::optimizeModule() {

    }

    llvm::orc::LLJIT *rrLLJit::getLLJitNonOwning() {
        return llJit.get();
    }

    void rrLLJit::addIRModule() {
        llvm::orc::ThreadSafeModule tsm(std::move(module), std::move(context));
        if (llvm::Error err = llJit->addIRModule(std::move(tsm))) {
            std::string errMsg = "Could not add module to LLJit";
            rrLogErr << errMsg;
            llvm::logAllUnhandledErrors(std::move(err), llvm::errs(), errMsg);
        }
    }

    /**
     * @brief makes a function available in the Jit'd address
     * space.
     * @details for instance, there might be a function called
     * Foo in the main roadrunner program. This function
     * produces a mapping between Foo and a name so that the
     * function can be used from Jit.
     * While this strategy works, it is not recommended and should
     * be deleted. This is because addresses are taken from a specific
     * session/runtime and change. Therefore, symbols mapped this way
     * cannot be cached. It is a good exercise, and a good way to
     * get started with mapping functions to jit.
     */
    void rrLLJit::mapFunctionToJitAbsoluteSymbol(const std::string& funcName, std::uint64_t funcAddress){

        auto symbolStringPool = llJit->getExecutionSession().getExecutorProcessControl().getSymbolStringPool();
        orc::SymbolStringPtr symbPtr = symbolStringPool->intern(funcName);

        // JITTargetAddress is uint64 typedefd
        llvm::JITSymbolFlags flg;
        llvm::JITEvaluatedSymbol symb(funcAddress, flg);
        if (llvm::Error err = llJit->getMainJITDylib().define(
                llvm::orc::absoluteSymbols({{symbPtr, symb}}))){
            llvm::logAllUnhandledErrors(std::move(err), llvm::errs(), "Could not add symbol "+ funcName);
        }
    }



}