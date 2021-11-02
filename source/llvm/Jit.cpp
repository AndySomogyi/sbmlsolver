//
// Created by Ciaran on 25/10/2021.
//

#include <stdint.h>
#include <rrRoadRunnerOptions.h>
#include "Jit.h"
#include "ModelResources.h"
#include "rrRoadRunnerOptions.h"

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

    llvm::raw_svector_ostream &Jit::getPostOptModuleStream() {
        return *postOptModuleStream;
    }


}

