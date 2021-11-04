//
// Created by Ciaran on 25/10/2021.
//

#include <stdint.h>
#include <rrRoadRunnerOptions.h>
#include "Jit.h"
#include "ModelResources.h"
#include "rrRoadRunnerOptions.h"
#include "llvm/Support/TargetRegistry.h"

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

        std::string stringifiedModule = postOptModuleStream->str().str();
        if (stringifiedModule.empty()){
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


    void Jit::mapFunctionsToAddresses(std::shared_ptr<ModelResources> &rc, std::uint32_t options) {

        rc->evalInitialConditionsPtr = (EvalInitialConditionsCodeGen::FunctionPtr) lookupFunctionAddress(
                "evalInitialConditions");

        rc->evalReactionRatesPtr = (EvalReactionRatesCodeGen::FunctionPtr) lookupFunctionAddress("evalReactionRates");

        rc->getBoundarySpeciesAmountPtr = (GetBoundarySpeciesAmountCodeGen::FunctionPtr) lookupFunctionAddress(
                "getBoundarySpeciesAmount");

        rc->getFloatingSpeciesAmountPtr = (GetFloatingSpeciesAmountCodeGen::FunctionPtr) lookupFunctionAddress(
                "getFloatingSpeciesAmount");

        rc->getBoundarySpeciesConcentrationPtr = (GetBoundarySpeciesConcentrationCodeGen::FunctionPtr) lookupFunctionAddress(
                "getBoundarySpeciesConcentration");

        rc->getFloatingSpeciesConcentrationPtr = (GetFloatingSpeciesAmountCodeGen::FunctionPtr) lookupFunctionAddress(
                "getFloatingSpeciesConcentration");

        rc->getCompartmentVolumePtr = (GetCompartmentVolumeCodeGen::FunctionPtr) lookupFunctionAddress(
                "getCompartmentVolume");

        rc->getGlobalParameterPtr = (GetGlobalParameterCodeGen::FunctionPtr) lookupFunctionAddress(
                "getGlobalParameter");

        rc->evalRateRuleRatesPtr = (EvalRateRuleRatesCodeGen::FunctionPtr) lookupFunctionAddress("evalRateRuleRates");

        rc->getEventTriggerPtr = (GetEventTriggerCodeGen::FunctionPtr) lookupFunctionAddress("getEventTrigger");

        rc->getEventPriorityPtr = (GetEventPriorityCodeGen::FunctionPtr) lookupFunctionAddress("getEventPriority");

        rc->getEventDelayPtr = (GetEventDelayCodeGen::FunctionPtr) lookupFunctionAddress("getEventDelay");

        rc->eventTriggerPtr = (EventTriggerCodeGen::FunctionPtr) lookupFunctionAddress("eventTrigger");

        rc->eventAssignPtr = (EventAssignCodeGen::FunctionPtr) lookupFunctionAddress("eventAssign");

        rc->evalVolatileStoichPtr = (EvalVolatileStoichCodeGen::FunctionPtr) lookupFunctionAddress(
                "evalVolatileStoich");

        rc->evalConversionFactorPtr = (EvalConversionFactorCodeGen::FunctionPtr) lookupFunctionAddress(
                "evalConversionFactor");
        if (options & LoadSBMLOptions::READ_ONLY) {
            rc->setBoundarySpeciesAmountPtr = 0;
            rc->setBoundarySpeciesConcentrationPtr = 0;
            rc->setFloatingSpeciesConcentrationPtr = 0;
            rc->setCompartmentVolumePtr = 0;
            rc->setFloatingSpeciesAmountPtr = 0;
            rc->setGlobalParameterPtr = 0;
        } else {
            rc->setBoundarySpeciesAmountPtr = (SetBoundarySpeciesAmountCodeGen::FunctionPtr) lookupFunctionAddress(
                    "setBoundarySpeciesAmount");

            rc->setBoundarySpeciesConcentrationPtr = (SetBoundarySpeciesAmountCodeGen::FunctionPtr) lookupFunctionAddress(
                    "setBoundarySpeciesConcentration");

            rc->setFloatingSpeciesConcentrationPtr = (SetBoundarySpeciesAmountCodeGen::FunctionPtr) lookupFunctionAddress(
                    "setFloatingSpeciesConcentration");

            rc->setCompartmentVolumePtr = (SetBoundarySpeciesAmountCodeGen::FunctionPtr) lookupFunctionAddress(
                    "setCompartmentVolume");

            rc->setBoundarySpeciesAmountPtr = (SetBoundarySpeciesAmountCodeGen::FunctionPtr) lookupFunctionAddress(
                    "setBoundarySpeciesAmount");

            rc->setFloatingSpeciesAmountPtr = (SetBoundarySpeciesAmountCodeGen::FunctionPtr) lookupFunctionAddress(
                    "setFloatingSpeciesAmount");

            rc->setGlobalParameterPtr = (SetGlobalParameterCodeGen::FunctionPtr) lookupFunctionAddress(
                    "setGlobalParameter");
        }

        if (options & LoadSBMLOptions::MUTABLE_INITIAL_CONDITIONS) {
            rc->getFloatingSpeciesInitConcentrationsPtr = (GetBoundarySpeciesAmountCodeGen::FunctionPtr) lookupFunctionAddress(
                    "getFloatingSpeciesInitConcentrations");
            rc->setFloatingSpeciesInitConcentrationsPtr = (SetBoundarySpeciesAmountCodeGen::FunctionPtr) lookupFunctionAddress(
                    "setFloatingSpeciesInitConcentrations");

            rc->getFloatingSpeciesInitAmountsPtr = (GetBoundarySpeciesAmountCodeGen::FunctionPtr) lookupFunctionAddress(
                    "getFloatingSpeciesInitAmounts");
            rc->setFloatingSpeciesInitAmountsPtr = (SetBoundarySpeciesAmountCodeGen::FunctionPtr) lookupFunctionAddress(
                    "setFloatingSpeciesInitAmounts");

            rc->getBoundarySpeciesInitConcentrationsPtr = (GetBoundarySpeciesAmountCodeGen::FunctionPtr) lookupFunctionAddress(
                    "getBoundarySpeciesInitConcentrations");
            rc->setBoundarySpeciesInitConcentrationsPtr = (SetBoundarySpeciesAmountCodeGen::FunctionPtr) lookupFunctionAddress(
                    "setBoundarySpeciesInitConcentrations");

            rc->getBoundarySpeciesInitAmountsPtr = (GetBoundarySpeciesAmountCodeGen::FunctionPtr) lookupFunctionAddress(
                    "getBoundarySpeciesInitAmounts");
            rc->setBoundarySpeciesInitAmountsPtr = (SetBoundarySpeciesAmountCodeGen::FunctionPtr) lookupFunctionAddress(
                    "setBoundarySpeciesInitAmounts");

            rc->getCompartmentInitVolumesPtr = (GetBoundarySpeciesAmountCodeGen::FunctionPtr) lookupFunctionAddress(
                    "getCompartmentInitVolumes");
            rc->setCompartmentInitVolumesPtr = (SetBoundarySpeciesAmountCodeGen::FunctionPtr) lookupFunctionAddress(
                    "setCompartmentInitVolumes");

            rc->getGlobalParameterInitValuePtr = (GetBoundarySpeciesAmountCodeGen::FunctionPtr) lookupFunctionAddress(
                    "getGlobalParameterInitValue");
            rc->setGlobalParameterInitValuePtr = (SetBoundarySpeciesAmountCodeGen::FunctionPtr) lookupFunctionAddress(
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

    std::string Jit::getDefaultTargetTriple() const {
        return llvm::sys::getDefaultTargetTriple();
    }


    const llvm::Target* Jit::getDefaultTargetMachine() const {
        std::string err;
        const llvm::Target* target = llvm::TargetRegistry::lookupTarget(getDefaultTargetTriple(), err);

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

