//
// Created by Ciaran on 25/10/2021.
//

#ifndef ROADRUNNER_MCJIT_H
#define ROADRUNNER_MCJIT_H

#include "Jit.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/IR/LegacyPassManager.h"

using namespace llvm;

namespace rrllvm {

    class MCJit : public Jit {
    public:

        explicit MCJit(std::uint32_t options);

        void addSupportFunctions() override;

        void transferObjectsToResources(std::shared_ptr<rrllvm::ModelResources> rc) override;

        std::uint64_t getFunctionAddress(const std::string& name) override;

        llvm::TargetMachine *getTargetMachine() override ;

        void addObjectFile(llvm::object::OwningBinary<llvm::object::ObjectFile> owningObject) override;

        void finalizeObject() override;

        const llvm::DataLayout& getDataLayout() override;

        void addModule(llvm::Module* M) override;

        ExecutionEngine* getExecutionEngineNonOwning() const;

//        void setTargetTriple(llvm::Triple triple) override;
//
//        void setDataLayout(llvm::DataLayout dataLayout) override;

    private:
        Function *createGlobalMappingFunction(const char *funcName, llvm::FunctionType *funcType, Module *module);

        void addGlobalMapping(const llvm::GlobalValue *gv, void *addr);

        void addGlobalMappings();

        llvm::legacy::FunctionPassManager *getFunctionPassManager() const ;

        void initFunctionPassManager();


        /**
         * Docs say to stack allocate the EngineBuilder
         */
        EngineBuilder engineBuilder;

        std::unique_ptr<ExecutionEngine> executionEngine;
        std::unique_ptr<llvm::legacy::FunctionPassManager> functionPassManager;
        std::unique_ptr<std::string> errString;
        std::uint32_t options;



    };

}

#endif //ROADRUNNER_MCJIT_H
