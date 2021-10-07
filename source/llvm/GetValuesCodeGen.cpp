/*
 * GetValuesCodeGen.cpp
 *
 *  Created on: Jul 27, 2013
 *      Author: andy
 */
#pragma hdrstop
#include "GetValuesCodeGen.h"
#include "LLVMException.h"
#include "ModelDataSymbolResolver.h"
#include "rrLogger.h"

#include <Poco/Logger.h>
#include <vector>

using namespace llvm;


namespace rrllvm
{

const char* GetBoundarySpeciesAmountCodeGen::FunctionName = "getBoundarySpeciesAmount";
const char* GetBoundarySpeciesAmountCodeGen::IndexArgName = "boundarySpeciesIndex";

GetBoundarySpeciesAmountCodeGen::GetBoundarySpeciesAmountCodeGen(
        const ModelGeneratorContext &mgc) :
        GetValueCodeGenBase<GetBoundarySpeciesAmountCodeGen, true>(mgc)
{
}


const char* GetFloatingSpeciesAmountCodeGen::FunctionName = "getFloatingSpeciesAmount";
const char* GetFloatingSpeciesAmountCodeGen::IndexArgName = "floatingSpeciesIndex";

GetFloatingSpeciesAmountCodeGen::GetFloatingSpeciesAmountCodeGen(
        const ModelGeneratorContext &mgc) :
        GetValueCodeGenBase<GetFloatingSpeciesAmountCodeGen, true>(mgc)
{
}

const char* GetBoundarySpeciesConcentrationCodeGen::FunctionName = "getBoundarySpeciesConcentration";
const char* GetBoundarySpeciesConcentrationCodeGen::IndexArgName = "boundarySpeciesIndex";

GetBoundarySpeciesConcentrationCodeGen::GetBoundarySpeciesConcentrationCodeGen(
        const ModelGeneratorContext &mgc) :
        GetValueCodeGenBase<GetBoundarySpeciesConcentrationCodeGen, false>(mgc)
{
}


const char* GetFloatingSpeciesConcentrationCodeGen::FunctionName = "getFloatingSpeciesConcentration";
const char* GetFloatingSpeciesConcentrationCodeGen::IndexArgName = "floatingSpeciesIndex";

GetFloatingSpeciesConcentrationCodeGen::GetFloatingSpeciesConcentrationCodeGen(
        const ModelGeneratorContext &mgc) :
        GetValueCodeGenBase<GetFloatingSpeciesConcentrationCodeGen, false>(mgc)
{
}

const char* GetCompartmentVolumeCodeGen::FunctionName = "getCompartmentVolume";
const char* GetCompartmentVolumeCodeGen::IndexArgName = "compartmentIndex";

GetCompartmentVolumeCodeGen::GetCompartmentVolumeCodeGen(
        const ModelGeneratorContext &mgc) :
        GetValueCodeGenBase<GetCompartmentVolumeCodeGen, false>(mgc)
{
}


const char* GetGlobalParameterCodeGen::FunctionName = "getGlobalParameter";
const char* GetGlobalParameterCodeGen::IndexArgName = "globalParameterIndex";

GetGlobalParameterCodeGen::GetGlobalParameterCodeGen(
        const ModelGeneratorContext &mgc) :
        GetValueCodeGenBase<GetGlobalParameterCodeGen, false>(mgc)
{
}





} /* namespace rr */

