/*
 * rrLLVMModelDataSymbols.cpp
 *
 *  Created on: Jun 28, 2013
 *      Author: andy
 */
#include "rrLLVMModelDataSymbols.h"
#include "lsLibStructural.h"
#include "rrLLVMException.h"
#include "rrOSSpecifics.h"

#include <sbml/Model.h>
#include <sbml/SBMLDocument.h>

#include <llvm/Support/raw_ostream.h>

#include <string>
#include <vector>
#include <sstream>

using namespace libsbml;
using namespace llvm;
using namespace std;

namespace rr
{

const char* LLVMModelDataSymbols::ModelDataName = "rr::ModelData";

LLVMModelDataSymbols::LLVMModelDataSymbols()
{
    // TODO Auto-generated constructor stub

}

LLVMModelDataSymbols::LLVMModelDataSymbols(const libsbml::Model *model,
        bool computeAndAssignConsevationLaws, IRBuilder<> *b) :
        builder(b),
        cachedStructValue(0)
{
    // get the compartments
    const ListOfCompartments *compartments = model->getListOfCompartments();
    for (int i = 0; i < compartments->size(); i++)
    {
        const Compartment *c = compartments->get(i);
        compartmentsMap.insert(StringIntPair(c->getId(), i));
    }

    // all the species
    const ListOfSpecies *species = model->getListOfSpecies();

    // get the floating species and set thier compartments
    // TODO: fix libstruct to return index array instead of id array
    ls::LibStructural structural(model);
    vector<string> reorderedList = computeAndAssignConsevationLaws ?
            structural.getReorderedSpecies() :
            structural.getSpecies();

    floatingSpeciesCompartments.resize(reorderedList.size());
    for(int i = 0; i < reorderedList.size(); i++)
    {
        const Species *s = species->get(reorderedList[i]);
        floatingSpeciesMap.insert(StringIntPair(s->getId(), i));
        floatingSpeciesCompartments[i] = compartmentsMap.find(s->getCompartment())->second;
    }

    // get the boundary species
    boundarySpeciesCompartments.resize(species->size() - floatingSpeciesCompartments.size());
    int j = 0;
    for (int i = 0; i < species->size(); i++)
    {
        const Species *s = species->get(i);
        if(s->getBoundaryCondition())
        {
            boundarySpeciesMap.insert(StringIntPair(s->getId(), j));
            boundarySpeciesCompartments[j] = compartmentsMap.find(s->getCompartment())->second;
            j++;
        }
    }

    // get the global parameters
    const ListOfParameters *parameters = model->getListOfParameters();
    for (int i = 0; i < parameters->size(); i++)
    {
        const Parameter *p = parameters->get(i);
        globalParametersMap.insert(StringIntPair(p->getId(), i));
    }

}

LLVMModelDataSymbols::~LLVMModelDataSymbols()
{
    // TODO Auto-generated destructor stub
}

llvm::Value* LLVMModelDataSymbols::createFloatSpeciesConcEP(llvm::Value* s,
        const std::string& id)
{
}

llvm::Value* LLVMModelDataSymbols::createFloatSpeciedAmtEP(llvm::Value* s,
        const std::string& id)
{
}

llvm::Value* LLVMModelDataSymbols::createFloatSpeciesConcFromAmtLoad(
        llvm::Value* s, const std::string& id)
{
}

llvm::Value* LLVMModelDataSymbols::createFloatSpeciesAmtFromConcLoad(
        llvm::Value* s, const std::string& id)
{
}

llvm::Value* LLVMModelDataSymbols::createGlobalParamEP(llvm::Value* s,
        const std::string& id)
{
}

int LLVMModelDataSymbols::getCompartmentIndex(
        const std::string& id) const
{
    StringIntMap::const_iterator i = compartmentsMap.find(id);
    if (i != compartmentsMap.end())
    {
        return i->second;
    }
    else
    {
        string s;
        raw_string_ostream err(s);
        err << "error in " << __FUNC__;
        err << "compartment id \"" << id << "\" not found in compartmentsMap";
        throw LLVMException(err.str());
    }
}

int LLVMModelDataSymbols::getFloatingSpeciesIndex(
        const std::string& allocator) const
{
}

int LLVMModelDataSymbols::getBoundarySpeciesIndex(
        const std::string& allocator) const
{
}

llvm::Value* LLVMModelDataSymbols::createGEP(llvm::Value* s,
        ModelDataFields field)
{
    Value *result = 0;
    validateStruct(s, __FUNC__);
    return builder->CreateStructGEP(s, field, "");
}

void LLVMModelDataSymbols::validateStruct(llvm::Value* s, const char* funcName)
{
    if (!builder)
    {
        string err = "error in ";
        err += funcName;
        err += ", builder is null";
        throw LLVMException(err);
    }

    if (s != cachedStructValue)
    {
        StructType *structType = dyn_cast<StructType>(s->getType());
        if (structType)
        {
            if (structType->getName().compare(ModelDataName) == 0)
            {
                cachedStructValue = s;
                return;
            }
        }
        std::string str;
        raw_string_ostream err(str);
        err << "error in " << funcName << ", "
                << "Invalid argument type, expected " << ModelDataName
                << ", but received ";
        s->getType()->print(err);
        throw LLVMException(err.str());
    }
}

} /* namespace rr */
