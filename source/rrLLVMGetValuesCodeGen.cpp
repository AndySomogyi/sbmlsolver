/*
 * rrLLVMGetBoundarySpeciesAmountsCodeGen.cpp
 *
 *  Created on: Jul 27, 2013
 *      Author: andy
 */

#include "rrLLVMGetValuesCodeGen.h"
#include "rrLLVMException.h"
#include "rrLLVMModelDataSymbolResolver.h"
#include "rrLogger.h"

#include <Poco/Logger.h>
#include <vector>

using namespace llvm;
using namespace std;

namespace rr
{

const char* LLVMGetBoundarySpeciesAmountCodeGen::FunctionName = "getBoundarySpeciesAmount";
const char* LLVMGetBoundarySpeciesAmountCodeGen::IndexArgName = "boundarySpeciesIndex";

LLVMGetBoundarySpeciesAmountCodeGen::LLVMGetBoundarySpeciesAmountCodeGen(
        const LLVMModelGeneratorContext &mgc) :
        LLVMGetValueCodeGenBase<LLVMGetBoundarySpeciesAmountCodeGen, true>(mgc)
{
}


const char* LLVMGetFloatingSpeciesAmountCodeGen::FunctionName = "getFloatingSpeciesAmount";
const char* LLVMGetFloatingSpeciesAmountCodeGen::IndexArgName = "floatingSpeciesIndex";

LLVMGetFloatingSpeciesAmountCodeGen::LLVMGetFloatingSpeciesAmountCodeGen(
        const LLVMModelGeneratorContext &mgc) :
        LLVMGetValueCodeGenBase<LLVMGetFloatingSpeciesAmountCodeGen, true>(mgc)
{
}

const char* LLVMGetBoundarySpeciesConcentrationCodeGen::FunctionName = "getBoundarySpeciesConcentration";
const char* LLVMGetBoundarySpeciesConcentrationCodeGen::IndexArgName = "boundarySpeciesIndex";

LLVMGetBoundarySpeciesConcentrationCodeGen::LLVMGetBoundarySpeciesConcentrationCodeGen(
        const LLVMModelGeneratorContext &mgc) :
        LLVMGetValueCodeGenBase<LLVMGetBoundarySpeciesConcentrationCodeGen, false>(mgc)
{
}


const char* LLVMGetFloatingSpeciesConcentrationCodeGen::FunctionName = "getFloatingSpeciesConcentration";
const char* LLVMGetFloatingSpeciesConcentrationCodeGen::IndexArgName = "floatingSpeciesIndex";

LLVMGetFloatingSpeciesConcentrationCodeGen::LLVMGetFloatingSpeciesConcentrationCodeGen(
        const LLVMModelGeneratorContext &mgc) :
        LLVMGetValueCodeGenBase<LLVMGetFloatingSpeciesConcentrationCodeGen, false>(mgc)
{
}





} /* namespace rr */

