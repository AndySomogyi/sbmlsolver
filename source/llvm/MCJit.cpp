//
// Created by Ciaran on 25/10/2021.
//

#include <llvm/ExecutionEngine/SectionMemoryManager.h>
#include "llvm/Support/DynamicLibrary.h"
#include "MCJit.h"
#include "ModelDataIRBuilder.h"
#include "SBMLSupportFunctions.h"
#include "rrRoadRunnerOptions.h"
#include "rrLogger.h"
#include "ModelResources.h"
#include "llvm/IR/AssemblyAnnotationWriter.h"

#ifdef LIBSBML_HAS_PACKAGE_DISTRIB
#   include "Random.h"
#endif

using namespace rr;

namespace rrllvm {

    /**
     * @brief cross platform mechanism for getting the target machine
     * file type.
     */
#if LLVM_VERSION_MAJOR == 6
    llvm::LLVMTargetMachine::CodeGenFileType getCodeGenFileType(){
        return llvm::TargetMachine::CGFT_ObjectFile;
    }
#elif LLVM_VERSION_MAJOR >= 12

    llvm::CodeGenFileType getCodeGenFileType() {
        return llvm::CGFT_ObjectFile;
    }

#endif


    MCJit::MCJit(std::uint32_t opt)
            : Jit(opt),
              engineBuilder(EngineBuilder(std::move(module))){

        engineBuilder
                .setErrorStr(errString.get())
                .setMCJITMemoryManager(std::make_unique<SectionMemoryManager>());
        executionEngine = std::unique_ptr<ExecutionEngine>(engineBuilder.create());
        MCJit::addGlobalMappings();
        MCJit::createCLibraryFunctions();
        MCJit::mapDistribFunctionsToJitSymbols();
        MCJit::initFunctionPassManager();
    }


    ExecutionEngine *MCJit::getExecutionEngineNonOwning() const {
        return executionEngine.get();
    }

    void MCJit::mapFunctionsToJitSymbols() {
        addGlobalMappings();
    }

    void MCJit::mapDistribFunctionsToJitSymbols() {
//        LLVMContext &context = *.getJitNonOwning()->getContextNonOwning();
//    llvm::ExecutionEngine *executionEngine = &ctx.getExecutionEngine();
        Type *double_type = Type::getDoubleTy(*context);


        // LLVM does not appear to have a true void ptr, so just use a pointer
        // to a byte, pointers are all the same size anyway.
        // used for the LLVMModelData::random which is not accessed by
        // generated llvm code anyway.
        // see also, llvm::StructType *ModelDataIRBuilder::createModelDataStructType(...)
        Type *voidPtrType = Type::getInt8PtrTy(*context);

        Type *args_void_double_quadruple[] = {voidPtrType, double_type, double_type, double_type, double_type};

        Type *args_void_double_triple[] = {voidPtrType, double_type, double_type, double_type};

        Type *args_void_double_double[] = {voidPtrType, double_type, double_type};

        Type *args_void_double[] = {voidPtrType, double_type};


        executionEngine->addGlobalMapping(
                createGlobalMappingFunction("rr_distrib_uniform",
                                            FunctionType::get(double_type, args_void_double_double, false), moduleNonOwning),
                (void *) distrib_uniform);

        executionEngine->addGlobalMapping(
                createGlobalMappingFunction("rr_distrib_normal",
                                            FunctionType::get(double_type, args_void_double_double, false), moduleNonOwning),
                (void *) distrib_normal);

        executionEngine->addGlobalMapping(
                createGlobalMappingFunction("rr_distrib_normal_four",
                                            FunctionType::get(double_type, args_void_double_quadruple, false), moduleNonOwning),
                (void *) distrib_normal_four);

        executionEngine->addGlobalMapping(
                createGlobalMappingFunction("rr_distrib_binomial",
                                            FunctionType::get(double_type, args_void_double_double, false), moduleNonOwning),
                (void *) distrib_binomial);

        executionEngine->addGlobalMapping(
                createGlobalMappingFunction("rr_distrib_bernoulli",
                                            FunctionType::get(double_type, args_void_double, false), moduleNonOwning),
                (void *) distrib_bernoulli);

        executionEngine->addGlobalMapping(
                createGlobalMappingFunction("rr_distrib_binomial_four",
                                            FunctionType::get(double_type, args_void_double_quadruple, false), moduleNonOwning),
                (void *) distrib_binomial_four);

        executionEngine->addGlobalMapping(
                createGlobalMappingFunction("rr_distrib_cauchy",
                                            FunctionType::get(double_type, args_void_double_double, false), moduleNonOwning),
                (void *) distrib_cauchy);

        executionEngine->addGlobalMapping(
                createGlobalMappingFunction("rr_distrib_cauchy_one",
                                            FunctionType::get(double_type, args_void_double, false), moduleNonOwning),
                (void *) distrib_cauchy_one);

        executionEngine->addGlobalMapping(
                createGlobalMappingFunction("rr_distrib_cauchy_four",
                                            FunctionType::get(double_type, args_void_double_quadruple, false), moduleNonOwning),
                (void *) distrib_cauchy_four);

        executionEngine->addGlobalMapping(
                createGlobalMappingFunction("rr_distrib_chisquare",
                                            FunctionType::get(double_type, args_void_double, false), moduleNonOwning),
                (void *) distrib_chisquare);

        executionEngine->addGlobalMapping(
                createGlobalMappingFunction("rr_distrib_chisquare_three",
                                            FunctionType::get(double_type, args_void_double_triple, false), moduleNonOwning),
                (void *) distrib_chisquare_three);

        executionEngine->addGlobalMapping(
                createGlobalMappingFunction("rr_distrib_exponential",
                                            FunctionType::get(double_type, args_void_double, false), moduleNonOwning),
                (void *) distrib_exponential);

        executionEngine->addGlobalMapping(
                createGlobalMappingFunction("rr_distrib_exponential_three",
                                            FunctionType::get(double_type, args_void_double_triple, false), moduleNonOwning),
                (void *) distrib_exponential_three);

        executionEngine->addGlobalMapping(
                createGlobalMappingFunction("rr_distrib_gamma",
                                            FunctionType::get(double_type, args_void_double_double, false), moduleNonOwning),
                (void *) distrib_gamma);

        executionEngine->addGlobalMapping(
                createGlobalMappingFunction("rr_distrib_gamma_four",
                                            FunctionType::get(double_type, args_void_double_quadruple, false), moduleNonOwning),
                (void *) distrib_gamma_four);

        executionEngine->addGlobalMapping(
                createGlobalMappingFunction("rr_distrib_laplace",
                                            FunctionType::get(double_type, args_void_double_double, false), moduleNonOwning),
                (void *) distrib_laplace);

        executionEngine->addGlobalMapping(
                createGlobalMappingFunction("rr_distrib_laplace_one",
                                            FunctionType::get(double_type, args_void_double, false), moduleNonOwning),
                (void *) distrib_laplace_one);

        executionEngine->addGlobalMapping(
                createGlobalMappingFunction("rr_distrib_laplace_four",
                                            FunctionType::get(double_type, args_void_double_quadruple, false), moduleNonOwning),
                (void *) distrib_laplace_four);

        executionEngine->addGlobalMapping(
                createGlobalMappingFunction("rr_distrib_lognormal",
                                            FunctionType::get(double_type, args_void_double_double, false), moduleNonOwning),
                (void *) distrib_lognormal);

        executionEngine->addGlobalMapping(
                createGlobalMappingFunction("rr_distrib_lognormal_four",
                                            FunctionType::get(double_type, args_void_double_quadruple, false), moduleNonOwning),
                (void *) distrib_lognormal_four);

        executionEngine->addGlobalMapping(
                createGlobalMappingFunction("rr_distrib_poisson",
                                            FunctionType::get(double_type, args_void_double, false), moduleNonOwning),
                (void *) distrib_poisson);

        executionEngine->addGlobalMapping(
                createGlobalMappingFunction("rr_distrib_poisson_three",
                                            FunctionType::get(double_type, args_void_double_triple, false), moduleNonOwning),
                (void *) distrib_poisson_three);

        executionEngine->addGlobalMapping(
                createGlobalMappingFunction("rr_distrib_rayleigh",
                                            FunctionType::get(double_type, args_void_double, false), moduleNonOwning),
                (void *) distrib_rayleigh);

        executionEngine->addGlobalMapping(
                createGlobalMappingFunction("rr_distrib_rayleigh_three",
                                            FunctionType::get(double_type, args_void_double_triple, false), moduleNonOwning),
                (void *) distrib_rayleigh_three);

    };

    void MCJit::transferObjectsToResources(std::shared_ptr<rrllvm::ModelResources> rc) {
        Jit::transferObjectsToResources(rc);
        // todo check whether we break stuff by not giving the execution engine to rc
        rc->executionEngine = std::move(executionEngine);
        rc->executionEngine = nullptr;
        rc->errStr = std::move(errString);
        errString = nullptr;

    }

    std::uint64_t MCJit::lookupFunctionAddress(const std::string &name) {
        return (std::uint64_t) executionEngine->getPointerToNamedFunction(name);
    }

//    std::uint64_t MCJit::getStructAddress(const std::string &name) {
//        return (std::uint64_t) executionEngine->getPointerToGlobalIfAvailable(name);
//    }

    llvm::TargetMachine *MCJit::getTargetMachine() {
        return executionEngine->getTargetMachine();
    }

    void MCJit::addObjectFile(llvm::object::OwningBinary<llvm::object::ObjectFile> owningObject) {
        getExecutionEngineNonOwning()->addObjectFile(std::move(owningObject));
    }

//    void MCJit::finalizeObject() {
//        getExecutionEngineNonOwning()->finalizeObject();
//    };

    const llvm::DataLayout &MCJit::getDataLayout() {
        return getExecutionEngineNonOwning()->getDataLayout();
    }

    void MCJit::addModule(llvm::Module *M) {

    }

    void MCJit::addModule(std::unique_ptr<llvm::Module> M, std::unique_ptr<llvm::LLVMContext> ctx) {
        executionEngine->addModule(std::move(M));
        optimizeModule();
    }

    void MCJit::addModule(){
        addModuleViaObjectFile();
    }

    void MCJit::addModuleViaObjectFile() {
        optimizeModule();
        if (postOptModuleStream->str().empty()) {
            std::string err = "Attempt to add module before its been optimized. Make a call to "
                              "MCJit::optimizeModule() before addModule()";
            rrLogErr << err;
            throw_llvm_exception(err);
        }
        //Read from modBufferOut into our execution engine
//        std::string moduleStr(postOptModuleStream.begin(), postOptModuleStream.end());

        auto memBuffer(llvm::MemoryBuffer::getMemBuffer(postOptModuleStream->str().str()));

        llvm::Expected<std::unique_ptr<llvm::object::ObjectFile> > objectFileExpected =
                llvm::object::ObjectFile::createObjectFile(llvm::MemoryBufferRef(postOptModuleStream->str(), "id"));

        if (!objectFileExpected) {
            //LS DEBUG:  find a way to get the text out of the error.
            auto err = objectFileExpected.takeError();
            std::string s = "LLVM object supposed to be file, but is not.";
            rrLog(Logger::LOG_FATAL) << s;
            throw_llvm_exception(s);
        }

        std::unique_ptr<llvm::object::ObjectFile> objectFile(std::move(objectFileExpected.get()));
        llvm::object::OwningBinary<llvm::object::ObjectFile> owningObject(std::move(objectFile), std::move(memBuffer));

        addObjectFile(std::move(owningObject));

        //https://stackoverflow.com/questions/28851646/llvm-jit-windows-8-1
        getExecutionEngineNonOwning()->finalizeObject();

    }

    void MCJit::optimizeModule() {
        //Currently we save jitted functions in object file format
        //in save state. Compiling the functions into this format in the first place
        //makes saveState significantly faster than creating the object file when it is called
        //We then load the object file into the jit engine to avoid compiling the functions twice

        auto TargetMachine = getTargetMachine();

        llvm::InitializeNativeTarget(); // todo may have already been called in RoadRunner constructor

        //Write the object file to modBufferOut
        std::error_code EC;
//        llvm::SmallVector<char, 10> modBufferOut;
//        postOptimizedModuleStream(modBufferOut);

        llvm::legacy::PassManager pass;
        auto FileType = getCodeGenFileType();

#if LLVM_VERSION_MAJOR == 6
        if (TargetMachine->addPassesToEmitFile(pass, *postOptModuleStream, FileType))
#elif LLVM_VERSION_MAJOR >= 12
        if (TargetMachine->addPassesToEmitFile(pass, *postOptModuleStream, nullptr, FileType))
#endif
        {
            throw std::logic_error("TargetMachine can't emit a file of type CGFT_ObjectFile");
        }

        pass.run(*getModuleNonOwning());

    }


    Function *MCJit::createGlobalMappingFunction(const char *funcName, llvm::FunctionType *funcType, Module *module) {
        // This was InternalLinkage, but it needs to be external for JIT'd code to see it
        return Function::Create(funcType, Function::ExternalLinkage, funcName, module);
    }

    void MCJit::addGlobalMapping(const llvm::GlobalValue *gv, void *addr) {
        llvm::sys::DynamicLibrary::AddSymbol(gv->getName(), addr);
        executionEngine->addGlobalMapping(gv, addr);
    }

    void MCJit::addGlobalMappings() {
        Type *double_type = Type::getDoubleTy(*context);
        Type *int_type = Type::getInt32Ty(*context);
        Type *args_i1[] = {int_type};
        Type *args_d1[] = {double_type};
        Type *args_d2[] = {double_type, double_type};

        llvm::sys::DynamicLibrary::LoadLibraryPermanently(nullptr);

        addGlobalMapping(ModelDataIRBuilder::getCSRMatrixSetNZDecl(moduleNonOwning), (void *) rr::csr_matrix_set_nz);
        addGlobalMapping(ModelDataIRBuilder::getCSRMatrixGetNZDecl(moduleNonOwning), (void *) rr::csr_matrix_get_nz);

        // AST_FUNCTION_ARCCOT:
        llvm::RTDyldMemoryManager::getSymbolAddressInProcess("arccot");
        addGlobalMapping(
                createGlobalMappingFunction("arccot",
                                            FunctionType::get(double_type, args_d1, false), moduleNonOwning),
                (void *) sbmlsupport::arccot);

        addGlobalMapping(
                createGlobalMappingFunction("rr_arccot_negzero",
                                            FunctionType::get(double_type, args_d1, false), moduleNonOwning),
                (void *) sbmlsupport::arccot_negzero);

        // AST_FUNCTION_ARCCOTH:
        addGlobalMapping(
                createGlobalMappingFunction("arccoth",
                                            FunctionType::get(double_type, args_d1, false), moduleNonOwning),
                (void *) sbmlsupport::arccoth);

        // AST_FUNCTION_ARCCSC:
        addGlobalMapping(
                createGlobalMappingFunction("arccsc",
                                            FunctionType::get(double_type, args_d1, false), moduleNonOwning),
                (void *) sbmlsupport::arccsc);

        // AST_FUNCTION_ARCCSCH:
        addGlobalMapping(
                createGlobalMappingFunction("arccsch",
                                            FunctionType::get(double_type, args_d1, false), moduleNonOwning),
                (void *) sbmlsupport::arccsch);

        // AST_FUNCTION_ARCSEC:
        addGlobalMapping(
                createGlobalMappingFunction("arcsec",
                                            FunctionType::get(double_type, args_d1, false), moduleNonOwning),
                (void *) sbmlsupport::arcsec);

        // AST_FUNCTION_ARCSECH:
        addGlobalMapping(
                createGlobalMappingFunction("arcsech",
                                            FunctionType::get(double_type, args_d1, false), moduleNonOwning),
                (void *) sbmlsupport::arcsech);

        // AST_FUNCTION_COT:
        addGlobalMapping(
                createGlobalMappingFunction("cot",
                                            FunctionType::get(double_type, args_d1, false), moduleNonOwning),
                (void *) sbmlsupport::cot);

        // AST_FUNCTION_COTH:
        addGlobalMapping(
                createGlobalMappingFunction("coth",
                                            FunctionType::get(double_type, args_d1, false), moduleNonOwning),
                (void *) sbmlsupport::coth);

        // AST_FUNCTION_CSC:
        addGlobalMapping(
                createGlobalMappingFunction("csc",
                                            FunctionType::get(double_type, args_d1, false), moduleNonOwning),
                (void *) sbmlsupport::csc);

        // AST_FUNCTION_CSCH:
        addGlobalMapping(
                createGlobalMappingFunction("csch",
                                            FunctionType::get(double_type, args_d1, false), moduleNonOwning),
                (void *) sbmlsupport::csch);

        // AST_FUNCTION_FACTORIAL:
        addGlobalMapping(
                createGlobalMappingFunction("rr_factoriali",
                                            FunctionType::get(int_type, args_i1, false), moduleNonOwning),
                (void *) sbmlsupport::factoriali);

        addGlobalMapping(
                createGlobalMappingFunction("rr_factoriald",
                                            FunctionType::get(double_type, args_d1, false), moduleNonOwning),
                (void *) sbmlsupport::factoriald);

        // AST_FUNCTION_LOG:
        addGlobalMapping(
                createGlobalMappingFunction("rr_logd",
                                            FunctionType::get(double_type, args_d2, false), moduleNonOwning),
                (void *) sbmlsupport::logd);

        // AST_FUNCTION_ROOT:
        addGlobalMapping(
                createGlobalMappingFunction("rr_rootd",
                                            FunctionType::get(double_type, args_d2, false), moduleNonOwning),
                (void *) sbmlsupport::rootd);

        // AST_FUNCTION_SEC:
        addGlobalMapping(
                createGlobalMappingFunction("sec",
                                            FunctionType::get(double_type, args_d1, false), moduleNonOwning),
                (void *) sbmlsupport::sec);

        // AST_FUNCTION_SECH:
        addGlobalMapping(
                createGlobalMappingFunction("sech",
                                            FunctionType::get(double_type, args_d1, false), moduleNonOwning),
                (void *) sbmlsupport::sech);

        // AST_FUNCTION_ARCCOSH:
        addGlobalMapping(
                createGlobalMappingFunction("arccosh",
                                            FunctionType::get(double_type, args_d1, false), moduleNonOwning),
                (void *) static_cast<double (*)(double)>(acosh));

        // AST_FUNCTION_ARCSINH:
        addGlobalMapping(
                createGlobalMappingFunction("arcsinh",
                                            FunctionType::get(double_type, args_d1, false), moduleNonOwning),
                (void *) static_cast<double (*)(double)>(asinh));

        // AST_FUNCTION_ARCTANH:
        addGlobalMapping(
                createGlobalMappingFunction("arctanh",
                                            FunctionType::get(double_type, args_d1, false), moduleNonOwning),
                (void *) static_cast<double (*)(double)>(atanh));

        // AST_FUNCTION_QUOTIENT:
        executionEngine->addGlobalMapping(
                createGlobalMappingFunction("quotient",
                                            FunctionType::get(double_type, args_d2, false), moduleNonOwning),
                (void *) sbmlsupport::quotient);

        // AST_FUNCTION_MAX:
        executionEngine->addGlobalMapping(
                createGlobalMappingFunction("rr_max",
                                            FunctionType::get(double_type, args_d2, false), moduleNonOwning),
                (void *) (sbmlsupport::max));

        // AST_FUNCTION_MIN:
        executionEngine->addGlobalMapping(
                createGlobalMappingFunction("rr_min",
                                            FunctionType::get(double_type, args_d2, false), moduleNonOwning),
                (void *) (sbmlsupport::min));

    }

    void MCJit::initFunctionPassManager() {
        if (options & LoadSBMLOptions::OPTIMIZE) {
            functionPassManager = std::make_unique<legacy::FunctionPassManager>(getModuleNonOwning());

            // Set up the optimizer pipeline.  Start with registering info about how the
            // target lays out data structures.

            /**
             * Note to developers - passes are stored in llvm/Transforms/Scalar.h.
             */

            // we only support LLVM >= 3.1
#if (LLVM_VERSION_MAJOR == 3) && (LLVM_VERSION_MINOR == 1)
            //#if (LLVM_VERSION_MAJOR == 6)
                functionPassManager->add(new TargetData(*executionEngine->getTargetData()));
#elif (LLVM_VERSION_MAJOR == 3) && (LLVM_VERSION_MINOR <= 4)
            functionPassManager->add(new DataLayout(*executionEngine->getDataLayout()));
#elif (LLVM_VERSION_MINOR > 4)
            // Needed for LLVM 3.5 regardless of architecture
            // also, should use DataLayoutPass(module) per Renato (http://reviews.llvm.org/D4607)
            functionPassManager->add(new DataLayoutPass(module));
#elif (LLVM_VERSION_MAJOR >= 6)
            // Do nothing, because I'm not sure what the non-legacy equivalent of DataLayoutPass is
#endif

            // Provide basic AliasAnalysis support for GVN.
            // FIXME: This was deprecated somewhere along the line
            //functionPassManager->add(createBasicAliasAnalysisPass());


            if (options & LoadSBMLOptions::OPTIMIZE_INSTRUCTION_SIMPLIFIER) {
                rrLog(Logger::LOG_INFORMATION) << "using OPTIMIZE_INSTRUCTION_SIMPLIFIER";
#if LLVM_VERSION_MAJOR == 6
                functionPassManager->add(createInstructionSimplifierPass());
#elif LLVM_VERSION_MAJOR >= 12
                functionPassManager->add(createInstSimplifyLegacyPass());
#else
                rrLogWarn << "Not using llvm optimization \"OPTIMIZE_INSTRUCTION_SIMPLIFIER\" "
                             "because llvm version is " << LLVM_VERSION_MAJOR;
#endif
            }

            if (options & LoadSBMLOptions::OPTIMIZE_INSTRUCTION_COMBINING) {
                rrLog(Logger::LOG_INFORMATION) << "using OPTIMIZE_INSTRUCTION_COMBINING";
                functionPassManager->add(createInstructionCombiningPass());
            }

            if (options & LoadSBMLOptions::OPTIMIZE_GVN) {
                rrLog(Logger::LOG_INFORMATION) << "using GVN optimization";
                functionPassManager->add(createNewGVNPass());

            }

            if (options & LoadSBMLOptions::OPTIMIZE_CFG_SIMPLIFICATION) {
                rrLog(Logger::LOG_INFORMATION) << "using OPTIMIZE_CFG_SIMPLIFICATION";
                functionPassManager->add(createCFGSimplificationPass());
            }

            if (options & LoadSBMLOptions::OPTIMIZE_DEAD_INST_ELIMINATION) {
                rrLog(Logger::LOG_INFORMATION) << "using OPTIMIZE_DEAD_INST_ELIMINATION";
#if LLVM_VERSION_MAJOR == 6
                functionPassManager->add(createDeadInstEliminationPass());
#elif LLVM_VERSION_MAJOR >= 12
                // do nothing since this seems to have been removed
                // or replaced with createDeadCodeEliminationPass, which we add below anyway
#else
                rrLogWarn << "Not using OPTIMIZE_DEAD_INST_ELIMINATION because you are using"
                             "LLVM version " << LLVM_VERSION_MAJOR;
#endif
            }

            if (options & LoadSBMLOptions::OPTIMIZE_DEAD_CODE_ELIMINATION) {
                rrLog(Logger::LOG_INFORMATION) << "using OPTIMIZE_DEAD_CODE_ELIMINATION";
                functionPassManager->add(createDeadCodeEliminationPass());
            }

            functionPassManager->doInitialization();
        }
    }

    llvm::legacy::FunctionPassManager *MCJit::getFunctionPassManager() const {
        return functionPassManager.get();
    }

//    void MCJit::setTargetTriple(llvm::Triple triple) {
//        TargetOptions ops;
//        engineBuilder.setTargetOptions()
//    }
//
//    void MCJit::setDataLayout(llvm::DataLayout dataLayout) {
//
//    }




}
