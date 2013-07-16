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





/*
 This function is obsolete.. initialize all model data in roadrunner instead..
 all this does is print the size of the struct.
 cInitModel(&mData);

 csetCompartmentVolumes(&mData);
 cinitializeInitialConditions(&mData);
 csetParameterValues(&mData);
 csetBoundaryConditions(&mData);

 csetInitialConditions(&mData);

 cconvertToAmounts(&mData);
 cevalInitialAssignments(&mData);
 ccomputeRules(&mData, y);
 cevalInitialAssignments(&mData);
 double* values = cGetCurrentValues(&mData);
 cresetEvents(&mData);
 cInitializeRateRuleSymbols(&mData);
 cInitializeRates(&mData);
 cconvertToConcentrations(&mData);
 cconvertToConcentrations(&mData);
 ccomputeConservedTotals(&mData);
 cevalEvents(&mData, timeIn, oAmounts);
 ctestConstraints(&mData);
 cupdateDependentSpeciesValues(&mData, y_vec);
 cAssignRates_b(&mData, local_rates);
 ccomputeAllRatesOfChange(&mData);
 */


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
        ls::LibStructural*, NOMSupport*, bool forceReCompile,
        bool computeAndAssignConsevationLaws)
{
    LLVMModelGeneratorContext context(sbml, computeAndAssignConsevationLaws);

    LLVMModelSymbols::FunctionPtr evalInitialConditionsPtr =
            LLVMModelSymbols(context).createFunction();


    // if anything up to this point throws an exception, thats OK, because
    // we have not allocated any memory yet that is not taken care of by
    // something else.
    // Now that everything that could have thrown would have thrown, we
    // can now create the model and set its fields.

    LLVMExecutableModel *exe = new LLVMExecutableModel();

    context.stealThePeach(&exe->symbols, &exe->context, &exe->executionEngine,
            &exe->errStr);

    exe->evalInitialConditionsPtr = evalInitialConditionsPtr;

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


} /* namespace rr */
