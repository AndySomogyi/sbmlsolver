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
#include "rrRoadRunnerOptions.h"

using namespace rr;

namespace rrllvm {

    Jit::Jit(std::uint32_t options)
            : options(options),
              context(std::make_unique<llvm::LLVMContext>()),
            // todo the module name should be the sbmlMD5. Might be cleaner to
            //  add this as a parameter to Jit constructor.
              module(std::make_unique<llvm::Module>("LLVM Module", *context)),
              moduleNonOwning(module.get()), /*Maintain a weak ref so we don't lose our handle to the module*/
              builder(std::make_unique<llvm::IRBuilder<>>(*context)) {

        compiledModuleStream = std::make_unique<llvm::raw_svector_ostream>(moduleBuffer);

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

    Jit::Jit()
        : Jit(LoadSBMLOptions().modelGeneratorOpt){}

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

    void Jit::transferObjectsToResources(std::shared_ptr<rrllvm::ModelResources> modelResources) {
        modelResources->context = std::move(context);
        context = nullptr;

        rrLogCriticalCiaran << "This is where you are converting object to module"
                               "string";

        std::string stringifiedModule = compiledModuleStream->str().str();
        if (stringifiedModule.empty()) {
//            stringifiedModule = getCompiledModelFromCache()
            std::string msg = "Compiled RoadRunner instancce not sucessfully stored as string. "
                              "Save and Load state features will not work";
            rrLogWarn << msg;
        }

        modelResources->moduleStr = stringifiedModule;

//        modelResources->symbols = symbols;
//        symbols = nullptr;
//        modelResources->executionEngine = std::move(executionEngine);
//        executionEngine = nullptr;
//        modelResources->random = random;
//        random = nullptr;
//        modelResources->errStr = errString;
//        errString = nullptr;
    }


    void Jit::mapLLVMGeneratedFunctionsToSymbols(ModelResources *modelResources, std::uint32_t options) {

        modelResources->evalInitialConditionsPtr = reinterpret_cast<EvalInitialConditionsCodeGen::FunctionPtr>(lookupFunctionAddress(
                "evalInitialConditions"));
        modelResources->evalReactionRatesPtr = reinterpret_cast<EvalReactionRatesCodeGen::FunctionPtr>(lookupFunctionAddress(
                "evalReactionRates"));
        modelResources->getBoundarySpeciesAmountPtr = reinterpret_cast<GetBoundarySpeciesAmountCodeGen::FunctionPtr>(lookupFunctionAddress(
                "getBoundarySpeciesAmount"));
        modelResources->getFloatingSpeciesAmountPtr = reinterpret_cast<GetFloatingSpeciesAmountCodeGen::FunctionPtr>(lookupFunctionAddress(
                "getFloatingSpeciesAmount"));
        modelResources->getBoundarySpeciesConcentrationPtr = reinterpret_cast<GetBoundarySpeciesConcentrationCodeGen::FunctionPtr>(lookupFunctionAddress(
                "getBoundarySpeciesConcentration"));
        modelResources->getFloatingSpeciesConcentrationPtr = reinterpret_cast<GetFloatingSpeciesAmountCodeGen::FunctionPtr>(lookupFunctionAddress(
                "getFloatingSpeciesConcentration"));
        modelResources->getCompartmentVolumePtr = reinterpret_cast<GetCompartmentVolumeCodeGen::FunctionPtr>(lookupFunctionAddress(
                "getCompartmentVolume"));
        modelResources->getGlobalParameterPtr = reinterpret_cast<GetGlobalParameterCodeGen::FunctionPtr>(lookupFunctionAddress(
                "getGlobalParameter"));
        modelResources->evalRateRuleRatesPtr = reinterpret_cast<EvalRateRuleRatesCodeGen::FunctionPtr>(lookupFunctionAddress(
                "evalRateRuleRates"));
        modelResources->getEventTriggerPtr = reinterpret_cast<GetEventTriggerCodeGen::FunctionPtr>(lookupFunctionAddress(
                "getEventTrigger"));
        modelResources->getEventPriorityPtr = reinterpret_cast<GetEventPriorityCodeGen::FunctionPtr>(lookupFunctionAddress(
                "getEventPriority"));
        modelResources->getEventDelayPtr = reinterpret_cast<GetEventDelayCodeGen::FunctionPtr>(lookupFunctionAddress(
                "getEventDelay"));
        modelResources->eventTriggerPtr = reinterpret_cast<EventTriggerCodeGen::FunctionPtr>(lookupFunctionAddress("eventTrigger"));
        modelResources->eventAssignPtr = reinterpret_cast<EventAssignCodeGen::FunctionPtr>(lookupFunctionAddress("eventAssign"));
        modelResources->evalVolatileStoichPtr = reinterpret_cast<EvalVolatileStoichCodeGen::FunctionPtr>(lookupFunctionAddress(
                "evalVolatileStoich"));
        modelResources->evalConversionFactorPtr = reinterpret_cast<EvalConversionFactorCodeGen::FunctionPtr>(lookupFunctionAddress(
                "evalConversionFactor"));
        if (options & LoadSBMLOptions::READ_ONLY) {
            modelResources->setBoundarySpeciesAmountPtr = 0;
            modelResources->setBoundarySpeciesConcentrationPtr = 0;
            modelResources->setFloatingSpeciesConcentrationPtr = 0;
            modelResources->setCompartmentVolumePtr = 0;
            modelResources->setFloatingSpeciesAmountPtr = 0;
            modelResources->setGlobalParameterPtr = 0;
        } else {
            modelResources->setBoundarySpeciesAmountPtr = reinterpret_cast<SetBoundarySpeciesAmountCodeGen::FunctionPtr>(lookupFunctionAddress(
                    "setBoundarySpeciesAmount"));
            modelResources->setBoundarySpeciesConcentrationPtr = reinterpret_cast<SetBoundarySpeciesAmountCodeGen::FunctionPtr>(lookupFunctionAddress(
                    "setBoundarySpeciesConcentration"));
            modelResources->setFloatingSpeciesConcentrationPtr = reinterpret_cast<SetBoundarySpeciesAmountCodeGen::FunctionPtr>(lookupFunctionAddress(
                    "setFloatingSpeciesConcentration"));
            modelResources->setCompartmentVolumePtr = reinterpret_cast<SetBoundarySpeciesAmountCodeGen::FunctionPtr>(lookupFunctionAddress(
                    "setCompartmentVolume"));
            modelResources->setBoundarySpeciesAmountPtr = reinterpret_cast<SetBoundarySpeciesAmountCodeGen::FunctionPtr>(lookupFunctionAddress(
                    "setBoundarySpeciesAmount"));
            modelResources->setFloatingSpeciesAmountPtr = reinterpret_cast<SetBoundarySpeciesAmountCodeGen::FunctionPtr>(lookupFunctionAddress(
                    "setFloatingSpeciesAmount"));
            modelResources->setGlobalParameterPtr = reinterpret_cast<SetGlobalParameterCodeGen::FunctionPtr>(lookupFunctionAddress(
                    "setGlobalParameter"));
        }

        if (options & LoadSBMLOptions::MUTABLE_INITIAL_CONDITIONS) {
            modelResources->getFloatingSpeciesInitConcentrationsPtr = reinterpret_cast<GetBoundarySpeciesAmountCodeGen::FunctionPtr>(lookupFunctionAddress(
                    "getFloatingSpeciesInitConcentrations"));
            modelResources->setFloatingSpeciesInitConcentrationsPtr = reinterpret_cast<SetBoundarySpeciesAmountCodeGen::FunctionPtr>(lookupFunctionAddress(
                    "setFloatingSpeciesInitConcentrations"));
            modelResources->getFloatingSpeciesInitAmountsPtr = reinterpret_cast<GetBoundarySpeciesAmountCodeGen::FunctionPtr>(lookupFunctionAddress(
                    "getFloatingSpeciesInitAmounts"));
            modelResources->setFloatingSpeciesInitAmountsPtr = reinterpret_cast<SetBoundarySpeciesAmountCodeGen::FunctionPtr>(lookupFunctionAddress(
                    "setFloatingSpeciesInitAmounts"));
            modelResources->getBoundarySpeciesInitConcentrationsPtr = reinterpret_cast<GetBoundarySpeciesAmountCodeGen::FunctionPtr>(lookupFunctionAddress(
                    "getBoundarySpeciesInitConcentrations"));
            modelResources->setBoundarySpeciesInitConcentrationsPtr = reinterpret_cast<SetBoundarySpeciesAmountCodeGen::FunctionPtr>(lookupFunctionAddress(
                    "setBoundarySpeciesInitConcentrations"));
            modelResources->getBoundarySpeciesInitAmountsPtr = reinterpret_cast<GetBoundarySpeciesAmountCodeGen::FunctionPtr>(lookupFunctionAddress(
                    "getBoundarySpeciesInitAmounts"));
            modelResources->setBoundarySpeciesInitAmountsPtr = reinterpret_cast<SetBoundarySpeciesAmountCodeGen::FunctionPtr>(lookupFunctionAddress(
                    "setBoundarySpeciesInitAmounts"));
            modelResources->getCompartmentInitVolumesPtr = reinterpret_cast<GetBoundarySpeciesAmountCodeGen::FunctionPtr>(lookupFunctionAddress(
                    "getCompartmentInitVolumes"));
            modelResources->setCompartmentInitVolumesPtr = reinterpret_cast<SetBoundarySpeciesAmountCodeGen::FunctionPtr>(lookupFunctionAddress(
                    "setCompartmentInitVolumes"));
            modelResources->getGlobalParameterInitValuePtr = reinterpret_cast<GetBoundarySpeciesAmountCodeGen::FunctionPtr>(lookupFunctionAddress(
                    "getGlobalParameterInitValue"));
            modelResources->setGlobalParameterInitValuePtr = reinterpret_cast<SetBoundarySpeciesAmountCodeGen::FunctionPtr>(lookupFunctionAddress(
                    "setGlobalParameterInitValue"));
        } else {
            modelResources->getFloatingSpeciesInitConcentrationsPtr = 0;
            modelResources->setFloatingSpeciesInitConcentrationsPtr = 0;

            modelResources->getFloatingSpeciesInitAmountsPtr = 0;
            modelResources->setFloatingSpeciesInitAmountsPtr = 0;

            modelResources->getBoundarySpeciesInitConcentrationsPtr = 0;
            modelResources->setBoundarySpeciesInitConcentrationsPtr = 0;

            modelResources->getBoundarySpeciesInitAmountsPtr = 0;
            modelResources->setBoundarySpeciesInitAmountsPtr = 0;

            modelResources->getCompartmentInitVolumesPtr = 0;
            modelResources->setCompartmentInitVolumesPtr = 0;

            modelResources->getGlobalParameterInitValuePtr = 0;
            modelResources->setGlobalParameterInitValuePtr = 0;
        }
    }

    std::string Jit::emitToString() {
        std::string str;
        llvm::raw_string_ostream os(str);
        os << *getModuleNonOwning();
        os.flush();
        return str;
    }

    llvm::raw_svector_ostream &Jit::getCompiledModuleStream() {
        return *compiledModuleStream;
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

