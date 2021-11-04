//
// Created by Ciaran on 25/10/2021.
//

#ifndef ROADRUNNER_LLJIT_H
#define ROADRUNNER_LLJIT_H

#define NOMINMAX

#include <llvm/IRReader/IRReader.h>
#include "llvm/Jit.h"
#include "llvm/ExecutionEngine/Orc/LLJIT.h"
#include "llvm/Support/SourceMgr.h"


using namespace rr;

namespace rrllvm {
    inline llvm::Error createSMDiagnosticError(llvm::SMDiagnostic &Diag) {
        using namespace llvm;
        std::string Msg;
        {
            raw_string_ostream OS(Msg);
            Diag.print("", OS);
        }
        return make_error<StringError>(std::move(Msg), inconvertibleErrorCode());
    }

    inline llvm::Expected<llvm::orc::ThreadSafeModule>
    parseModule(llvm::StringRef Source, llvm::StringRef Name) {
        using namespace llvm;
        auto Ctx = std::make_unique<LLVMContext>();
        SMDiagnostic Err;
        if (auto M = parseIR(MemoryBufferRef(Source, Name), Err, *Ctx))
            return orc::ThreadSafeModule(std::move(M), std::move(Ctx));

        return createSMDiagnosticError(Err);
    }

    /**
     * @brief Thin layer around the llvm::orc::LLJit.
     *
     * @details The interface for LLJit is used as a framework
     */
    class LLJit : public Jit {
    public:

        LLJit() = default;

        explicit LLJit(std::uint32_t options);

        void mapFunctionsToJitSymbols() override;

        void mapDistribFunctionsToJitSymbols() override;

        std::uint64_t lookupFunctionAddress(const std::string &name) override;

        void addObjectFile(llvm::object::OwningBinary<llvm::object::ObjectFile> owningObject) override;

        const llvm::DataLayout &getDataLayout() override;

        void addModule(llvm::Module *M) override;

        void addModule() override;

        void addModuleViaObjectFile() override;

        void addModule(llvm::orc::ThreadSafeModule tsm);

        void addModule(std::unique_ptr<llvm::Module> M, std::unique_ptr<llvm::LLVMContext> ctx) override;

        llvm::orc::LLJIT *getLLJitNonOwning();

        void addIRModule();

        /**
         * @brief prints the symbol tables currently in the jit.
         */
        std::string dump();

        friend std::ostream& operator<<(std::ostream& os, LLJit* llJit);

    private:
        void mapFunctionToJitAbsoluteSymbol(const std::string &funcName, std::uint64_t funcAddress);

        std::unique_ptr<llvm::orc::LLJIT> llJit;

    };

}

#endif //ROADRUNNER_LLJIT_H
