//
// Created by Ciaran on 25/10/2021.
//

#ifndef ROADRUNNER_RRLLJIT_H
#define ROADRUNNER_RRLLJIT_H

#define NOMINMAX
#include "llvm/Jit.h"
#include "llvm/ExecutionEngine/Orc/LLJIT.h"


using namespace rr;

namespace rrllvm {

    /**
     * @brief Thin layer around the llvm::orc::LLJit.
     *
     * @details The interface for LLJit is used as a framework
     */
    class rrLLJit : public Jit{
    public:

        rrLLJit() = default;

        explicit rrLLJit(std::uint32_t options);

        void mapFunctionsToJitSymbols() override;

        void mapDistribFunctionsToJitSymbols() override;


        std::uint64_t getFunctionAddress(const std::string &name) override;

        llvm::TargetMachine *getTargetMachine() override;

        void addObjectFile(llvm::object::OwningBinary<llvm::object::ObjectFile> owningObject) override;

        void finalizeObject() override;

        const llvm::DataLayout &getDataLayout() override;

        void addModule(llvm::Module *M) override;

        void addModule() override;

        void optimizeModule() override;

        void loadJittedFunctions() override;

        void addModule(llvm::orc::ThreadSafeModule tsm);

        llvm::orc::LLJIT* getLLJitNonOwning() ;

        void addIRModule();

    private:
        void mapFunctionToJitAbsoluteSymbol(const std::string& funcName, std::uint64_t funcAddress);

        std::unique_ptr<llvm::orc::LLJIT> llJit;

    };

}

#endif //ROADRUNNER_RRLLJIT_H
