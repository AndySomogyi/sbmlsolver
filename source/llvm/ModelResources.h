/*
 * ModelResources.h
 *
 *  Created on: Aug 28, 2013
 *      Author: andy
 */

#ifndef CACHEDMODEL_H_
#define CACHEDMODEL_H_

#include "LLVMExecutableModel.h"

namespace rrllvm
{

class ModelResources
{
public:
    ModelResources();
    ~ModelResources();

    const LLVMModelDataSymbols *symbols;
    const llvm::LLVMContext *context;
    const llvm::ExecutionEngine *executionEngine;
    const std::string *errStr;

    EvalInitialConditionsCodeGen::FunctionPtr evalInitialConditionsPtr;
    EvalReactionRatesCodeGen::FunctionPtr evalReactionRatesPtr;
    GetBoundarySpeciesAmountCodeGen::FunctionPtr getBoundarySpeciesAmountPtr;
    GetFloatingSpeciesAmountCodeGen::FunctionPtr getFloatingSpeciesAmountPtr;
    GetBoundarySpeciesConcentrationCodeGen::FunctionPtr getBoundarySpeciesConcentrationPtr;
    GetFloatingSpeciesConcentrationCodeGen::FunctionPtr getFloatingSpeciesConcentrationPtr;
    GetCompartmentVolumeCodeGen::FunctionPtr getCompartmentVolumePtr;
    GetGlobalParameterCodeGen::FunctionPtr getGlobalParameterPtr;
    EvalRateRuleRatesCodeGen::FunctionPtr evalRateRuleRatesPtr;
    GetEventTriggerCodeGen::FunctionPtr getEventTriggerPtr;
    GetEventPriorityCodeGen::FunctionPtr getEventPriorityPtr;
    GetEventDelayCodeGen::FunctionPtr getEventDelayPtr;
    EventTriggerCodeGen::FunctionPtr eventTriggerPtr;
    EventAssignCodeGen::FunctionPtr eventAssignPtr;
    EvalVolatileStoichCodeGen::FunctionPtr evalVolatileStoichPtr;
    EvalConversionFactorCodeGen::FunctionPtr evalConversionFactorPtr;
    SetBoundarySpeciesAmountCodeGen::FunctionPtr setBoundarySpeciesAmountPtr;
    SetFloatingSpeciesAmountCodeGen::FunctionPtr setFloatingSpeciesAmountPtr;
    SetBoundarySpeciesConcentrationCodeGen::FunctionPtr setBoundarySpeciesConcentrationPtr;
    SetFloatingSpeciesConcentrationCodeGen::FunctionPtr setFloatingSpeciesConcentrationPtr;
    SetCompartmentVolumeCodeGen::FunctionPtr setCompartmentVolumePtr;
    SetGlobalParameterCodeGen::FunctionPtr setGlobalParameterPtr;

    // init value accessors
    SetFloatingSpeciesInitConcentrationCodeGen::FunctionPtr setFloatingSpeciesInitConcentrationsPtr;
    SetCompartmentInitVolumeCodeGen::FunctionPtr setCompartmentInitVolumesPtr;
    GetFloatingSpeciesInitConcentrationCodeGen::FunctionPtr getFloatingSpeciesInitConcentrationsPtr;
    GetCompartmentInitVolumeCodeGen::FunctionPtr getCompartmentInitVolumesPtr;
};

} /* namespace rrllvm */
#endif /* CACHEDMODEL_H_ */
