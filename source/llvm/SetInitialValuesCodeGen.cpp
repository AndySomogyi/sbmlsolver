/*
 * GetValuesCodeGen.cpp
 *
 *  Created on: Sept 16, 2013
 *      Author: andy
 */
#pragma hdrstop
#include "SetInitialValuesCodeGen.h"
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
static std::vector<std::pair<uint, std::string> > independentElements(const LLVMModelDataSymbols &dataSymbols,
        const std::vector<string> elements)
{
    std::vector<std::pair<uint, std::string> > result;

    for(std::vector<string>::const_iterator i = elements.begin();
            i != elements.end(); ++i)
    {
        if (dataSymbols.isIndependentElement(*i) || dataSymbols.isConservedMoiety(*i))
        {
            pair<uint, string> value(i - elements.begin(), dataSymbols.createInitSymbol(*i));
            result.push_back(value);
        }
    }

    return result;
}



const char* SetFloatingSpeciesInitialConcentrationCodeGen::FunctionName = "setFloatingSpeciesInitialConcentration";
const char* SetFloatingSpeciesInitialConcentrationCodeGen::IndexArgName = "floatingSpeciesIndex";

SetFloatingSpeciesInitialConcentrationCodeGen::SetFloatingSpeciesInitialConcentrationCodeGen(
        const ModelGeneratorContext &mgc) :
        SetInitialValueCodeGenBase<SetFloatingSpeciesInitialConcentrationCodeGen, false>(mgc)
{
}

std::vector<std::pair<uint, std::string> > SetFloatingSpeciesInitialConcentrationCodeGen::getIds()
{
    return independentElements(dataSymbols, dataSymbols.getFloatingSpeciesIds());
}



} /* namespace rr */

