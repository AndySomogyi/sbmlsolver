/*
 * LLVMModelGenerator.cpp
 *
 * Created on: Jun 3, 2013
 *
 * Author: Andy Somogyi,
 *     email decode: V1 = "."; V2 = "@"; V3 = V1;
 *     andy V1 somogyi V2 gmail V3 com
 */
#include "LLVMModelGenerator.h"
#include "LLVMExecutableModel.h"
#include "ModelGeneratorContext.h"
#include "LLVMIncludes.h"


namespace rr
{

LLVMModelGenerator::LLVMModelGenerator()
{
}

LLVMModelGenerator::~LLVMModelGenerator()
{
}

bool LLVMModelGenerator::setTemporaryDirectory(const string& path)
{
    return true;
}

string LLVMModelGenerator::getTemporaryDirectory()
{
    return LLVMCompiler::gurgle();
}


ExecutableModel* LLVMModelGenerator::createModel(const std::string& sbml,
        uint options)
{
    bool computeAndAssignConsevationLaws =
            options & ModelGenerator::ComputeAndAssignConsevationLaws;

    ModelGeneratorContext context(sbml, computeAndAssignConsevationLaws);

    EvalInitialConditionsCodeGen::FunctionPtr evalInitialConditionsPtr =
            EvalInitialConditionsCodeGen(context).createFunction();

    EvalReactionRatesCodeGen::FunctionPtr evalReactionRatesPtr =
            EvalReactionRatesCodeGen(context).createFunction();

    GetBoundarySpeciesAmountCodeGen::FunctionPtr getBoundarySpeciesAmountPtr =
            GetBoundarySpeciesAmountCodeGen(context).createFunction();

    GetFloatingSpeciesAmountCodeGen::FunctionPtr getFloatingSpeciesAmountPtr =
            GetFloatingSpeciesAmountCodeGen(context).createFunction();

    GetBoundarySpeciesConcentrationCodeGen::FunctionPtr getBoundarySpeciesConcentrationPtr =
            GetBoundarySpeciesConcentrationCodeGen(context).createFunction();

    GetFloatingSpeciesConcentrationCodeGen::FunctionPtr getFloatingSpeciesConcentrationPtr =
            GetFloatingSpeciesConcentrationCodeGen(context).createFunction();

    GetCompartmentVolumeCodeGen::FunctionPtr getCompartmentVolumePtr =
            GetCompartmentVolumeCodeGen(context).createFunction();

    GetGlobalParameterCodeGen::FunctionPtr getGlobalParameterPtr =
            GetGlobalParameterCodeGen(context).createFunction();

    EvalRateRuleRatesCodeGen::FunctionPtr evalRateRuleRatesPtr =
            EvalRateRuleRatesCodeGen(context).createFunction();

    GetEventTriggerCodeGen::FunctionPtr getEventTriggerPtr =
            GetEventTriggerCodeGen(context).createFunction();

    GetEventPriorityCodeGen::FunctionPtr getEventPriorityPtr =
            GetEventPriorityCodeGen(context).createFunction();

    GetEventDelayCodeGen::FunctionPtr getEventDelayPtr =
            GetEventDelayCodeGen(context).createFunction();

    EventTriggerCodeGen::FunctionPtr eventTriggerPtr =
            EventTriggerCodeGen(context).createFunction();

    EventAssignCodeGen::FunctionPtr eventAssignPtr =
            EventAssignCodeGen(context).createFunction();


    // if anything up to this point throws an exception, thats OK, because
    // we have not allocated any memory yet that is not taken care of by
    // something else.
    // Now that everything that could have thrown would have thrown, we
    // can now create the model and set its fields.

    LLVMExecutableModel *exe = new LLVMExecutableModel();

    // * MOVE * the bits over from the context to the exe model.
    context.stealThePeach(&exe->symbols, &exe->context, &exe->executionEngine,
            &exe->errStr);

    exe->symbols->initAllocModelDataBuffers(exe->modelData);
    LLVMModelData::clone(&exe->modelDataCopy, &exe->modelData);

    exe->evalInitialConditionsPtr = evalInitialConditionsPtr;
    exe->evalReactionRatesPtr = evalReactionRatesPtr;
    exe->getBoundarySpeciesAmountPtr = getBoundarySpeciesAmountPtr;
    exe->getFloatingSpeciesAmountPtr = getFloatingSpeciesAmountPtr;
    exe->getBoundarySpeciesConcentrationPtr = getBoundarySpeciesConcentrationPtr;
    exe->getFloatingSpeciesConcentrationPtr = getFloatingSpeciesConcentrationPtr;
    exe->getCompartmentVolumePtr = getCompartmentVolumePtr;
    exe->getGlobalParameterPtr = getGlobalParameterPtr;
    exe->evalRateRuleRatesPtr = evalRateRuleRatesPtr;
    exe->getEventTriggerPtr = getEventTriggerPtr;
    exe->getEventPriorityPtr = getEventPriorityPtr;
    exe->getEventDelayPtr = getEventDelayPtr;
    exe->eventTriggerPtr = eventTriggerPtr;
    exe->eventAssignPtr = eventAssignPtr;

    return exe;
}

Compiler* LLVMModelGenerator::getCompiler()
{
    return &compiler;
}

bool LLVMModelGenerator::setCompiler(const string& compiler)
{
    return true;
}

/************ LLVM Utility Functions, TODO: Move To Separate File ************/

/**
 * C++ 11 style to_string for LLVM types
 */
std::string to_string(const llvm::Value *value)
{
    std::string str;
    llvm::raw_string_ostream stream(str);
    value->print(stream);
    return str;
}

} /* namespace rr */


