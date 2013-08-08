/*
 * rrLLVMModelGenerator.cpp
 *
 * Created on: Jun 3, 2013
 *
 * Author: Andy Somogyi,
 *     email decode: V1 = "."; V2 = "@"; V3 = V1;
 *     andy V1 somogyi V2 gmail V3 com
 */
#include "rrLLVMModelGenerator.h"
#include "rrLLVMExecutableModel.h"
#include "rrLLVMModelGeneratorContext.h"
#include "rrLLVMIncludes.h"


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

ExecutableModel* LLVMModelGenerator::createModel(const string& sbml,
        ls::LibStructural*, bool forceReCompile,
        bool computeAndAssignConsevationLaws)
{
    return createModel(sbml, computeAndAssignConsevationLaws);
}

ExecutableModel* LLVMModelGenerator::createModel(const std::string& sbml,
        bool computeAndAssignConsevationLaws)
{
    LLVMModelGeneratorContext context(sbml, computeAndAssignConsevationLaws);

    LLVMEvalInitialConditionsCodeGen::FunctionPtr evalInitialConditionsPtr =
            LLVMEvalInitialConditionsCodeGen(context).createFunction();

    LLVMEvalReactionRatesCodeGen::FunctionPtr evalReactionRatesPtr =
            LLVMEvalReactionRatesCodeGen(context).createFunction();

    LLVMGetBoundarySpeciesAmountCodeGen::FunctionPtr getBoundarySpeciesAmountPtr =
            LLVMGetBoundarySpeciesAmountCodeGen(context).createFunction();

    LLVMGetFloatingSpeciesAmountCodeGen::FunctionPtr getFloatingSpeciesAmountPtr =
            LLVMGetFloatingSpeciesAmountCodeGen(context).createFunction();

    LLVMGetBoundarySpeciesConcentrationCodeGen::FunctionPtr getBoundarySpeciesConcentrationPtr =
            LLVMGetBoundarySpeciesConcentrationCodeGen(context).createFunction();

    LLVMGetFloatingSpeciesConcentrationCodeGen::FunctionPtr getFloatingSpeciesConcentrationPtr =
            LLVMGetFloatingSpeciesConcentrationCodeGen(context).createFunction();

    LLVMGetCompartmentVolumeCodeGen::FunctionPtr getCompartmentVolumePtr =
            LLVMGetCompartmentVolumeCodeGen(context).createFunction();

    LLVMGetGlobalParameterCodeGen::FunctionPtr getGlobalParameterPtr =
            LLVMGetGlobalParameterCodeGen(context).createFunction();

    LLVMEvalRateRuleRatesCodeGen::FunctionPtr evalRateRuleRatesPtr =
            LLVMEvalRateRuleRatesCodeGen(context).createFunction();


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


