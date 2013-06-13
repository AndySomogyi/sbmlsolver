/*
 * rrLLVMModelGenerator.cpp
 *
 *  Created on: Jun 3, 2013
 *      Author: andy
 */

#include "rrLLVMModelGenerator.h"

#include "llvm/Analysis/Passes.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/JIT.h"
#include "llvm/DataLayout.h"
#include "llvm/DerivedTypes.h"
#include "llvm/IRBuilder.h"
#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "llvm/PassManager.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Transforms/Scalar.h"


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
    // TODO Auto-generated constructor stub

}

LLVMModelGenerator::~LLVMModelGenerator()
{
    // TODO Auto-generated destructor stub
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
        LibStructural* ls, NOMSupport* nom, bool forceReCompile,
        bool computeAndAssignConsevationLaws)
{
    return 0;
}

Compiler* LLVMModelGenerator::getCompiler()
{
    return 0;
}

bool LLVMModelGenerator::setCompiler(const string& compiler)
{
    return false;
}

//void CModelGenerator::writeSetCompartmentVolumes(CodeBuilder& ignore)
//{
//    mHeader.AddFunctionExport("void", "setCompartmentVolumes(ModelData* md)");
//    mSource << "void setCompartmentVolumes(ModelData* md)\n{";
//
//    for (int i = 0; i < ms.mCompartmentList.size(); i++)
//    {
//        mSource<<append("\n\t" + convertSymbolToC(ms.mCompartmentList[i].name) + " = (double)" +
//                  writeDouble(ms.mCompartmentList[i].value) + ";" + NL());
//
//        // at this point we also have to take care of all initial assignments for compartments as well as
//        // the assignment rules on compartments ... otherwise we are in trouble :)
//        stack<string> initializations = mNOM->getMatchForSymbol(ms.mCompartmentList[i].name);
//        while (initializations.size() > 0)
//        {
//            string term(initializations.top());
//            string sub = substituteTerms(ms.mNumReactions, "", term);
//            mSource<<append("\t" + sub + ";" + NL());
//            initializations.pop();
//        }
//    }
//
//    mSource<<append("}" + NL() + NL());
//}

} /* namespace rr */
