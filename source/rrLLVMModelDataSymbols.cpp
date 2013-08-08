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
    "RateRuleValues",                           // 11
    "RateRuleRates",                            // 11
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

/*
static void test() {
    cout << "test\n";
}
*/

static std::vector<std::string> getIds(const rr::LLVMModelDataSymbols::StringUIntMap & m)
{
    vector<string> result;
    result.resize(m.size());
    for(rr::LLVMModelDataSymbols::StringUIntMap::const_iterator i = m.begin();
            i != m.end(); i++)
    {
        result[i->second] = i->first;
    }
    return result;
}

namespace rr
{

LLVMModelDataSymbols::LLVMModelDataSymbols() :
        linearlyIndependentFloatingSpeciesSize(0),
        independentFloatingSpeciesSize(0),
        independentBoundarySpeciesSize(0),
        independentGlobalParameterSize(0),
        independentCompartmentSize(0)
{
}

LLVMModelDataSymbols::LLVMModelDataSymbols(const libsbml::Model *model,
        bool computeAndAssignConsevationLaws) :
        linearlyIndependentFloatingSpeciesSize(0),
        independentFloatingSpeciesSize(0),
        independentBoundarySpeciesSize(0),
        independentGlobalParameterSize(0),
        independentCompartmentSize(0)
{
    modelName = model->getName();

    // first go through the rules, see if they determine other stuff
    {
        const ListOfRules * rules = model->getListOfRules();
        for (unsigned i = 0; i < rules->size(); ++i)
        {
            const Rule *rule = rules->get(i);

            if (dynamic_cast<const AssignmentRule*>(rule))
            {
                assigmentRules.insert(rule->getVariable());
            }
            else if (dynamic_cast<const RateRule*>(rule))
            {
                uint rri = rateRules.size();
                rateRules[rule->getId()] = rri;
            }
            else if (dynamic_cast<const AlgebraicRule*>(rule))
            {
                poco_warning(getLogger(), string("encountered algegraic rule: ")
                        + rule->getId() + string(", currently not handled"));
            }
        }
    }

    // get the compartments, need to reorder them to set the independent ones
    // first
    {
        list<string> indCompartments;
        list<string> depCompartments;
        const ListOfCompartments *compartments = model->getListOfCompartments();
        for (uint i = 0; i < compartments->size(); i++)
        {
            const Compartment *c = compartments->get(i);
            const string& id = c->getId();
            if (isIndependentElement(id))
            {
                indCompartments.push_back(id);
            }
            else
            {
                depCompartments.push_back(id);
            }
        }
        for (list<string>::const_iterator i = indCompartments.begin();
                i != indCompartments.end(); ++i)
        {
            uint ci = compartmentsMap.size();
            compartmentsMap[*i] = ci;
        }

        for (list<string>::const_iterator i = depCompartments.begin();
                i != depCompartments.end(); ++i)
        {
            uint ci = compartmentsMap.size();
            compartmentsMap[*i] = ci;
        }

        // finally set how many ind compartments we have
        independentCompartmentSize = indCompartments.size();
    }

    // process the floating species
    {
        const ListOfSpecies *species = model->getListOfSpecies();
        list<string> indFltSpecies;
        list<string> depFltSpecies;

        // get the floating species and set thier compartments
        ls::LibStructural structural(model);

        poco_information(getLogger(),
                "performed structural analysis on model: " +
                structural.getAnalysisMsg());

        // reorder by linearly independent first, then linearly dependent
        vector<string> reorderedList = computeAndAssignConsevationLaws ?
                structural.getReorderedSpecies() :
                structural.getSpecies();

        linearlyIndependentFloatingSpeciesSize = structural.getNumIndSpecies();

        // figure out 'fully' indendent flt species -- those without rules.
        for (uint i = 0; i < reorderedList.size(); ++i)
        {
            // just make sure its a valid species
            const string& sid = reorderedList[i];
            const Species *s = 0;
            assert((s = species->get(sid)) && !s->getBoundaryCondition());

            if (computeAndAssignConsevationLaws &&
                    i <= linearlyIndependentFloatingSpeciesSize &&
                    !isIndependentElement(sid))
            {
                string msg = "structural analysis determined that " + sid +
                        " is linearly independent, but it has has rules "
                        "(assignment or rate) determining its dynamics.";
                throw_llvm_exception(msg);
            }

            if (isIndependentElement(sid))
            {
                indFltSpecies.push_back(sid);
            }
            else
            {
                depFltSpecies.push_back(sid);
            }
        }

        // stuff the species in the map
        for (list<string>::const_iterator i = indFltSpecies.begin();
                i != indFltSpecies.end(); ++i)
        {
            uint si = floatingSpeciesMap.size();
            floatingSpeciesMap[*i] = si;
        }

        for (list<string>::const_iterator i = depFltSpecies.begin();
                i != depFltSpecies.end(); ++i)
        {
            uint si = floatingSpeciesMap.size();
            floatingSpeciesMap[*i] = si;
        }

        // figure out what compartments they belong to
        floatingSpeciesCompartments.resize(floatingSpeciesMap.size());
        for (StringUIntMap::const_iterator i = floatingSpeciesMap.begin();
                i != floatingSpeciesMap.end(); ++i)
        {
            const Species *s = species->get(i->first);
            uint compId = compartmentsMap.find(s->getCompartment())->second;
            floatingSpeciesCompartments[i->second] = compId;
        }

        // finally set how many ind species we've found
        independentFloatingSpeciesSize = indFltSpecies.size();

        if (Logger::PRIO_INFORMATION <= getLogger().getLevel())
        {
            LoggingBuffer log(Logger::PRIO_INFORMATION, __FILE__, __LINE__);

            log.stream() << "found " << indFltSpecies.size()
                    << " independent and " << depFltSpecies.size()
                    << " dependent floating species." << endl;

            log.stream() << "linearly independent species: " <<
                    linearlyIndependentFloatingSpeciesSize << endl;

            vector<string> ids = getFloatingSpeciesIds();
            for (uint i = 0; i < ids.size(); ++i)
            {
                log.stream() << "floating species [" << i << "] = \'" << ids[i]
                        << "\'" << endl;
            }
        }
    }

    // display compartment info. We need to get the compartments before the
    // so we can get the species compartments. But the struct anal dumps
    // a bunch of stuff, so to keep things looking nice in the log, we
    // display the compartment info here.
    if (Logger::PRIO_INFORMATION <= getLogger().getLevel())
    {
        LoggingBuffer log(Logger::PRIO_INFORMATION, __FILE__, __LINE__);

        log.stream() << "found " << independentCompartmentSize
                << " independent and " << (compartmentsMap.size() -
                        independentCompartmentSize)
                << " dependent compartments." << endl;

        vector<string> ids = getCompartmentIds();
        for (uint i = 0; i < ids.size(); ++i)
        {
            log.stream() << "compartment [" << i << "] = \'" << ids[i]
                    << "\'" << endl;
        }
    }

    // process the boundary species
    {
        const ListOfSpecies *species = model->getListOfSpecies();
        list<string> indBndSpecies;
        list<string> depBndSpecies;

        // get the boundary species

        for (uint i = 0; i < species->size(); ++i)
        {
            const Species *s = species->get(i);
            if (s->getBoundaryCondition())
            {
                if (isIndependentElement(s->getId()))
                {
                    indBndSpecies.push_back(s->getId());
                }
                else
                {
                    depBndSpecies.push_back(s->getId());
                }
            }
        }

        // stuff the species in the map
        for (list<string>::const_iterator i = indBndSpecies.begin();
                i != indBndSpecies.end(); ++i)
        {
            uint bi = boundarySpeciesMap.size();
            boundarySpeciesMap[*i] = bi;
        }

        for (list<string>::const_iterator i = depBndSpecies.begin();
                i != depBndSpecies.end(); ++i)
        {
            uint bi = boundarySpeciesMap.size();
            boundarySpeciesMap[*i] = bi;
        }

        // figure out what compartments they belong to
        boundarySpeciesCompartments.resize(boundarySpeciesMap.size());
        for (StringUIntMap::const_iterator i = boundarySpeciesMap.begin();
                i != boundarySpeciesMap.end(); ++i)
        {
            const Species *s = species->get(i->first);
            uint compId = compartmentsMap.find(s->getCompartment())->second;
            boundarySpeciesCompartments[i->second] = compId;
        }

        // finally set how many we have
        independentBoundarySpeciesSize = indBndSpecies.size();

        if (Logger::PRIO_INFORMATION <= getLogger().getLevel())
        {
            LoggingBuffer log(Logger::PRIO_INFORMATION, __FILE__, __LINE__);

            log.stream() << "found "
                    << indBndSpecies.size() << " independent and "
                    << depBndSpecies.size() << " dependent boundary species."
                    << endl;

            vector<string> ids = getBoundarySpeciesIds();
            for (uint i = 0; i < ids.size(); ++i)
            {
                log.stream() << "boundary species [" << i << "] = \'" << ids[i] << "\'" << endl;
            }
        }
    }


    // get the global parameters, need to reorder them to set the independent
    // ones first
    {
        list<string> indParam;
        list<string> depParam;
        const ListOfParameters *parameters = model->getListOfParameters();
        for (uint i = 0; i < parameters->size(); i++)
        {
            const Parameter *p = parameters->get(i);
            const string& id = p->getId();
            if (isIndependentElement(id))
            {
                indParam.push_back(id);
            }
            else
            {
                depParam.push_back(id);
            }
        }
        for (list<string>::const_iterator i = indParam.begin();
                i != indParam.end(); ++i)
        {
            uint pi = globalParametersMap.size();
            globalParametersMap[*i] = pi;
        }

        for (list<string>::const_iterator i = depParam.begin();
                i != depParam.end(); ++i)
        {
            uint pi = globalParametersMap.size();
            globalParametersMap[*i] = pi;
        }

        // finally set how many ind compartments we have
        independentGlobalParameterSize = indParam.size();

        if (Logger::PRIO_INFORMATION <= getLogger().getLevel())
        {
            LoggingBuffer log(Logger::PRIO_INFORMATION, __FILE__, __LINE__);

            log.stream() << "found " << independentGlobalParameterSize
                    << " independent and " << depParam.size()
                    << " dependent global parameters." << endl;

            vector<string> ids = getGlobalParameterIds();
            for (uint i = 0; i < ids.size(); ++i)
            {
                log.stream() << "global parameter [" << i << "] = \'" << ids[i]
                        << "\'" << endl;
            }
        }
    }

    // get the reactions
    const ListOfReactions *reactions = model->getListOfReactions();
    for (uint i = 0; i < reactions->size(); i++)
    {
        const Reaction *r = reactions->get(i);
        reactionsMap.insert(StringUIntPair(r->getId(), i));

        // go through the reaction reactants and products to know how much to
        // allocate space for the stochiometry matrix.
        // all species that participate in reactions must be floating.
        const ListOfSpeciesReferences *reactants = r->getListOfReactants();
        for (uint j = 0; j < reactants->size(); j++)
        {
            const SimpleSpeciesReference *r = reactants->get(j);

            // its OK if we do not find reactants as floating species, they
            // might be boundary species, so they do not change.
            try
            {
                uint speciesIdx = getFloatingSpeciesIndex(r->getSpecies());
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
        for (uint j = 0; j < products->size(); j++)
        {
            const SimpleSpeciesReference *p = products->get(j);
            // products had better be in the stoich matrix.

            try
            {
                uint speciesIdx = getFloatingSpeciesIndex(p->getSpecies());
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
}


uint LLVMModelDataSymbols::getCompartmentIndex(
        const std::string& id) const
{
    StringUIntMap::const_iterator i = compartmentsMap.find(id);
    if (i != compartmentsMap.end())
    {
        return i->second;
    }
    else
    {
        throw LLVMException("could not find compartment with id " + id, __FUNC__);
    }
}

uint LLVMModelDataSymbols::getFloatingSpeciesIndex(
        const std::string& id) const
{
    StringUIntMap::const_iterator i = floatingSpeciesMap.find(id);
    if(i != floatingSpeciesMap.end())
    {
        return i->second;
    }
    else
    {
        throw LLVMException("could not find floating species with id " + id, __FUNC__);
    }
}

uint LLVMModelDataSymbols::getBoundarySpeciesIndex(
        const std::string& id) const
{
    StringUIntMap::const_iterator i = boundarySpeciesMap.find(id);
    if(i != boundarySpeciesMap.end())
    {
        return i->second;
    }
    else
    {
        throw LLVMException("could not find boundary species with id " + id, __FUNC__);
    }
}

uint LLVMModelDataSymbols::getBoundarySpeciesSize() const
{
    return boundarySpeciesMap.size();
}


uint LLVMModelDataSymbols::getGlobalParameterIndex(
        const std::string& id) const
{
    StringUIntMap::const_iterator i = globalParametersMap.find(id);
    if(i != globalParametersMap.end())
    {
        return i->second;
    }
    else
    {
        throw LLVMException("could not find global parameter species with id " + id, __FUNC__);
    }
}

void LLVMModelDataSymbols::initAllocModelDataBuffers(LLVMModelData& m) const
{
    // zero out the structure
    LLVMModelData::init(m);

    // set the buffer sizes


    //
    m.numIndependentSpecies         = independentFloatingSpeciesSize;




    //mData.numDependentSpecies           = ms.mNumDependentSpecies;
    m.numGlobalParameters           = this->independentGlobalParameterSize;
    m.numReactions                  = reactionsMap.size();
    //mData.numEvents                     = ms.mNumEvents;
    m.numFloatingSpecies            = this->independentFloatingSpeciesSize;
    m.numRateRules                  = rateRules.size();
    m.numCompartments               = this->independentCompartmentSize;
    m.numBoundarySpecies            = this->independentBoundarySpeciesSize;
    //mData.srSize                        = ms.mNumModifiableSpeciesReferences;
    //mData.eventPrioritiesSize           = ms.mNumEvents;
    //mData.eventStatusArraySize          = ms.mNumEvents;
    //mData.previousEventStatusArraySize  = ms.mNumEvents;
    //mData.eventPersistentTypeSize       = ms.mNumEvents;
    //mm.eventTestsSize                = ms.mNumEvents;
    //mm.eventTypeSize                 = ms.mNumEvents;

    m.modelName = strdup(modelName.c_str());

    // in certain cases, the data returned by c++ new may be alligned differently than
    // malloc, so just use rrCalloc here just to be safe, plus rrCalloc returns zero
    // initialized memory.


    m.floatingSpeciesAmounts = (double*)calloc(m.numIndependentSpecies, sizeof(double));
    m.floatingSpeciesAmountRates = (double*)calloc(m.numIndependentSpecies, sizeof(double));
    m.rateRuleValues = (double*)calloc(m.numRateRules, sizeof(double));
    m.rateRuleRates = (double*)calloc(m.numRateRules, sizeof(double));
    //m.floatingSpeciesConcentrations = (double*)calloc(m.numFloatingSpecies, sizeof(double));
    m.reactionRates = (double*)calloc(m.numReactions, sizeof(double));
    //m.dependentSpeciesConservedSums = (double*)rrCalloc(m.numDependentSpecies, sizeof(double));
    //m.floatingSpeciesInitConcentrations = (double*)rrCalloc(m.numFloatingSpecies, sizeof(double));
    m.globalParameters = (double*)calloc(m.numGlobalParameters, sizeof(double));
    m.compartmentVolumes = (double*)calloc(m.numCompartments, sizeof(double));
    //m.boundarySpeciesConcentrations = (double*)rrCalloc(m.numBoundarySpecies, sizeof(double));
    m.boundarySpeciesAmounts = (double*)calloc(m.numBoundarySpecies, sizeof(double));
    //m.sr = (double*)rrCalloc(m.srSize, sizeof(double));
    //m.eventPriorities = (double*)rrCalloc(m.eventPrioritiesSize, sizeof(double));
    //m.eventStatusArray = (bool*)rrCalloc(m.eventStatusArraySize, sizeof(bool));
    //m.previousEventStatusArray = (bool*)rrCalloc(m.previousEventStatusArraySize, sizeof(bool));
    //m.eventPersistentType = (bool*)rrCalloc(m.eventPersistentTypeSize, sizeof(bool));
    //m.eventTests = (double*)rrCalloc(m.eventTestsSize, sizeof(double));
    //m.eventType = (bool*)rrCalloc(m.eventTypeSize, sizeof(bool));
    m.floatingSpeciesCompartments = (unsigned*)calloc(m.numFloatingSpecies, sizeof(unsigned));
    m.boundarySpeciesCompartments = (unsigned*)calloc(m.numBoundarySpecies, sizeof(unsigned));
    //m.work = (double*)rrCalloc(m.workSize, sizeof(double));


    // allocate the stoichiometry matrix
    m.stoichiometry = csr_matrix_new(getFloatingSpeciesSize(), getReactionSize(),
            stoichRowIndx, stoichColIndx, vector<double>(stoichRowIndx.size(), 0));

    // fill out the species / compartment mapping arrays
    for (StringUIntMap::const_iterator i = floatingSpeciesMap.begin();
            i != floatingSpeciesMap.end(); ++i)
    {
        uint compIndex = getFloatingSpeciesCompartmentIndex(i->first);
        m.floatingSpeciesCompartments[i->second] = compIndex;
    }
}

std::vector<std::string> LLVMModelDataSymbols::getCompartmentIds() const
{
    return getIds(compartmentsMap);
}

std::vector<std::string> LLVMModelDataSymbols::getBoundarySpeciesIds() const
{
    return getIds(boundarySpeciesMap);
}

uint LLVMModelDataSymbols::getFloatingSpeciesCompartmentIndex(
        const std::string& id) const
{
    uint speciesIndex = getFloatingSpeciesIndex(id);
    return floatingSpeciesCompartments[speciesIndex];
}

uint LLVMModelDataSymbols::getBoundarySpeciesCompartmentIndex(
        const std::string& id) const
{
    uint speciesIndex = getBoundarySpeciesIndex(id);
    return boundarySpeciesCompartments[speciesIndex];
}

uint LLVMModelDataSymbols::getReactionIndex(const std::string& id) const
{
    StringUIntMap::const_iterator i = reactionsMap.find(id);
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

uint LLVMModelDataSymbols::getReactionSize() const
{
    return reactionsMap.size();
}

uint LLVMModelDataSymbols::getFloatingSpeciesSize() const
{
    return floatingSpeciesMap.size();
}

std::list<std::pair<uint, uint> > LLVMModelDataSymbols::getStoichiometryIndx() const
{
    std::list<std::pair<uint, uint> > result;

    for (uint i = 0; i < stoichRowIndx.size(); i++)
    {
        pair<uint,uint> entry(stoichRowIndx[i], stoichColIndx[i]);
        result.push_back(entry);
    }

    return result;
}

void LLVMModelDataSymbols::print() const
{
    uint i = 0;


    for (StringUIntMap::const_iterator i = floatingSpeciesMap.begin();
            i != floatingSpeciesMap.end(); i++)
    {
        cout << "float species id: " << i->first << ", index: " << i->second
                << "\n";
    }

    for (StringUIntMap::const_iterator i = boundarySpeciesMap.begin();
            i != boundarySpeciesMap.end(); i++)
    {
        cout << "boundary species id: " << i->first << ", index: " << i->second
                << "\n";
    }

    for (StringUIntMap::const_iterator i = compartmentsMap.begin();
            i != compartmentsMap.end(); i++)
    {
        cout << "compartment id: " << i->first << ", index: " << i->second
                << "\n";
    }

    for (StringUIntMap::const_iterator i = globalParametersMap.begin();
            i != globalParametersMap.end(); i++)
    {
        cout << "global parameter id: " << i->first << ", index: " << i->second
                << "\n";
    }

    for (StringUIntMap::const_iterator i = reactionsMap.begin();
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

uint LLVMModelDataSymbols::getLinearlyIndependentFloatingSpeciesSize() const
{
    return linearlyIndependentFloatingSpeciesSize;
}

uint LLVMModelDataSymbols::getIndependentFloatingSpeciesSize() const
{
    return independentFloatingSpeciesSize;
}

bool LLVMModelDataSymbols::isIndependentFloatingSpecies(
        const std::string& id) const
{
    StringUIntMap::const_iterator i = floatingSpeciesMap.find(id);
    return i != floatingSpeciesMap.end() &&
            i->second < independentFloatingSpeciesSize;
}

bool LLVMModelDataSymbols::isIndependentBoundarySpecies(
        const std::string& id) const
{
    StringUIntMap::const_iterator i = boundarySpeciesMap.find(id);
    return i != boundarySpeciesMap.end() &&
            i->second < independentBoundarySpeciesSize;
}

bool LLVMModelDataSymbols::isIndependentGlobalParameter(
        const std::string& id) const
{
    StringUIntMap::const_iterator i = globalParametersMap.find(id);
    return i != globalParametersMap.end() &&
            i->second < independentGlobalParameterSize;
}

bool LLVMModelDataSymbols::isIndependentCompartment(const std::string& id) const
{
    StringUIntMap::const_iterator i = compartmentsMap.find(id);
    return i != compartmentsMap.end() &&
            i->second < independentCompartmentSize;
}

const char* LLVMModelDataSymbols::getFieldName(ModelDataFields field)
{
    if (field >= Size && field <= ModelName)
    {
        return modelDataFieldsNames[field];
    }
    else
    {
        return "Error, field is out of range";
    }
}

uint LLVMModelDataSymbols::getIndependentGlobalParameterSize() const
{
    return independentGlobalParameterSize;
}

uint LLVMModelDataSymbols::getIndependentCompartmentSize() const
{
    return independentCompartmentSize;
}

uint LLVMModelDataSymbols::getIndependentBoundarySpeciesSize() const
{
    return independentBoundarySpeciesSize;
}

uint LLVMModelDataSymbols::getRateRuleIndex(std::string const& id) const
{
    StringUIntMap::const_iterator i = rateRules.find(id);
    if (i != rateRules.end())
    {
        return i->second;
    }
    else
    {
        throw LLVMException("could not find rate rule with id " + id, __FUNC__);
    }
}

uint LLVMModelDataSymbols::getRateRuleSize() const
{
    return rateRules.size();
}

bool LLVMModelDataSymbols::isIndependentElement(const std::string& id) const
{
    return rateRules.find(id) == rateRules.end() &&
            assigmentRules.find(id) == assigmentRules.end();
}

bool LLVMModelDataSymbols::hasAssignmentRule(const std::string& id) const
{
    return assigmentRules.find(id) != assigmentRules.end();
}

bool LLVMModelDataSymbols::hasRateRule(const std::string& id) const
{
    return rateRules.find(id) != rateRules.end();
}

uint LLVMModelDataSymbols::getCompartmentsSize() const
{
    return compartmentsMap.size();
}

uint LLVMModelDataSymbols::getGlobalParametersSize() const
{
    return globalParametersMap.size();
}


} /* namespace rr */


