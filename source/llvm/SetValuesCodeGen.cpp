/*
 * GetValuesCodeGen.cpp
 *
 *  Created on: Sept 16, 2013
 *      Author: andy
 */

#include "SetValuesCodeGen.h"
#include "LLVMException.h"
#include "ModelDataSymbolResolver.h"
#include "rrLogger.h"

#include <Poco/Logger.h>
#include <vector>

using namespace llvm;
using namespace std;

namespace rrllvm
{

const char* SetBoundarySpeciesAmountCodeGen::FunctionName = "setBoundarySpeciesAmount";
const char* SetBoundarySpeciesAmountCodeGen::IndexArgName = "boundarySpeciesIndex";

SetBoundarySpeciesAmountCodeGen::SetBoundarySpeciesAmountCodeGen(
        const ModelGeneratorContext &mgc) :
        SetValueCodeGenBase<SetBoundarySpeciesAmountCodeGen, true>(mgc)
{
}


const char* SetFloatingSpeciesAmountCodeGen::FunctionName = "setFloatingSpeciesAmount";
const char* SetFloatingSpeciesAmountCodeGen::IndexArgName = "floatingSpeciesIndex";

SetFloatingSpeciesAmountCodeGen::SetFloatingSpeciesAmountCodeGen(
        const ModelGeneratorContext &mgc) :
        SetValueCodeGenBase<SetFloatingSpeciesAmountCodeGen, true>(mgc)
{
}

const char* SetBoundarySpeciesConcentrationCodeGen::FunctionName = "setBoundarySpeciesConcentration";
const char* SetBoundarySpeciesConcentrationCodeGen::IndexArgName = "boundarySpeciesIndex";

SetBoundarySpeciesConcentrationCodeGen::SetBoundarySpeciesConcentrationCodeGen(
        const ModelGeneratorContext &mgc) :
        SetValueCodeGenBase<SetBoundarySpeciesConcentrationCodeGen, false>(mgc)
{
}


const char* SetFloatingSpeciesConcentrationCodeGen::FunctionName = "setFloatingSpeciesConcentration";
const char* SetFloatingSpeciesConcentrationCodeGen::IndexArgName = "floatingSpeciesIndex";

SetFloatingSpeciesConcentrationCodeGen::SetFloatingSpeciesConcentrationCodeGen(
        const ModelGeneratorContext &mgc) :
        SetValueCodeGenBase<SetFloatingSpeciesConcentrationCodeGen, false>(mgc)
{
}

const char* SetCompartmentVolumeCodeGen::FunctionName = "setCompartmentVolume";
const char* SetCompartmentVolumeCodeGen::IndexArgName = "compartmentIndex";

SetCompartmentVolumeCodeGen::SetCompartmentVolumeCodeGen(
        const ModelGeneratorContext &mgc) :
        SetValueCodeGenBase<SetCompartmentVolumeCodeGen, false>(mgc)
{
}


const char* SetGlobalParameterCodeGen::FunctionName = "setGlobalParameter";
const char* SetGlobalParameterCodeGen::IndexArgName = "globalParameterIndex";

SetGlobalParameterCodeGen::SetGlobalParameterCodeGen(
        const ModelGeneratorContext &mgc) :
        SetValueCodeGenBase<SetGlobalParameterCodeGen, false>(mgc)
{
}





} /* namespace rr */

