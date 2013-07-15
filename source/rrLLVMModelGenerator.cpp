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
    return false;
}

string LLVMModelGenerator::getTemporaryDirectory()
{
    return "";
}

ExecutableModel* LLVMModelGenerator::createModel(const string& sbml,
        ls::LibStructural* ls, NOMSupport* nom, bool forceReCompile,
        bool computeAndAssignConsevationLaws)
{
    return 0;
}

Compiler* LLVMModelGenerator::getCompiler()
{
    return &compiler;
}

bool LLVMModelGenerator::setCompiler(const string& compiler)
{
    return false;
}


} /* namespace rr */
