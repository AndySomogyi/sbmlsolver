//
// Created by Ciaran on 25/10/2021.
//


#include "LLJit.h"
#include "rrConfig.h"
#include "rrLogger.h"
#include "llvm/IR/Function.h"
#include "SBMLSupportFunctions.h"
#include "llvm/ExecutionEngine/Orc/EPCDynamicLibrarySearchGenerator.h"
#include "ModelDataIRBuilder.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SourceMgr.h"
#include "SBMLModelObjectCache.h"
#include "rrRoadRunnerOptions.h"
#include "rrSparse.h"

#if LIBSBML_HAS_PACKAGE_DISTRIB

#   include "Random.h"

#endif

using namespace llvm;
using namespace rr;
using namespace sbmlsupport;

namespace rrllvm {

#define strToFnPtrMapElement(funcName, fnPtr)    {llJit->mangleAndIntern(funcName), JITEvaluatedSymbol::fromPointer(fnPtr)}


    llvm::CodeGenOpt::Level convertRRCodeGenOptLevelToLLVM(std::uint32_t options) {
        // these should be mutually exclusive
        if (options & rr::LoadSBMLOptions::NONE)
            return llvm::CodeGenOpt::None;
        else if (options & rr::LoadSBMLOptions::LESS)
            return llvm::CodeGenOpt::Less;
        else if (options & rr::LoadSBMLOptions::DEFAULT)
            return llvm::CodeGenOpt::Default;
        else if (options & rr::LoadSBMLOptions::AGGRESSIVE)
            return llvm::CodeGenOpt::Aggressive;
        else {
            throw std::invalid_argument("None of the LLJIT_OPTIMIZATION_LEVELS are set to true");
        }
    }

    LLJit::LLJit(std::uint32_t options)
            : Jit(options) {

        // todo, can we cross compile providing a different host arch?

        /**
         * Create a JTMB.
         *
         * This is moved into the LLJitBuilder and becomes harder to access, so pull out
         * things which we need later on and store them as member variables.
         */
        llvm::Expected<llvm::orc::JITTargetMachineBuilder> expectedJTMB = llvm::orc::JITTargetMachineBuilder::detectHost();
        if (!expectedJTMB) {
            llvm::logAllUnhandledErrors(std::move(expectedJTMB.takeError()), llvm::errs(),
                                        "Cannot create JitTargetMachineBuilder");
        }
        auto JTMB = *expectedJTMB;

        /**
         * None, Less, Default or Aggressive are the options.
         */
        JTMB.setCodeGenOptLevel(convertRRCodeGenOptLevelToLLVM(options));

        // query the target machine builder for its data layout.
        auto DL = JTMB.getDefaultDataLayoutForTarget();
        if (!DL) {
            std::string err = "Unable to get default data layout";
            rrLogErr << err;
            llvm::logAllUnhandledErrors(std::move(DL.takeError()), llvm::errs(), err);
            throw_llvm_exception(err);
        }
//#ifndef NDEBUG // defined in llvm. JITTargetMachineBuilderPrinter only exists in dbg builds
//        if (Logger::getLevel() <= Logger::Level::LOG_DEBUG) {
//            std::string s;
//            llvm::raw_string_ostream os(s);
//            llvm::orc::JITTargetMachineBuilderPrinter jtmbp(JTMB, "RoadRunnnerJTMBPrinter");
//            jtmbp.print(os);
//            rrLogDebug << "JitTargetMachineBuilder information: ";
//            rrLogDebug << s;
//        }
//#endif
        llvm::Expected<std::unique_ptr<llvm::TargetMachine>> expectedTargetMachine = JTMB.createTargetMachine();
        if (!expectedTargetMachine) {
            std::string err = "Could not create target machine";
            rrLogErr << err;
            llvm::logAllUnhandledErrors(std::move(expectedTargetMachine.takeError()), llvm::errs(), err);
            throw_llvm_exception(err);
        }

        // tell the LLJit instance to look in the current process
        // for symbols before complaining that they can't be found,
        auto DLSG = llvm::orc::DynamicLibrarySearchGenerator::GetForCurrentProcess(DL->getGlobalPrefix());
        if (!DLSG) {
            llvm::logAllUnhandledErrors(
                    std::move(DLSG.takeError()),
                    llvm::errs(),
                    "DynamicLibrarySearchGenerator not built successfully"
            );
        }

        /**
         * todo expose as option to user
         */
        int numCompileThreads = rr::Config::getValue(rr::Config::LLJIT_NUM_THREADS).getAs<int>();
        // in case c++ fails to detect num cores from hardware
        // when using default options.
        if (numCompileThreads == 0) {
            numCompileThreads = 1;
        }

        // Create the LLJitBuilder
        llvm::orc::LLJITBuilder llJitBuilder;
        llJitBuilder
                .setNumCompileThreads(numCompileThreads)
                .setDataLayout(std::move(*DL))
                .setJITTargetMachineBuilder(std::move(JTMB))

                        /**
                         * Set a custom compile function that 1) caches objects
                         * and 2) stores a pointer to the targetMachine for access later.
                         * The targetMachine pointer is owned by the JITTargetMachineBuilder.
                         */
                .setCompileFunctionCreator([&](llvm::orc::JITTargetMachineBuilder JTMB)
                                                   -> Expected<std::unique_ptr<llvm::orc::IRCompileLayer::IRCompiler>> {
                    // Create the target machine.
                    auto TM = JTMB.createTargetMachine();
                    if (!TM)
                        return TM.takeError();

                    // store a pointer to it for later use
                    targetMachineNonOwning = (*TM).get();
                    if (numCompileThreads > 0)
                        return std::make_unique<llvm::orc::ConcurrentIRCompiler>(
                                std::move(JTMB),
                                &SBMLModelObjectCache::getObjectCache());

                    return std::make_unique<llvm::orc::TMOwningSimpleCompiler>(
                            std::move(*TM),
                            &SBMLModelObjectCache::getObjectCache());
                });


        auto llJitExpected = llJitBuilder.create();
        if (!llJitExpected) {
            std::string err = "failed to create an LLJit instance";
            rrLogErr << err;
            llvm::logAllUnhandledErrors(
                    std::move(llJitExpected.takeError()), llvm::errs(), "[LLJit creation failure] ");
        }
        // if success, grab the LLJit obj from the llvm::Expected object.
        llJit = std::move(*llJitExpected);

        // adds the generater created above
        llJit->getMainJITDylib().addGenerator(std::move(*DLSG));

        LLJit::mapFunctionsToJitSymbols();
        LLJit::mapDistribFunctionsToJitSymbols();
    }

//    void LLJit::mapFunctionsToJitSymbols2() {
//        using namespace rr;
//
//        mapFunctionToAbsoluteSymbol("arccot", reinterpret_cast<std::uint64_t>(&sbmlsupport::arccot));
//        mapFunctionToAbsoluteSymbol("rr_arccot_negzero",
//                                    reinterpret_cast<std::uint64_t>(&sbmlsupport::rr_arccot_negzero));
//        mapFunctionToAbsoluteSymbol("arccoth", reinterpret_cast<std::uint64_t>(&sbmlsupport::arccoth));
//        mapFunctionToAbsoluteSymbol("arccsc", reinterpret_cast<std::uint64_t>(&sbmlsupport::arccsc));
//        mapFunctionToAbsoluteSymbol("arccsch", reinterpret_cast<std::uint64_t>(&sbmlsupport::arccsch));
//        mapFunctionToAbsoluteSymbol("arcsec", reinterpret_cast<std::uint64_t>(&sbmlsupport::arcsec));
//        mapFunctionToAbsoluteSymbol("arcsech", reinterpret_cast<std::uint64_t>(&sbmlsupport::arcsech));
//        mapFunctionToAbsoluteSymbol("cot", reinterpret_cast<std::uint64_t>(&sbmlsupport::cot));
//        mapFunctionToAbsoluteSymbol("coth", reinterpret_cast<std::uint64_t>(&sbmlsupport::coth));
//        mapFunctionToAbsoluteSymbol("csc", reinterpret_cast<std::uint64_t>(&sbmlsupport::csc));
//        mapFunctionToAbsoluteSymbol("csch", reinterpret_cast<std::uint64_t>(&sbmlsupport::csch));
//        mapFunctionToAbsoluteSymbol("rr_factoriali", reinterpret_cast<std::uint64_t>(&sbmlsupport::rr_factoriali));
//        mapFunctionToAbsoluteSymbol("rr_factoriald", reinterpret_cast<std::uint64_t>(&sbmlsupport::rr_factoriald));
//        mapFunctionToAbsoluteSymbol("rr_logd", reinterpret_cast<std::uint64_t>(&sbmlsupport::rr_logd));
//        mapFunctionToAbsoluteSymbol("rr_rootd", reinterpret_cast<std::uint64_t>(&sbmlsupport::rr_rootd));
//        mapFunctionToAbsoluteSymbol("sec", reinterpret_cast<std::uint64_t>(&sbmlsupport::sec));
//        mapFunctionToAbsoluteSymbol("sech", reinterpret_cast<std::uint64_t>(&sbmlsupport::sech));
//        mapFunctionToAbsoluteSymbol("arccosh", reinterpret_cast<std::uint64_t>(&sbmlsupport::arccosh));
//        mapFunctionToAbsoluteSymbol("arcsinh", reinterpret_cast<std::uint64_t>(&sbmlsupport::arcsinh));
//        mapFunctionToAbsoluteSymbol("arctanh", reinterpret_cast<std::uint64_t>(&sbmlsupport::arctanh));
//        mapFunctionToAbsoluteSymbol("quotient", reinterpret_cast<std::uint64_t>(&sbmlsupport::quotient));
//        mapFunctionToAbsoluteSymbol("rr_max", reinterpret_cast<std::uint64_t>(&sbmlsupport::rr_max));
//        mapFunctionToAbsoluteSymbol("rr_min", reinterpret_cast<std::uint64_t>(&sbmlsupport::rr_min));
//        mapFunctionToAbsoluteSymbol("csr_matrix_get_nz", reinterpret_cast<std::uint64_t>(&csr_matrix_get_nz));
//        mapFunctionToAbsoluteSymbol("csr_matrix_set_nz", reinterpret_cast<std::uint64_t>(&csr_matrix_set_nz));
//    }

    void LLJit::mapFunctionsToJitSymbols() {
        llvm::Error err = llJit->getMainJITDylib().define(llvm::orc::absoluteSymbols(
                {
                        strToFnPtrMapElement("arccot", &sbmlsupport::arccot),
                        strToFnPtrMapElement("rr_arccot_negzero", &sbmlsupport::rr_arccot_negzero),
                        strToFnPtrMapElement("arccoth", &sbmlsupport::arccoth),
                        strToFnPtrMapElement("arccsc", &sbmlsupport::arccsc),
                        strToFnPtrMapElement("arccsch", &sbmlsupport::arccsch),
                        strToFnPtrMapElement("arcsec", &sbmlsupport::arcsec),
                        strToFnPtrMapElement("arcsech", &sbmlsupport::arcsech),
                        strToFnPtrMapElement("cot", &sbmlsupport::cot),
                        strToFnPtrMapElement("coth", &sbmlsupport::coth),
                        strToFnPtrMapElement("csc", &sbmlsupport::csc),
                        strToFnPtrMapElement("csch", &sbmlsupport::csch),
                        strToFnPtrMapElement("rr_factoriali", &sbmlsupport::rr_factoriali),
                        strToFnPtrMapElement("rr_factoriald", &sbmlsupport::rr_factoriald),
                        strToFnPtrMapElement("rr_logd", &sbmlsupport::rr_logd),
                        strToFnPtrMapElement("rr_rootd", &sbmlsupport::rr_rootd),
                        strToFnPtrMapElement("sec", &sbmlsupport::sec),
                        strToFnPtrMapElement("sech", &sbmlsupport::sech),
                        strToFnPtrMapElement("arccosh", &sbmlsupport::arccosh),
                        strToFnPtrMapElement("arcsinh", &sbmlsupport::arcsinh),
                        strToFnPtrMapElement("arctanh", &sbmlsupport::arctanh),
                        strToFnPtrMapElement("quotient", &sbmlsupport::quotient),
                        strToFnPtrMapElement("rr_max", &sbmlsupport::rr_max),
                        strToFnPtrMapElement("rr_min", &sbmlsupport::rr_min),
                        strToFnPtrMapElement("csr_matrix_get_nz", &rr::csr_matrix_get_nz),
                        strToFnPtrMapElement("csr_matrix_set_nz", &rr::csr_matrix_set_nz),
                }
        ));
        if (err) {
            llvm::logAllUnhandledErrors(std::move(err), llvm::errs(), "[ error mapping functions to symbols ]");
        }

    }

    void LLJit::mapDistribFunctionsToJitSymbols() {
        // todo merge with mapFunctionsToJitSymbols. Creation of one map with more elements faster then two with less.
#if LIBSBML_HAS_PACKAGE_DISTRIB
        llvm::Error err = llJit->getMainJITDylib().define(llvm::orc::absoluteSymbols(
                {
                        strToFnPtrMapElement("rr_distrib_uniform", &distrib_uniform),
                        strToFnPtrMapElement("rr_distrib_normal", &distrib_normal),
                        strToFnPtrMapElement("rr_distrib_normal_four", &distrib_normal_four),
                        strToFnPtrMapElement("rr_distrib_bernoulli", &distrib_bernoulli),
                        strToFnPtrMapElement("rr_distrib_binomial", &distrib_binomial),
                        strToFnPtrMapElement("rr_distrib_binomial_four", &distrib_binomial_four),
                        strToFnPtrMapElement("rr_distrib_cauchy", &distrib_cauchy),
                        strToFnPtrMapElement("rr_distrib_cauchy_one", &distrib_cauchy_one),
                        strToFnPtrMapElement("rr_distrib_cauchy_four", &distrib_cauchy_four),
                        strToFnPtrMapElement("rr_distrib_chisquare", &distrib_chisquare),
                        strToFnPtrMapElement("rr_distrib_chisquare_three", &distrib_chisquare_three),
                        strToFnPtrMapElement("rr_distrib_exponential", &distrib_exponential),
                        strToFnPtrMapElement("rr_distrib_exponential_three", &distrib_exponential_three),
                        strToFnPtrMapElement("rr_distrib_gamma", &distrib_gamma),
                        strToFnPtrMapElement("rr_distrib_gamma_four", &distrib_gamma_four),
                        strToFnPtrMapElement("rr_distrib_laplace", &distrib_laplace),
                        strToFnPtrMapElement("rr_distrib_laplace_one", &distrib_laplace_one),
                        strToFnPtrMapElement("rr_distrib_laplace_four", &distrib_laplace_four),
                        strToFnPtrMapElement("rr_distrib_lognormal", &distrib_lognormal),
                        strToFnPtrMapElement("rr_distrib_lognormal_four", &distrib_lognormal_four),
                        strToFnPtrMapElement("rr_distrib_poisson", &distrib_poisson),
                        strToFnPtrMapElement("rr_distrib_poisson_three", &distrib_poisson_three),
                        strToFnPtrMapElement("rr_distrib_rayleigh", &distrib_rayleigh),
                        strToFnPtrMapElement("rr_distrib_rayleigh_three", &distrib_rayleigh_three),
                }
        ));
        if (err){
            llvm::logAllUnhandledErrors(std::move(err), llvm::errs(), "[ distrib functions failed to map to symbols ]");
        }
#endif
    };

    std::uint64_t LLJit::lookupFunctionAddress(const std::string &name) {
        // automatically looks up manged name
        llvm::Expected<llvm::JITEvaluatedSymbol> expectedSymbol = llJit->lookup(name);
        if (!expectedSymbol) {
            std::string err = "Could not find symbol " + mangleName(name);
            rrLogErr << err;
            llvm::logAllUnhandledErrors(
                    std::move(expectedSymbol.takeError()),
                    llvm::errs(),
                    "[symbol lookup error] ");
            throw std::logic_error(err);
        }
        llvm::JITEvaluatedSymbol symbol = *expectedSymbol;
        rrLogInfo << "LLJit has loaded jit'd function called " << name;
        return symbol.getAddress();
    }


    void LLJit::addObjectFile(rrOwningBinary owningObject) {
        llvm::Error err = llJit->addObjectFile(std::move(owningObject.takeBinary().second));
        if (err) {
            std::string errMsg = "Could not add object to LLJit";
            rrLogErr << errMsg;
            llvm::logAllUnhandledErrors(std::move(err), llvm::errs(), errMsg);
        }
    }

    const llvm::DataLayout &LLJit::getDataLayout() const {
        return llJit->getDataLayout();
    }

    void LLJit::addModule(llvm::orc::ThreadSafeModule tsm) {
        std::cout << "full module: " << std::endl;
        std::cout << emitToString() << std::endl;
        if (llvm::Error err = llJit->addIRModule(std::move(tsm))) {
            std::string errMsg = "Unable to add ThreadSafeModule to LLJit";
            rrLogErr << errMsg;
            llvm::logAllUnhandledErrors(std::move(err), llvm::errs(), errMsg);
        }
    }

    void LLJit::addModule(std::unique_ptr<llvm::Module> M, std::unique_ptr<llvm::LLVMContext> ctx) {
        llvm::Error err = llJit->addIRModule(
                llvm::orc::ThreadSafeModule(std::move(M), std::move(ctx))
        );
        if (err) {
            llvm::logAllUnhandledErrors(std::move(err), llvm::errs(), "error adding module");
        }
    }


    void LLJit::addModule(llvm::Module *m) {

    }

    void LLJit::addObjectFile(std::unique_ptr<llvm::MemoryBuffer> obj) {
        llvm::Error err = llJit->addObjectFile(std::move(obj));
        if (err) {
            std::string s = "Unable to add object file to LLJit";
            rrLogErr << s;
            llvm::logAllUnhandledErrors(std::move(err), llvm::errs(), s);
        }
    }

    void LLJit::addObjectFile(std::unique_ptr<llvm::object::ObjectFile> objectFile) {
//        llJit->addObjectFile(std::move(objectFile));

    }


    void LLJit::addModule() {
        llvm::orc::ThreadSafeModule TSM(std::move(module), std::move(context));
        llvm::Error err = llJit->addIRModule(std::move(TSM));
        if (err) {
            std::string errMsg = "Could not add main JITDylib to LLJit";
            llvm::logAllUnhandledErrors(std::move(err), llvm::errs(), errMsg);
            rrLogErr << errMsg;
        }
    }

    std::unique_ptr<llvm::MemoryBuffer> LLJit::getCompiledModelFromCache(const std::string &sbmlMD5) {
        SBMLModelObjectCache &cache = SBMLModelObjectCache::getObjectCache();
        auto v = cache.inspect();
        LLVMContext ctx;
        std::unique_ptr<llvm::Module> m = std::make_unique<llvm::Module>(sbmlMD5, ctx);
        std::unique_ptr<llvm::MemoryBuffer> objBuf = cache.getObject(m.get());
        return std::move(objBuf);
    }

    llvm::orc::LLJIT *LLJit::getLLJitNonOwning() {
        return llJit.get();
    }

    std::string LLJit::dump() {
        std::string s;
        llvm::raw_string_ostream os(s);
        llJit->getExecutionSession().dump(os);
        return s;
    }

    std::ostream &operator<<(std::ostream &os, LLJit *llJit) {
        os << llJit->dump();
        return os;
    }


    void LLJit::addIRModule() {
        llvm::orc::ThreadSafeModule tsm(std::move(module), std::move(context));
        llvm::Error err = llJit->addIRModule(std::move(tsm));
        if (err) {
            std::string errMsg = "Could not add module to LLJit";
            rrLogErr << errMsg;
            llvm::logAllUnhandledErrors(std::move(err), llvm::errs(), errMsg);
        }
    }

    std::string LLJit::getModuleAsString(std::string sbmlMD5) {
        std::unique_ptr<llvm::MemoryBuffer> memBuf = getCompiledModelFromCache(sbmlMD5);
        MemoryBufferRef memBufRef = memBuf->getMemBufferRef();
        std::string s = memBufRef.getBuffer().str();
        if (s.empty()) {
            std::string err = "Unable to convert module to string. Have you made a call to addModule or addObject yet?";
            rrLogErr << err;
            throw_llvm_exception(err);
        }
        return s;
    };


}