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

using namespace rr;

namespace rrllvm {

    MCJit::MCJit(std::uint32_t opt)
            : Jit(), options(opt),
            engineBuilder(EngineBuilder(std::move(module))),
            executionEngine(std::unique_ptr<ExecutionEngine>(engineBuilder.create())){

        engineBuilder
                .setErrorStr(errString.get())
                .setMCJITMemoryManager(std::make_unique<SectionMemoryManager>());
        MCJit::addGlobalMappings();
        std::cout << emitToString();
        MCJit::initFunctionPassManager();
//        executionEngine->getTargetMachine()->getTargetTriple();
    }


    ExecutionEngine* MCJit::getExecutionEngineNonOwning() const{
        return executionEngine.get();
    }

    void MCJit::addExternalFunctionsFromSBML() {
        addGlobalMappings();
    }

    void MCJit::transferObjectsToResources(std::shared_ptr<rrllvm::ModelResources> rc) {
        Jit::transferObjectsToResources(rc);
        // todo check whether we break stuff by not giving the execution engine to rc
        rc->executionEngine = std::move(executionEngine);
        rc->executionEngine = nullptr;
        rc->errStr = std::move(errString);
        errString = nullptr;
    }

    std::uint64_t MCJit::getFunctionAddress(const string &name) {
        return executionEngine->getFunctionAddress(name);
    }

    llvm::TargetMachine* MCJit::getTargetMachine(){
        return executionEngine->getTargetMachine();
    }

    void MCJit::addObjectFile(llvm::object::OwningBinary<llvm::object::ObjectFile> owningObject){
        getExecutionEngineNonOwning()->addObjectFile(std::move(owningObject));
    }

    void MCJit::finalizeObject(){
        getExecutionEngineNonOwning()->finalizeObject();
    };

    const llvm::DataLayout& MCJit::getDataLayout() {
        return getExecutionEngineNonOwning()->getDataLayout();
    }

    void MCJit::addModule(llvm::Module* M) {

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

    llvm::legacy::FunctionPassManager * MCJit::getFunctionPassManager() const {
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
