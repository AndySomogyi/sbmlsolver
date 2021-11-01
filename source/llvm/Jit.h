//
// Created by Ciaran on 25/10/2021.
//

#ifndef ROADRUNNER_JIT_H
#define ROADRUNNER_JIT_H


#include <llvm/Analysis/TargetLibraryInfo.h>
#include <rrSparse.h>
#include "llvm/IR/IRBuilder.h"
#include "LLVMException.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Object/ObjectFile.h"
#include "rrSparse.h"

namespace rr {
    class ExecutableModel;

}

namespace rrllvm {

    class ModelResources;

    template<typename FunctionPtrType>
    class CodeGenBase;


    /**
     * @brief define some function pointer signatures
     */

    using FnPtr_d1 = double (*)(double x);
    using FnPtr_i1 = int (*)(int x);
    using FnPtr_d2 = double (*)(double x, double y);

    // these are taken from C library by createCLibraryFunctions
    using powFnTy = FnPtr_d2;
    using fabsFnTy = FnPtr_d1;
    using acosFnTy = FnPtr_d1;
    using asinFnTy = FnPtr_d1;
    using atanFnTy = FnPtr_d1;
    using ceilFnTy = FnPtr_d1;
    using cosFnTy = FnPtr_d1;
    using coshFnTy = FnPtr_d1;
    using expFnTy = FnPtr_d1;
    using floorFnTy = FnPtr_d1;
    using logFnTy = FnPtr_d1;
    using log10FnTy = FnPtr_d1;
    using sinFnTy = FnPtr_d1;
    using sinhFnTy = FnPtr_d1;
    using tanFnTy = FnPtr_d1;
    using tanhFnTy = FnPtr_d1;
    using fmodFnTy = FnPtr_d2;

    // these are taken from libsbml in global mappings
    using arccotFnTy = FnPtr_d1;
    using rr_arccot_negzeroFnTy = FnPtr_d1;
    using arccothFnTy = FnPtr_d1;
    using arccscFnTy = FnPtr_d1;
    using arccschFnTy = FnPtr_d1;
    using arcsecFnTy = FnPtr_d1;
    using arcsechFnTy = FnPtr_d1;
    using cotFnTy = FnPtr_d1;
    using cothFnTy = FnPtr_d1;
    using cscFnTy = FnPtr_d1;
    using cschFnTy = FnPtr_d1;
    using rr_factorialiFnTy = FnPtr_i1;
    using rr_factorialdFnTy = FnPtr_d1;
    using rr_logdFnTy = FnPtr_d2;
    using rr_rootdFnTy = FnPtr_d2;
    using secFnTy = FnPtr_d1;
    using sechFnTy = FnPtr_d1;
    using arccoshFnTy = FnPtr_d1;
    using arcsinhFnTy = FnPtr_d1;
    using arctanhFnTy = FnPtr_d1;
    using quotientFnTy = FnPtr_d2;
    using rr_maxFnTy = FnPtr_d2;
    using rr_minFnTy = FnPtr_d2;

    using csr_matrix_set_nz_FnTy = rr::csr_matrix* (*)(int, int, double);
    using csr_matrix_get_nz_FnTy = rr::csr_matrix* (*)(int, int);

    class Jit {
    public:

        Jit(std::uint32_t options);

        /**
         * @brief adds functions that are declared and defined by libsbml
         * to the Jit engine.
         * @details i.e. tan, arccsh, quotient. In MCJit (llvm 6) these
         * were "mapped" to sbml support functions in addGlobalMappings. This API has changed in
         * later llvm's and so this functionality is made virtual.
         *
         * @seealso Jit::
         */
        virtual void mapFunctionsToJitSymbols() = 0;

        virtual std::uint64_t getFunctionAddress(const std::string &name) = 0;

//        virtual std::uint64_t getStructAddress(const std::string &name) = 0;

        virtual llvm::TargetMachine *getTargetMachine() = 0;

        virtual void addObjectFile(llvm::object::OwningBinary<llvm::object::ObjectFile> owningObject) = 0;

        /**
         * MCJit needs this but might be a deprecated api. We have to include it anyway.
         */
        virtual void finalizeObject() = 0;

        virtual const llvm::DataLayout &getDataLayout() = 0;

        virtual void addModule(llvm::Module *M) = 0;


        /**
         * *Moves* objects over to ModelResources ptr
         */
        virtual void transferObjectsToResources(std::shared_ptr<rrllvm::ModelResources> rc);


        virtual void mapFunctionsToAddresses(std::shared_ptr<ModelResources> &rc, std::uint32_t options);


        /**
         * @brief returns a non owning pointer to the llvm::Module instance
         */
        virtual llvm::Module *getModuleNonOwning();

        /**
         * @brief returns a non owning pointer to the llvm::LLVMContext instance
         */
        virtual llvm::LLVMContext *getContextNonOwning();

        /**
         * @brief returns a non owning pointer to the llvm::LLVMContext instance
         */
        virtual llvm::IRBuilder<> *getBuilderNonOwning();

        virtual std::string emitToString();

//        virtual void setTargetTriple(llvm::Triple triple);
//
//        virtual void setDataLayout(llvm::DataLayout dataLayout);


    protected:

        std::unique_ptr<llvm::LLVMContext> context;
        std::unique_ptr<llvm::Module> module;
        llvm::Module *moduleNonOwning = nullptr;
        std::unique_ptr<llvm::IRBuilder<>> builder;
//        llvm::Triple  triple;
//        llvm::DataLayout DataLayout;
        std::uint32_t options;

    };

}

#endif //ROADRUNNER_JIT_H
