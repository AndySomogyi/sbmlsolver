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

static std::vector<std::string> getIds(const rr::LLVMModelDataSymbols::StringIntMap & m)
{
    vector<string> result;
    result.resize(m.size());
    for(rr::LLVMModelDataSymbols::StringIntMap::const_iterator i = m.begin();
            i != m.end(); i++)
    {
        result[i->second] = i->first;
    }
    return result;
}

namespace rr
{





LLVMModelDataSymbols::LLVMModelDataSymbols()
{
    // TODO Auto-generated constructor stub

}

LLVMModelDataSymbols::LLVMModelDataSymbols(const libsbml::Model *model,
        bool computeAndAssignConsevationLaws)
{
    modelName = model->getName();

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
        throw LLVMException("could not find compartment with id " + id, __FUNC__);
    }
}

int LLVMModelDataSymbols::getFloatingSpeciesIndex(
        const std::string& id) const
{
    StringIntMap::const_iterator i = floatingSpeciesMap.find(id);
    if(i != floatingSpeciesMap.end())
    {
        return i->second;
    }
    else
    {
        throw LLVMException("could not find floating species with id " + id, __FUNC__);
    }
}

int LLVMModelDataSymbols::getBoundarySpeciesIndex(
        const std::string& id) const
{
    StringIntMap::const_iterator i = boundarySpeciesMap.find(id);
    if(i != boundarySpeciesMap.end())
    {
        return i->second;
    }
    else
    {
        throw LLVMException("could not find boundary species with id " + id, __FUNC__);
    }
}


int LLVMModelDataSymbols::getGlobalParameterIndex(
        const std::string& id) const
{
    StringIntMap::const_iterator i = globalParametersMap.find(id);
    if(i != globalParametersMap.end())
    {
        return i->second;
    }
    else
    {
        throw LLVMException("could not find global parameter species with id " + id, __FUNC__);
    }
}

void LLVMModelDataSymbols::initAllocModelDataBuffers(ModelData& m) const
{
    // zero out the structure
    initModelData(m);

    // set the buffer sizes
    //mData.numIndependentSpecies         = ms.mNumIndependentSpecies;
    //mData.numDependentSpecies           = ms.mNumDependentSpecies;
    m.numGlobalParameters           = globalParametersMap.size();
    //mData.numReactions                  = ms.mReactionList.size();
    //mData.numEvents                     = ms.mNumEvents;
    m.numFloatingSpecies            = floatingSpeciesMap.size();
    //mData.numRateRules                  = ms.mRateRules.size();
    m.numCompartments               = compartmentsMap.size();
    m.numBoundarySpecies            = boundarySpeciesMap.size();
    //mData.srSize                        = ms.mNumModifiableSpeciesReferences;
    //mData.eventPrioritiesSize           = ms.mNumEvents;
    //mData.eventStatusArraySize          = ms.mNumEvents;
    //mData.previousEventStatusArraySize  = ms.mNumEvents;
    //mData.eventPersistentTypeSize       = ms.mNumEvents;
    //mData.eventTestsSize                = ms.mNumEvents;
    //mData.eventTypeSize                 = ms.mNumEvents;

    // allocate the data buffers
    allocModelDataBuffers(m, modelName);
}

std::vector<std::string> LLVMModelDataSymbols::getCompartmentIds() const
{
    return getIds(compartmentsMap);
}

std::vector<std::string> LLVMModelDataSymbols::getBoundarySpeciesIds() const
{
    return getIds(boundarySpeciesMap);
}

int LLVMModelDataSymbols::getFloatingSpeciesCompartmentIndex(
        const std::string& id) const
{
    int speciesIndex = getFloatingSpeciesIndex(id);
    return floatingSpeciesCompartments[speciesIndex];
}

int LLVMModelDataSymbols::getBoundarySpeciesCompartmentIndex(
        const std::string& id) const
{
    int speciesIndex = getBoundarySpeciesIndex(id);
    return boundarySpeciesCompartments[speciesIndex];
}

void LLVMModelDataSymbols::print() const
{
    for (StringIntMap::const_iterator i = floatingSpeciesMap.begin();
            i != floatingSpeciesMap.end(); i++)
    {
        cout << "float species id: " << i->first << ", index: " << i->second
                << "\n";
    }

    for (StringIntMap::const_iterator i = boundarySpeciesMap.begin();
            i != boundarySpeciesMap.end(); i++)
    {
        cout << "boundary species id: " << i->first << ", index: " << i->second
                << "\n";
    }

    for (StringIntMap::const_iterator i = compartmentsMap.begin();
            i != compartmentsMap.end(); i++)
    {
        cout << "compartment id: " << i->first << ", index: " << i->second
                << "\n";
    }

    for (StringIntMap::const_iterator i = globalParametersMap.begin();
            i != globalParametersMap.end(); i++)
    {
        cout << "global parameter id: " << i->first << ", index: " << i->second
                << "\n";
    }

}


std::vector<std::string> LLVMModelDataSymbols::getGlobalParameterIds() const
{
    return getIds(globalParametersMap);
}

std::vector<std::string> LLVMModelDataSymbols::getFloatingSpeciesIds() const
{
    return getIds(floatingSpeciesMap);
}







} /* namespace rr */
