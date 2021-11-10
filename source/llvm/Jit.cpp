//
// Created by Ciaran on 25/10/2021.
//

#include <stdint.h>
#include <rrRoadRunnerOptions.h>
#include "Jit.h"
#include "ModelResources.h"
#include "rrRoadRunnerOptions.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/Host.h"
#include "SBMLModelObjectCache.h"

using namespace rr;

namespace rrllvm {

    Jit::Jit(std::uint32_t options)
            : options(options),
              context(std::make_unique<llvm::LLVMContext>()),
              module(std::make_unique<llvm::Module>("LLVM Module", *context)),
              moduleNonOwning(module.get()), /*Maintain a weak ref so we don't lose our handle to the module*/
              builder(std::make_unique<llvm::IRBuilder<>>(*context)) {

        postOptModuleStream = std::make_unique<llvm::raw_svector_ostream>(moduleBuffer);

        // IR module is initialized with just a ModuleID and a source filename
        llvm::InitializeNativeTarget();
        llvm::InitializeNativeTargetAsmPrinter();
        llvm::InitializeNativeTargetAsmParser();
        if (!context) {
            std::string err = "Context pointer is null";
            rrLogErr << err;
            throw_llvm_exception(err)
        }
        if (!module) {
            std::string err = "Module pointer is null";
            rrLogErr << err;
            throw_llvm_exception(err)
        }
        if (!builder) {
            std::string err = "builder pointer is null";
            rrLogErr << err;
            throw_llvm_exception(err)
        }

        createCLibraryFunctions();
    }

    llvm::Module *Jit::getModuleNonOwning() {
        return moduleNonOwning;
    }

    llvm::LLVMContext *Jit::getContextNonOwning() {
        return context.get();
    }

    void Jit::setModuleIdentifier(const std::string &id) {
        moduleNonOwning->setModuleIdentifier(id);
    }


    llvm::IRBuilder<> *Jit::getBuilderNonOwning() {
        return builder.get();
    }


    void Jit::transferObjectsToResources(std::shared_ptr<rrllvm::ModelResources> rc) {
        rc->context = std::move(context);
        context = nullptr;

        std::string stringifiedModule = postOptModuleStream->str().str();
        if (stringifiedModule.empty()) {
            std::string msg = "Compiled RoadRunner instancce not sucessfully stored as string. "
                              "Save and Load state features will not work";
            rrLogWarn << msg;
        }
        rc->moduleStr = stringifiedModule;

//        rc->symbols = symbols;
//        symbols = nullptr;
//        rc->executionEngine = std::move(executionEngine);
//        executionEngine = nullptr;
//        rc->random = random;
//        random = nullptr;
//        rc->errStr = errString;
//        errString = nullptr;
    }


    void Jit::mapFunctionsToAddresses(ModelResources *rc, std::uint32_t options) {

        rc->evalInitialConditionsPtr = reinterpret_cast<EvalInitialConditionsCodeGen::FunctionPtr>(lookupFunctionAddress(
                "evalInitialConditions"));
        rc->evalReactionRatesPtr = reinterpret_cast<EvalReactionRatesCodeGen::FunctionPtr>(lookupFunctionAddress(
                "evalReactionRates"));
        rc->getBoundarySpeciesAmountPtr = reinterpret_cast<GetBoundarySpeciesAmountCodeGen::FunctionPtr>(lookupFunctionAddress(
                "getBoundarySpeciesAmount"));
        rc->getFloatingSpeciesAmountPtr = reinterpret_cast<GetFloatingSpeciesAmountCodeGen::FunctionPtr>(lookupFunctionAddress(
                "getFloatingSpeciesAmount"));
        rc->getBoundarySpeciesConcentrationPtr = reinterpret_cast<GetBoundarySpeciesConcentrationCodeGen::FunctionPtr>(lookupFunctionAddress(
                "getBoundarySpeciesConcentration"));
        rc->getFloatingSpeciesConcentrationPtr = reinterpret_cast<GetFloatingSpeciesAmountCodeGen::FunctionPtr>(lookupFunctionAddress(
                "getFloatingSpeciesConcentration"));
        rc->getCompartmentVolumePtr = reinterpret_cast<GetCompartmentVolumeCodeGen::FunctionPtr>(lookupFunctionAddress(
                "getCompartmentVolume"));
        rc->getGlobalParameterPtr = reinterpret_cast<GetGlobalParameterCodeGen::FunctionPtr>(lookupFunctionAddress(
                "getGlobalParameter"));
        rc->evalRateRuleRatesPtr = reinterpret_cast<EvalRateRuleRatesCodeGen::FunctionPtr>(lookupFunctionAddress(
                "evalRateRuleRates"));
        rc->getEventTriggerPtr = reinterpret_cast<GetEventTriggerCodeGen::FunctionPtr>(lookupFunctionAddress(
                "getEventTrigger"));
        rc->getEventPriorityPtr = reinterpret_cast<GetEventPriorityCodeGen::FunctionPtr>(lookupFunctionAddress(
                "getEventPriority"));
        rc->getEventDelayPtr = reinterpret_cast<GetEventDelayCodeGen::FunctionPtr>(lookupFunctionAddress(
                "getEventDelay"));
        rc->eventTriggerPtr = reinterpret_cast<EventTriggerCodeGen::FunctionPtr>(lookupFunctionAddress("eventTrigger"));
        rc->eventAssignPtr = reinterpret_cast<EventAssignCodeGen::FunctionPtr>(lookupFunctionAddress("eventAssign"));
        rc->evalVolatileStoichPtr = reinterpret_cast<EvalVolatileStoichCodeGen::FunctionPtr>(lookupFunctionAddress(
                "evalVolatileStoich"));
        rc->evalConversionFactorPtr = reinterpret_cast<EvalConversionFactorCodeGen::FunctionPtr>(lookupFunctionAddress(
                "evalConversionFactor"));
        if (options & LoadSBMLOptions::READ_ONLY) {
            rc->setBoundarySpeciesAmountPtr = 0;
            rc->setBoundarySpeciesConcentrationPtr = 0;
            rc->setFloatingSpeciesConcentrationPtr = 0;
            rc->setCompartmentVolumePtr = 0;
            rc->setFloatingSpeciesAmountPtr = 0;
            rc->setGlobalParameterPtr = 0;
        } else {
            rc->setBoundarySpeciesAmountPtr = reinterpret_cast<SetBoundarySpeciesAmountCodeGen::FunctionPtr>(lookupFunctionAddress(
                    "setBoundarySpeciesAmount"));
            rc->setBoundarySpeciesConcentrationPtr = reinterpret_cast<SetBoundarySpeciesAmountCodeGen::FunctionPtr>(lookupFunctionAddress(
                    "setBoundarySpeciesConcentration"));
            rc->setFloatingSpeciesConcentrationPtr = reinterpret_cast<SetBoundarySpeciesAmountCodeGen::FunctionPtr>(lookupFunctionAddress(
                    "setFloatingSpeciesConcentration"));
            rc->setCompartmentVolumePtr = reinterpret_cast<SetBoundarySpeciesAmountCodeGen::FunctionPtr>(lookupFunctionAddress(
                    "setCompartmentVolume"));
            rc->setBoundarySpeciesAmountPtr = reinterpret_cast<SetBoundarySpeciesAmountCodeGen::FunctionPtr>(lookupFunctionAddress(
                    "setBoundarySpeciesAmount"));
            rc->setFloatingSpeciesAmountPtr = reinterpret_cast<SetBoundarySpeciesAmountCodeGen::FunctionPtr>(lookupFunctionAddress(
                    "setFloatingSpeciesAmount"));
            rc->setGlobalParameterPtr = reinterpret_cast<SetGlobalParameterCodeGen::FunctionPtr>(lookupFunctionAddress(
                    "setGlobalParameter"));
        }

        if (options & LoadSBMLOptions::MUTABLE_INITIAL_CONDITIONS) {
            rc->getFloatingSpeciesInitConcentrationsPtr = reinterpret_cast<GetBoundarySpeciesAmountCodeGen::FunctionPtr>(lookupFunctionAddress(
                    "getFloatingSpeciesInitConcentrations"));
            rc->setFloatingSpeciesInitConcentrationsPtr = reinterpret_cast<SetBoundarySpeciesAmountCodeGen::FunctionPtr>(lookupFunctionAddress(
                    "setFloatingSpeciesInitConcentrations"));
            rc->getFloatingSpeciesInitAmountsPtr = reinterpret_cast<GetBoundarySpeciesAmountCodeGen::FunctionPtr>(lookupFunctionAddress(
                    "getFloatingSpeciesInitAmounts"));
            rc->setFloatingSpeciesInitAmountsPtr = reinterpret_cast<SetBoundarySpeciesAmountCodeGen::FunctionPtr>(lookupFunctionAddress(
                    "setFloatingSpeciesInitAmounts"));
            rc->getBoundarySpeciesInitConcentrationsPtr = reinterpret_cast<GetBoundarySpeciesAmountCodeGen::FunctionPtr>(lookupFunctionAddress(
                    "getBoundarySpeciesInitConcentrations"));
            rc->setBoundarySpeciesInitConcentrationsPtr = reinterpret_cast<SetBoundarySpeciesAmountCodeGen::FunctionPtr>(lookupFunctionAddress(
                    "setBoundarySpeciesInitConcentrations"));
            rc->getBoundarySpeciesInitAmountsPtr = reinterpret_cast<GetBoundarySpeciesAmountCodeGen::FunctionPtr>(lookupFunctionAddress(
                    "getBoundarySpeciesInitAmounts"));
            rc->setBoundarySpeciesInitAmountsPtr = reinterpret_cast<SetBoundarySpeciesAmountCodeGen::FunctionPtr>(lookupFunctionAddress(
                    "setBoundarySpeciesInitAmounts"));
            rc->getCompartmentInitVolumesPtr = reinterpret_cast<GetBoundarySpeciesAmountCodeGen::FunctionPtr>(lookupFunctionAddress(
                    "getCompartmentInitVolumes"));
            rc->setCompartmentInitVolumesPtr = reinterpret_cast<SetBoundarySpeciesAmountCodeGen::FunctionPtr>(lookupFunctionAddress(
                    "setCompartmentInitVolumes"));
            rc->getGlobalParameterInitValuePtr = reinterpret_cast<GetBoundarySpeciesAmountCodeGen::FunctionPtr>(lookupFunctionAddress(
                    "getGlobalParameterInitValue"));
            rc->setGlobalParameterInitValuePtr = reinterpret_cast<SetBoundarySpeciesAmountCodeGen::FunctionPtr>(lookupFunctionAddress(
                    "setGlobalParameterInitValue"));
        } else {
            rc->getFloatingSpeciesInitConcentrationsPtr = 0;
            rc->setFloatingSpeciesInitConcentrationsPtr = 0;

            rc->getFloatingSpeciesInitAmountsPtr = 0;
            rc->setFloatingSpeciesInitAmountsPtr = 0;

            rc->getBoundarySpeciesInitConcentrationsPtr = 0;
            rc->setBoundarySpeciesInitConcentrationsPtr = 0;

            rc->getBoundarySpeciesInitAmountsPtr = 0;
            rc->setBoundarySpeciesInitAmountsPtr = 0;

            rc->getCompartmentInitVolumesPtr = 0;
            rc->setCompartmentInitVolumesPtr = 0;

            rc->getGlobalParameterInitValuePtr = 0;
            rc->setGlobalParameterInitValuePtr = 0;
        }
    }

    std::string Jit::emitToString() {
        std::string str;
        llvm::raw_string_ostream os(str);
        os << *getModuleNonOwning();
        os.flush();
        return str;
    }

    llvm::raw_svector_ostream &Jit::getPostOptModuleStream() {
        return *postOptModuleStream;
    }


    void Jit::createCLibraryFunction(llvm::LibFunc funcId, llvm::FunctionType *funcType) {
        // For some reason I had a problem when I passed the default ctor for TargetLibraryInfoImpl in
        // std::string error;
        //TargetLibraryInfoImpl defaultImpl(TargetRegistry::lookupTarget(sys::getDefaultTargetTriple(), error));
        llvm::TargetLibraryInfoImpl defaultImpl;
        llvm::TargetLibraryInfo targetLib(defaultImpl);

        if (targetLib.has(funcId)) {
            const llvm::StringRef &name = targetLib.getName(funcId);
            llvm::Function::Create(
                    funcType, llvm::Function::ExternalLinkage, name, getModuleNonOwning()
            );
        } else {
            std::string msg = "native target does not have library function for ";
            msg += targetLib.getName(funcId);
            throw_llvm_exception(msg);
        }
    }

    void Jit::createCLibraryFunctions() {
        using namespace llvm;
//        LLVMContext &context = module->getContext();
        Type *double_type = Type::getDoubleTy(*context);
        Type *args_d1[] = {double_type};
        Type *args_d2[] = {double_type, double_type};

        /// double pow(double x, double y);
        createCLibraryFunction(LibFunc_pow,
                               FunctionType::get(double_type, args_d2, false));

        /// double fabs(double x);
        createCLibraryFunction(LibFunc_fabs,
                               FunctionType::get(double_type, args_d1, false));

        /// double acos(double x);
        createCLibraryFunction(LibFunc_acos,
                               FunctionType::get(double_type, args_d1, false));

        /// double asin(double x);
        createCLibraryFunction(LibFunc_asin,
                               FunctionType::get(double_type, args_d1, false));

        /// double atan(double x);
        createCLibraryFunction(LibFunc_atan,
                               FunctionType::get(double_type, args_d1, false));

        /// double ceil(double x);
        createCLibraryFunction(LibFunc_ceil,
                               FunctionType::get(double_type, args_d1, false));

        /// double cos(double x);
        createCLibraryFunction(LibFunc_cos,
                               FunctionType::get(double_type, args_d1, false));

        /// double cosh(double x);
        createCLibraryFunction(LibFunc_cosh,
                               FunctionType::get(double_type, args_d1, false));

        /// double exp(double x);
        createCLibraryFunction(LibFunc_exp,
                               FunctionType::get(double_type, args_d1, false));

        /// double floor(double x);
        createCLibraryFunction(LibFunc_floor,
                               FunctionType::get(double_type, args_d1, false));

        /// double log(double x);
        createCLibraryFunction(LibFunc_log,
                               FunctionType::get(double_type, args_d1, false));

        /// double log10(double x);
        createCLibraryFunction(LibFunc_log10,
                               FunctionType::get(double_type, args_d1, false));

        /// double sin(double x);
        createCLibraryFunction(LibFunc_sin,
                               FunctionType::get(double_type, args_d1, false));

        /// double sinh(double x);
        createCLibraryFunction(LibFunc_sinh,
                               FunctionType::get(double_type, args_d1, false));

        /// double tan(double x);
        createCLibraryFunction(LibFunc_tan,
                               FunctionType::get(double_type, args_d1, false));

        /// double tanh(double x);
        createCLibraryFunction(LibFunc_tanh,
                               FunctionType::get(double_type, args_d1, false));

        /// double fmod(double x, double y);
        createCLibraryFunction(LibFunc_fmod,
                               FunctionType::get(double_type, args_d2, false));
    }

    void Jit::addModuleViaObjectFile() {
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

    }

    /**
    * getProcessTriple() - Return an appropriate target triple for generating
    * code to be loaded into the current process, e.g. when using the JIT.
    */
    std::string Jit::getProcessTriple() const {
        return llvm::sys::getProcessTriple();
    }

    std::string Jit::getDefaultTargetTriple() const {
        return llvm::sys::getDefaultTargetTriple();
    }


    const llvm::Target *Jit::getDefaultTargetMachine() const {
        std::string err;
        const llvm::Target *target = llvm::TargetRegistry::lookupTarget(getDefaultTargetTriple(), err);

        // Print an error and exit if we couldn't find the requested target.
        // This generally occurs if we've forgotten to initialise the
        // TargetRegistry or we have a bogus target triple.
        if (!target) {
            rrLogErr << err;
            llvm::errs() << err;
        }
        return target;
    }

}

