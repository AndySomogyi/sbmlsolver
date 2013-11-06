/*
 * GetValuesCodeGen.cpp
 *
 *  Created on: Jul 27, 2013
 *      Author: andy
 */
#pragma hdrstop
#include "GetInitialValuesCodeGen.h"
#include "LLVMException.h"
#include "ModelDataSymbolResolver.h"
#include "rrLogger.h"

#include <Poco/Logger.h>
#include <vector>

using namespace llvm;
using namespace std;

namespace rrllvm
{



const char* GetFloatingSpeciesInitConcentrationCodeGen::FunctionName = "getFloatingSpeciesInitConcentration";
const char* GetFloatingSpeciesInitConcentrationCodeGen::IndexArgName = "floatingSpeciesIndex";

GetFloatingSpeciesInitConcentrationCodeGen::GetFloatingSpeciesInitConcentrationCodeGen(
        const ModelGeneratorContext &mgc) :
        GetInitialValueCodeGenBase<GetFloatingSpeciesInitConcentrationCodeGen, false>(mgc)
{
}

const char* GetCompartmentInitVolumeCodeGen::FunctionName = "getCompartmentInitVolume";
const char* GetCompartmentInitVolumeCodeGen::IndexArgName = "compartmentIndex";

GetCompartmentInitVolumeCodeGen::GetCompartmentInitVolumeCodeGen(
        const ModelGeneratorContext &mgc) :
        GetInitialValueCodeGenBase<GetCompartmentInitVolumeCodeGen, false>(mgc)
{
}






} /* namespace rr */

