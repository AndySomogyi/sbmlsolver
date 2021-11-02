//
// Created by Ciaran on 25/10/2021.
//

#ifndef ROADRUNNER_MCJIT_H
#define ROADRUNNER_MCJIT_H

#include <rrRoadRunnerOptions.h>
#include "Jit.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/IR/LegacyPassManager.h"

using namespace llvm;
using namespace rr;

namespace rrllvm {

    class MCJit : public Jit {
    public:

        explicit MCJit(std::uint32_t options);

        void mapFunctionsToJitSymbols() override;

        void mapDistribFunctionsToJitSymbols() override;

        void transferObjectsToResources(std::shared_ptr<rrllvm::ModelResources> rc) override;

        std::uint64_t lookupFunctionAddress(const std::string& name) override;

//        std::uint64_t getStructAddress(const string &name) override;

        llvm::TargetMachine *getTargetMachine() override ;

        void addObjectFile(llvm::object::OwningBinary<llvm::object::ObjectFile> owningObject) override;

        void finalizeObject() override;

        const llvm::DataLayout& getDataLayout() override;

        void addModule(llvm::Module* M) override;

        void addModule() override;

        void optimizeModule() override;

        void loadJittedFunctions() override;

        ExecutionEngine* getExecutionEngineNonOwning() const;

//        void setTargetTriple(llvm::Triple triple) override;
//
//        void setDataLayout(llvm::DataLayout dataLayout) override;

        llvm::legacy::FunctionPassManager *getFunctionPassManager() const ;



    private:
        Function *createGlobalMappingFunction(const char *funcName, llvm::FunctionType *funcType, Module *module);

        void addGlobalMapping(const llvm::GlobalValue *gv, void *addr);

        void addGlobalMappings();

        /**
         * @brief Add a function from the standard C library to the IR Module.
         * @example An example declaration is:
         *   declare double @pow(double, double)
         * @details the declaration is resolved with the standard C
         * library.
         * @code
                using powFn = double (*)(double x, double y);
                powFn pow = (powFn) executionEngine->getPointerToNamedFunction("pow");
                std::cout << pow(4, 2) << std::endl; // outputs 16
         * @endcode
         */
        void createCLibraryFunction(llvm::LibFunc funcId, llvm::FunctionType *funcType);

        /**
         * @brief Pull a collection of functions from the standard C library into the developing LLVM IR Module
         * @details The interface for pulling in the C library functions (seems to) work for both llvm-6
         * and llvm-13. Therefore it is implemented in the superclass of all Jit's.
         * The following functions are declared:
         *    - pow, fabs, acos, asin, atan, ceil, cos, cosh, exp,
         *      floor, log, log10, sin, sinh, tan, tanh, fmod
         * @see Jit::mapFunctionsToJitSymbols (which is made virtual because the interface has changed between
         * llvm-6 and llvm-13).
         */
        void createCLibraryFunctions();



        void initFunctionPassManager();

        /**
         * Docs say to stack allocate the EngineBuilder
         */
        EngineBuilder engineBuilder;
        std::unique_ptr<ExecutionEngine> executionEngine;
        std::unique_ptr<llvm::legacy::FunctionPassManager> functionPassManager;
        std::unique_ptr<std::string> errString;



    };

}

#endif //ROADRUNNER_MCJIT_H
