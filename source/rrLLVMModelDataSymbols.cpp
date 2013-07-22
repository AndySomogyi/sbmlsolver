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
#include "rrLogger.h"
#include "rrSparse.h"
#include "rrStringUtils.h"

#include <Poco/LogStream.h>
#include <sbml/Model.h>
#include <sbml/SBMLDocument.h>

#include <llvm/Support/raw_ostream.h>

#include <string>
#include <vector>
#include <sstream>

using namespace libsbml;
using namespace llvm;
using namespace std;
using Poco::LogStream;
using Poco::Message;

static const char* modelDataFieldsNames[] =  {
    "Size",                                     // 0
    "Flags",                                    // 1
    "Time",                                     // 2
    "NumIndependentSpecies",                    // 3
    "NumDependentSpecies",                      // 4
    "DependentSpeciesConservedSums",            // 5
    "NumGlobalParameters",                      // 6
    "GlobalParameters",                         // 7
    "NumReactions",                             // 8
    "ReactionRates",                            // 9
    "NumRateRules",                             // 10
    "RateRules",                                // 11
    "LocalParametersOffsets",                   // 12
    "LocalParametersNum",                       // 13
    "LocalParameters",                          // 14
    "NumFloatingSpecies",                       // 15
    "FloatingSpeciesConcentrations",            // 16
    "FloatingSpeciesInitConcentrations",        // 17
    "FloatingSpeciesAmountRates",               // 18
    "FloatingSpeciesAmounts",                   // 19
    "FloatingSpeciesCompartments",              // 20
    "NumBoundarySpecies",                       // 21
    "BoundarySpeciesConcentrations",            // 22
    "BoundarySpeciesAmounts",                   // 23
    "BoundarySpeciesCompartments",              // 24
    "NumCompartments",                          // 25
    "CompartmentVolumes",                       // 26
    "Stoichiometry",                            // 27
    "NumEvents",                                // 28
    "EventTypeSize",                            // 29
    "EventType",                                // 20
    "EventPersistentTypeSize",                  // 31
    "EventPersistentType",                      // 32
    "EventTestsSize",                           // 33
    "EventTests",                               // 34
    "EventPrioritiesSize",                      // 35
    "EventPriorities",                          // 36
    "EventStatusArraySize",                     // 37
    "EventStatusArray",                         // 38
    "PreviousEventStatusArraySize",             // 39
    "PreviousEventStatusArray",                 // 40
    "StateVectorSize",                          // 41
    "StateVector",                              // 42
    "StateVectorRate",                          // 43
    "WorkSize",                                 // 44
    "Work",                                     // 45
    "EventDelays",                              // 46
    "EventAssignments",                         // 47
    "ComputeEventAssignments",                  // 48
    "PerformEventAssignments",                  // 49
    "ModelName",                                // 50
    "SrSize",                                   // 51
    "Sr"                                        // 52
};

static void test() {
    cout << "test\n";
}

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

    // get the reactions
    const ListOfReactions *reactions = model->getListOfReactions();
    for (int i = 0; i < reactions->size(); i++)
    {
        const Reaction *r = reactions->get(i);
        reactionsMap.insert(StringIntPair(r->getId(), i));

        // go through the reaction reactants and products to know how much to
        // allocate space for the stochiometry matrix.
        // all species that participate in reactions must be floating.
        const ListOfSpeciesReferences *reactants = r->getListOfReactants();
        for (int j = 0; j < reactants->size(); j++)
        {
            const SimpleSpeciesReference *r = reactants->get(j);
            const string& speciesId = r->getSpecies();

            // its OK if we do not find reactants as floating species, they
            // might be boundary species, so they do not change.
            try
            {
                int speciesIdx = getFloatingSpeciesIndex(r->getSpecies());
                stoichColIndx.push_back(i);
                stoichRowIndx.push_back(speciesIdx);
            }
            catch (exception&)
            {
                string err = "could not find reactant ";
                err += r->getSpecies();
                err += " in the list of floating species for reaction ";
                err += r->getId();
                err += ", this species will be ignored in this reaction.";

                LogStream ls(getLogger(), Message::PRIO_WARNING);
                ls << err << endl;
            }
        }

        const ListOfSpeciesReferences *products = r->getListOfProducts();
        for (int j = 0; j < products->size(); j++)
        {
            const SimpleSpeciesReference *p = products->get(j);
            // products had better be in the stoich matrix.

            try
            {
                int speciesIdx = getFloatingSpeciesIndex(p->getSpecies());
                stoichColIndx.push_back(i);
                stoichRowIndx.push_back(speciesIdx);
                LogStream ls(getLogger(), Message::PRIO_TRACE);
                ls << "product, row: " << i << ", col: " << speciesIdx << endl;
            }
            catch (exception&)
            {
                string err = "could not find product ";
                err += p->getSpecies();
                err += " in the list of floating species for reaction ";
                err += r->getId();
                err += ", this species will be ignored in this reaction.";

                LogStream ls(getLogger(), Message::PRIO_WARNING);
                ls << err << endl;
            }
        }
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
    m.numReactions                  = reactionsMap.size();
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

    // allocate the stoichiometry matrix
    m.stoichiometry = csr_matrix_new(getFloatingSpeciesSize(), getReactionSize(),
            stoichRowIndx, stoichColIndx, vector<double>(stoichRowIndx.size(), 0));
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

int LLVMModelDataSymbols::getReactionIndex(const std::string& id) const
{
    StringIntMap::const_iterator i = reactionsMap.find(id);
    if (i != reactionsMap.end())
    {
        return i->second;
    }
    else
    {
        throw LLVMException("could not find reactions with id " + id, __FUNC__);
    }
}

std::vector<std::string> LLVMModelDataSymbols::getReactionIds() const
{
    return getIds(reactionsMap);
}

int LLVMModelDataSymbols::getReactionSize() const
{
    return reactionsMap.size();
}

int LLVMModelDataSymbols::getFloatingSpeciesSize() const
{
    return floatingSpeciesMap.size();
}

std::list<std::pair<int, int> > LLVMModelDataSymbols::getStoichiometryIndx() const
{
    std::list<std::pair<int, int> > result;

    for (int i = 0; i < stoichRowIndx.size(); i++)
    {
        pair<int,int> entry(stoichRowIndx[i], stoichColIndx[i]);
        result.push_back(entry);
    }

    return result;
}

void LLVMModelDataSymbols::print() const
{
    int i = 0;


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

    for (StringIntMap::const_iterator i = reactionsMap.begin();
            i != reactionsMap.end(); i++)
    {
        cout << "reaction id: " << i->first << ", index: " << i->second
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

const char* LLVMModelDataSymbols::getFieldName(ModelDataFields field)
{
    if (field >= Size && field <= Sr)
    {
        return modelDataFieldsNames[field];
    }
    else
    {
        return "Error, field is out of range";
    }
}

} /* namespace rr */
