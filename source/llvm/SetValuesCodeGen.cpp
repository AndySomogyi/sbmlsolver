/*
 * GetValuesCodeGen.cpp
 *
 *  Created on: Sept 16, 2013
 *      Author: andy
 */
#pragma hdrstop
#include "SetValuesCodeGen.h"
#include "LLVMException.h"
#include "ModelDataSymbolResolver.h"
#include "rrLogger.h"

#include <Poco/Logger.h>
#include <vector>
#include <algorithm>

using namespace llvm;
using namespace std;



namespace rrllvm
{

/**
 * C++ 11 has the copy_if template, but for the time being, we need to be
 * compatible with older C++, so use our own filtering here.
 */
static std::vector<string> independentElements(const LLVMModelDataSymbols &dataSymbols,
        const std::vector<string> elements)
{
    std::vector<string> result;

    for(std::vector<string>::const_iterator i = elements.begin();
            i != elements.end(); ++i)
    {
        if (dataSymbols.isIndependentElement(*i))
        {
            result.push_back(*i);
        }
    }

    return result;
}

const char* SetBoundarySpeciesAmountCodeGen::FunctionName = "setBoundarySpeciesAmount";
const char* SetBoundarySpeciesAmountCodeGen::IndexArgName = "boundarySpeciesIndex";

SetBoundarySpeciesAmountCodeGen::SetBoundarySpeciesAmountCodeGen(
        const ModelGeneratorContext &mgc) :
        SetValueCodeGenBase<SetBoundarySpeciesAmountCodeGen, true>(mgc)
{
}

std::vector<string> SetBoundarySpeciesAmountCodeGen::getIds()
{
    return independentElements(dataSymbols, dataSymbols.getBoundarySpeciesIds());
}


const char* SetFloatingSpeciesAmountCodeGen::FunctionName = "setFloatingSpeciesAmount";
const char* SetFloatingSpeciesAmountCodeGen::IndexArgName = "floatingSpeciesIndex";

SetFloatingSpeciesAmountCodeGen::SetFloatingSpeciesAmountCodeGen(
        const ModelGeneratorContext &mgc) :
        SetValueCodeGenBase<SetFloatingSpeciesAmountCodeGen, true>(mgc)
{
}

std::vector<string> SetFloatingSpeciesAmountCodeGen::getIds()
{
    return independentElements(dataSymbols, dataSymbols.getFloatingSpeciesIds());
}


const char* SetBoundarySpeciesConcentrationCodeGen::FunctionName = "setBoundarySpeciesConcentration";
const char* SetBoundarySpeciesConcentrationCodeGen::IndexArgName = "boundarySpeciesIndex";

SetBoundarySpeciesConcentrationCodeGen::SetBoundarySpeciesConcentrationCodeGen(
        const ModelGeneratorContext &mgc) :
        SetValueCodeGenBase<SetBoundarySpeciesConcentrationCodeGen, false>(mgc)
{
}

std::vector<string> SetBoundarySpeciesConcentrationCodeGen::getIds()
{
    return independentElements(dataSymbols, dataSymbols.getBoundarySpeciesIds());
}


const char* SetFloatingSpeciesConcentrationCodeGen::FunctionName = "setFloatingSpeciesConcentration";
const char* SetFloatingSpeciesConcentrationCodeGen::IndexArgName = "floatingSpeciesIndex";

SetFloatingSpeciesConcentrationCodeGen::SetFloatingSpeciesConcentrationCodeGen(
        const ModelGeneratorContext &mgc) :
        SetValueCodeGenBase<SetFloatingSpeciesConcentrationCodeGen, false>(mgc)
{
}

std::vector<string> SetFloatingSpeciesConcentrationCodeGen::getIds()
{
    return independentElements(dataSymbols, dataSymbols.getFloatingSpeciesIds());
}


const char* SetCompartmentVolumeCodeGen::FunctionName = "setCompartmentVolume";
const char* SetCompartmentVolumeCodeGen::IndexArgName = "compartmentIndex";

SetCompartmentVolumeCodeGen::SetCompartmentVolumeCodeGen(
        const ModelGeneratorContext &mgc) :
        SetValueCodeGenBase<SetCompartmentVolumeCodeGen, false>(mgc)
{
}

std::vector<string> SetCompartmentVolumeCodeGen::getIds()
{
    return independentElements(dataSymbols, dataSymbols.getCompartmentIds());
}


const char* SetGlobalParameterCodeGen::FunctionName = "setGlobalParameter";
const char* SetGlobalParameterCodeGen::IndexArgName = "globalParameterIndex";

SetGlobalParameterCodeGen::SetGlobalParameterCodeGen(
        const ModelGeneratorContext &mgc) :
        SetValueCodeGenBase<SetGlobalParameterCodeGen, false>(mgc)
{
}

std::vector<string> SetGlobalParameterCodeGen::getIds()
{
    return independentElements(dataSymbols, dataSymbols.getGlobalParameterIds());
}

} /* namespace rr */

