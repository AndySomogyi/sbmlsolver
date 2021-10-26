//
// Created by Ciaran on 25/10/2021.
//

#include <rrRoadRunnerOptions.h>
#include "Jit.h"
#include "ModelResources.h"
#include "rrRoadRunnerOptions.h"

using namespace rr;

namespace rrllvm {

    Jit::Jit()
            : context(std::make_unique<llvm::LLVMContext>()),
              module(std::make_unique<llvm::Module>("LLVM Module", *context)),
              moduleNonOwning(module.get()), /*Maintain a weak ref so we don't lose our handle to the module*/
              builder(std::make_unique<llvm::IRBuilder<>>(*context)) {
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

        Jit::createLibraryFunctions();
    }

    llvm::Module *Jit::getModuleNonOwning() {
        return moduleNonOwning;
    }

    llvm::LLVMContext *Jit::getContextNonOwning() {
        return context.get();
    }

    llvm::IRBuilder<> *Jit::getBuilderNonOwning() {
        return builder.get();
    }


    void Jit::transferObjectsToResources(std::shared_ptr<rrllvm::ModelResources> rc) {
        rc->context = std::move(context);
        context = nullptr;

//        rc->symbols = symbols;
//        symbols = nullptr;
//        rc->executionEngine = std::move(executionEngine);
//        executionEngine = nullptr;
//        rc->random = random;
//        random = nullptr;
//        rc->errStr = errString;
//        errString = nullptr;
    }


    void Jit::createLibraryFunction(llvm::LibFunc funcId, llvm::FunctionType *funcType) {
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

    void Jit::createLibraryFunctions() {
        using namespace llvm;
//        LLVMContext &context = module->getContext();
        Type *double_type = Type::getDoubleTy(*context);
        Type *args_d1[] = {double_type};
        Type *args_d2[] = {double_type, double_type};

        /// double pow(double x, double y);
        createLibraryFunction(LibFunc_pow,
                              FunctionType::get(double_type, args_d2, false));

        /// double fabs(double x);
        createLibraryFunction(LibFunc_fabs,
                              FunctionType::get(double_type, args_d1, false));

        /// double acos(double x);
        createLibraryFunction(LibFunc_acos,
                              FunctionType::get(double_type, args_d1, false));

        /// double asin(double x);
        createLibraryFunction(LibFunc_asin,
                              FunctionType::get(double_type, args_d1, false));

        /// double atan(double x);
        createLibraryFunction(LibFunc_atan,
                              FunctionType::get(double_type, args_d1, false));

        /// double ceil(double x);
        createLibraryFunction(LibFunc_ceil,
                              FunctionType::get(double_type, args_d1, false));

        /// double cos(double x);
        createLibraryFunction(LibFunc_cos,
                              FunctionType::get(double_type, args_d1, false));

        /// double cosh(double x);
        createLibraryFunction(LibFunc_cosh,
                              FunctionType::get(double_type, args_d1, false));

        /// double exp(double x);
        createLibraryFunction(LibFunc_exp,
                              FunctionType::get(double_type, args_d1, false));

        /// double floor(double x);
        createLibraryFunction(LibFunc_floor,
                              FunctionType::get(double_type, args_d1, false));

        /// double log(double x);
        createLibraryFunction(LibFunc_log,
                              FunctionType::get(double_type, args_d1, false));

        /// double log10(double x);
        createLibraryFunction(LibFunc_log10,
                              FunctionType::get(double_type, args_d1, false));

        /// double sin(double x);
        createLibraryFunction(LibFunc_sin,
                              FunctionType::get(double_type, args_d1, false));

        /// double sinh(double x);
        createLibraryFunction(LibFunc_sinh,
                              FunctionType::get(double_type, args_d1, false));

        /// double tan(double x);
        createLibraryFunction(LibFunc_tan,
                              FunctionType::get(double_type, args_d1, false));

        /// double tanh(double x);
        createLibraryFunction(LibFunc_tanh,
                              FunctionType::get(double_type, args_d1, false));

        /// double fmod(double x, double y);
        createLibraryFunction(LibFunc_fmod,
                              FunctionType::get(double_type, args_d2, false));
    }

    void Jit::mapFunctionsToAddresses(std::shared_ptr<ModelResources> &rc, std::uint32_t options) {

        rc->evalInitialConditionsPtr = (EvalInitialConditionsCodeGen::FunctionPtr) getFunctionAddress(
                "evalInitialConditions");

        rc->evalReactionRatesPtr = (EvalReactionRatesCodeGen::FunctionPtr) getFunctionAddress("evalReactionRates");

        rc->getBoundarySpeciesAmountPtr = (GetBoundarySpeciesAmountCodeGen::FunctionPtr) getFunctionAddress(
                "getBoundarySpeciesAmount");

        rc->getFloatingSpeciesAmountPtr = (GetFloatingSpeciesAmountCodeGen::FunctionPtr) getFunctionAddress(
                "getFloatingSpeciesAmount");

        rc->getBoundarySpeciesConcentrationPtr = (GetBoundarySpeciesConcentrationCodeGen::FunctionPtr) getFunctionAddress(
                "getBoundarySpeciesConcentration");

        rc->getFloatingSpeciesConcentrationPtr = (GetFloatingSpeciesAmountCodeGen::FunctionPtr) getFunctionAddress(
                "getFloatingSpeciesConcentration");

        rc->getCompartmentVolumePtr = (GetCompartmentVolumeCodeGen::FunctionPtr) getFunctionAddress(
                "getCompartmentVolume");

        rc->getGlobalParameterPtr = (GetGlobalParameterCodeGen::FunctionPtr) getFunctionAddress("getGlobalParameter");

        rc->evalRateRuleRatesPtr = (EvalRateRuleRatesCodeGen::FunctionPtr) getFunctionAddress("evalRateRuleRates");

        rc->getEventTriggerPtr = (GetEventTriggerCodeGen::FunctionPtr) getFunctionAddress("getEventTrigger");

        rc->getEventPriorityPtr = (GetEventPriorityCodeGen::FunctionPtr) getFunctionAddress("getEventPriority");

        rc->getEventDelayPtr = (GetEventDelayCodeGen::FunctionPtr) getFunctionAddress("getEventDelay");

        rc->eventTriggerPtr = (EventTriggerCodeGen::FunctionPtr) getFunctionAddress("eventTrigger");

        rc->eventAssignPtr = (EventAssignCodeGen::FunctionPtr) getFunctionAddress("eventAssign");

        rc->evalVolatileStoichPtr = (EvalVolatileStoichCodeGen::FunctionPtr) getFunctionAddress("evalVolatileStoich");

        rc->evalConversionFactorPtr = (EvalConversionFactorCodeGen::FunctionPtr) getFunctionAddress(
                "evalConversionFactor");
        if (options & LoadSBMLOptions::READ_ONLY) {
            rc->setBoundarySpeciesAmountPtr = 0;
            rc->setBoundarySpeciesConcentrationPtr = 0;
            rc->setFloatingSpeciesConcentrationPtr = 0;
            rc->setCompartmentVolumePtr = 0;
            rc->setFloatingSpeciesAmountPtr = 0;
            rc->setGlobalParameterPtr = 0;
        } else {
            rc->setBoundarySpeciesAmountPtr = (SetBoundarySpeciesAmountCodeGen::FunctionPtr) getFunctionAddress(
                    "setBoundarySpeciesAmount");

            rc->setBoundarySpeciesConcentrationPtr = (SetBoundarySpeciesAmountCodeGen::FunctionPtr) getFunctionAddress(
                    "setBoundarySpeciesConcentration");

            rc->setFloatingSpeciesConcentrationPtr = (SetBoundarySpeciesAmountCodeGen::FunctionPtr) getFunctionAddress(
                    "setFloatingSpeciesConcentration");

            rc->setCompartmentVolumePtr = (SetBoundarySpeciesAmountCodeGen::FunctionPtr) getFunctionAddress(
                    "setCompartmentVolume");

            rc->setBoundarySpeciesAmountPtr = (SetBoundarySpeciesAmountCodeGen::FunctionPtr) getFunctionAddress(
                    "setBoundarySpeciesAmount");

            rc->setFloatingSpeciesAmountPtr = (SetBoundarySpeciesAmountCodeGen::FunctionPtr) getFunctionAddress(
                    "setFloatingSpeciesAmount");

            rc->setGlobalParameterPtr = (SetGlobalParameterCodeGen::FunctionPtr) getFunctionAddress(
                    "setGlobalParameter");
        }

        if (options & LoadSBMLOptions::MUTABLE_INITIAL_CONDITIONS) {
            rc->getFloatingSpeciesInitConcentrationsPtr = (GetBoundarySpeciesAmountCodeGen::FunctionPtr) getFunctionAddress(
                    "getFloatingSpeciesInitConcentrations");
            rc->setFloatingSpeciesInitConcentrationsPtr = (SetBoundarySpeciesAmountCodeGen::FunctionPtr) getFunctionAddress(
                    "setFloatingSpeciesInitConcentrations");

            rc->getFloatingSpeciesInitAmountsPtr = (GetBoundarySpeciesAmountCodeGen::FunctionPtr) getFunctionAddress(
                    "getFloatingSpeciesInitAmounts");
            rc->setFloatingSpeciesInitAmountsPtr = (SetBoundarySpeciesAmountCodeGen::FunctionPtr) getFunctionAddress(
                    "setFloatingSpeciesInitAmounts");

            rc->getBoundarySpeciesInitConcentrationsPtr = (GetBoundarySpeciesAmountCodeGen::FunctionPtr) getFunctionAddress(
                    "getBoundarySpeciesInitConcentrations");
            rc->setBoundarySpeciesInitConcentrationsPtr = (SetBoundarySpeciesAmountCodeGen::FunctionPtr) getFunctionAddress(
                    "setBoundarySpeciesInitConcentrations");

            rc->getBoundarySpeciesInitAmountsPtr = (GetBoundarySpeciesAmountCodeGen::FunctionPtr) getFunctionAddress(
                    "getBoundarySpeciesInitAmounts");
            rc->setBoundarySpeciesInitAmountsPtr = (SetBoundarySpeciesAmountCodeGen::FunctionPtr) getFunctionAddress(
                    "setBoundarySpeciesInitAmounts");

            rc->getCompartmentInitVolumesPtr = (GetBoundarySpeciesAmountCodeGen::FunctionPtr) getFunctionAddress(
                    "getCompartmentInitVolumes");
            rc->setCompartmentInitVolumesPtr = (SetBoundarySpeciesAmountCodeGen::FunctionPtr) getFunctionAddress(
                    "setCompartmentInitVolumes");

            rc->getGlobalParameterInitValuePtr = (GetBoundarySpeciesAmountCodeGen::FunctionPtr) getFunctionAddress(
                    "getGlobalParameterInitValue");
            rc->setGlobalParameterInitValuePtr = (SetBoundarySpeciesAmountCodeGen::FunctionPtr) getFunctionAddress(
                    "setGlobalParameterInitValue");
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


}

