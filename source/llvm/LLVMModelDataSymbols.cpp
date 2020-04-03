/*
 * LLVMModelDataSymbols.cpp
 *
 *  Created on: Jun 28, 2013
 *      Author: andy
 */
#pragma hdrstop
#include "LLVMModelDataSymbols.h"
#include "lsLibStructural.h"
#include "LLVMException.h"
#include "rrLogger.h"
#include "rrSparse.h"
#include "rrStringUtils.h"
#include "conservation/ConservationExtension.h"
#include "rrRoadRunnerOptions.h"

#include <Poco/LogStream.h>
#include <sbml/Model.h>
#include <sbml/SBMLDocument.h>

#include <string>
#include <vector>
#include <sstream>


#include "tr1proxy/rr_memory.h"
#include "tr1proxy/rr_unordered_map.h"


using namespace libsbml;
using namespace std;

using rr::Logger;
using rr::getLogger;
using rr::LoggingBuffer;

using rr::conservation::ConservationExtension;

using Poco::LogStream;
using Poco::Message;

static const char* modelDataFieldsNames[] =  {
        "Size",                                 // 0
        "Flags",                                // 1
        "Time",                                 // 2
        "NumIndCompartments",                   // 3
        "NumIndFloatingSpecies",                // 4
        "NumIndBoundarySpecies",                // 5
        "NumIndGlobalParameters",               // 6
        "NumRateRules",                         // 7
        "NumReactions",                         // 8

        "NumInitCompartments",                  // 9
        "NumInitFloatingSpecies",               // 10
        "NumInitBoundarySpecies",               // 11
        "NumInitGlobalParameters",              // 12

        "Stoichiometry",                        // 13
        "RandomPtr",                            // 14
        "NumEvents",                            // 15
        "StateVectorSize",                      // 16
        "StateVector",                          // 17
        "StateVectorRate",                      // 18
        "RateRuleRates",                        // 19
        "FloatingSpeciesAmountRates",           // 20

        "CompartmentVolumesAlias",              // 21
        "InitCompartmentVolumesAlias",          // 22
        "InitFloatingSpeciesAmountsAlias",      // 23
        "BoundarySpeciesAmountsAlias",          // 24
        "InitBoundarySpeciesAmountsAlias",      // 25
        "GlobalParametersAlias",                // 26
        "InitGlobalParametersAlias",            // 27
        "ReactionRatesAlias",                   // 28

        "RateRuleValuesAlias",                  // 29
        "FloatingSpeciesAmountsAlias",          // 30

        "CompartmentVolumes",                   // 31
        "InitCompartmentVolumes",               // 32
        "InitFloatingSpeciesAmounts",           // 33
        "BoundarySpeciesAmounts",               // 34
        "InitBoundarySpeciesAmounts",           // 35
        "GlobalParameters",                     // 36
        "InitGlobalParameters",                 // 37
        "ReactionRates",                        // 38
        "NotSafe_RateRuleValues",               // 39
        "NotSafe_FloatingSpeciesAmounts"        // 40
};


static std::vector<std::string> getIds(const rrllvm::LLVMModelDataSymbols::StringUIntMap & m)
{
    vector<string> result(m.size());
    for(rrllvm::LLVMModelDataSymbols::StringUIntMap::const_iterator i = m.begin();
            i != m.end(); i++)
    {
        assert(i->second < result.size() && "symbol map index out of bounds, possibly duplicate sbml ids");
        result[i->second] = i->first;
    }
    return result;
}

namespace rrllvm
{

LLVMModelDataSymbols::LLVMModelDataSymbols() :
    independentFloatingSpeciesSize(0),
    independentBoundarySpeciesSize(0),
    independentGlobalParameterSize(0),
    independentCompartmentSize(0),
    independentInitFloatingSpeciesSize(0),
    independentInitBoundarySpeciesSize(0),
    independentInitGlobalParameterSize(0),
    independentInitCompartmentSize(0)
{
    assert(sizeof(modelDataFieldsNames) / sizeof(const char*)
            == NotSafe_FloatingSpeciesAmounts + 1
            && "wrong number of items in modelDataFieldsNames");
}

LLVMModelDataSymbols::LLVMModelDataSymbols(const libsbml::Model *model,
        unsigned options) :
    independentFloatingSpeciesSize(0),
    independentBoundarySpeciesSize(0),
    independentGlobalParameterSize(0),
    independentCompartmentSize(0),
    independentInitFloatingSpeciesSize(0),
    independentInitBoundarySpeciesSize(0),
    independentInitGlobalParameterSize(0),
    independentInitCompartmentSize(0)
{
    assert(sizeof(modelDataFieldsNames) / sizeof(const char*)
            == NotSafe_FloatingSpeciesAmounts + 1
            && "wrong number of items in modelDataFieldsNames");

    modelName = model->getName();

    // first go through the rules, see if they determine other stuff
    {
        const ListOfRules * rules = model->getListOfRules();
        for (unsigned i = 0; i < rules->size(); ++i)
        {
            const Rule *rule = rules->get(i);

            if (dynamic_cast<const AssignmentRule*>(rule))
            {
                assignmentRules.insert(rule->getVariable());
            }
            else if (dynamic_cast<const RateRule*>(rule))
            {
                uint rri = rateRules.size();
                rateRules[rule->getId()] = rri;
            }
            else if (dynamic_cast<const AlgebraicRule*>(rule))
            {
                char* formula = SBML_formulaToString(rule->getMath());
                Log(Logger::LOG_WARNING)
                    << "Unable to handle algebraic rules. Formula '0 = "
                    << formula << "' ignored.";
                free(formula);
            }
        }
    }

    {
        const ListOfInitialAssignments *initAssignmentList =
                model->getListOfInitialAssignments();

        for (unsigned i = 0; i < initAssignmentList->size(); ++i)
        {
            const libsbml::InitialAssignment *ia = initAssignmentList->get(i);

            initAssignmentRules.insert(ia->getSymbol());
        }
    }

    // get the compartments, need to reorder them to set the independent ones
    // first. Make sure compartments is called *before* float species.
    initCompartments(model);


    // process the floating species
    initFloatingSpecies(model, options & rr::LoadSBMLOptions::CONSERVED_MOIETIES);

    // display compartment info. We need to get the compartments before the
    // so we can get the species compartments. But the struct anal dumps
    // a bunch of stuff, so to keep things looking nice in the log, we
    // display the compartment info here.
    displayCompartmentInfo();

    initBoundarySpecies(model);

    initGlobalParameters(model, options & rr::LoadSBMLOptions::CONSERVED_MOIETIES);

    initReactions(model);

    initEvents(model);
}

LLVMModelDataSymbols::LLVMModelDataSymbols(std::istream& in)
{
	this->loadState(in);
}



LLVMModelDataSymbols::~LLVMModelDataSymbols()
{
}

const std::string& LLVMModelDataSymbols::getModelName() const
{
    return modelName;
}

LLVMModelDataSymbols::SymbolIndexType LLVMModelDataSymbols::getSymbolIndex(
        const std::string& name, int& result) const
{
    StringUIntMap::const_iterator i;

    if((i = floatingSpeciesMap.find(name)) != floatingSpeciesMap.end())
    {
        result = i->second;
        return FLOATING_SPECIES;
    }
    else if((i = boundarySpeciesMap.find(name)) != boundarySpeciesMap.end())
    {
        result = i->second;
        return BOUNDARY_SPECIES;
    }
    else if((i = compartmentsMap.find(name)) != compartmentsMap.end())
    {
        result = i->second;
        return COMPARTMENT;
    }
    else if((i = globalParametersMap.find(name)) != globalParametersMap.end())
    {
        result = i->second;
        return GLOBAL_PARAMETER;
    }
    else if((i = reactionsMap.find(name)) != reactionsMap.end())
    {
        result = i->second;
        return REACTION;
    }
    else if ((i = eventIds.find(name)) != eventIds.end())
    {
        result = i->second;
        return EVENT;
    }

    result = -1;
    return INVALID_SYMBOL;
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
        const std::string& id, bool requireIndependent) const
{
    StringUIntMap::const_iterator i = floatingSpeciesMap.find(id);
    if(i != floatingSpeciesMap.end())
    {
        if (requireIndependent && i->second >= independentFloatingSpeciesSize)
        {
            string msg = "The species " + id + " is valid, however it is dependent on a rule";
            throw LLVMException(msg);
        }
        return i->second;
    }
    else
    {
        string msg = "could not find floating species with id "  + id;
        throw LLVMException(msg);
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

/*
void LLVMModelDataSymbols::initAllocModelDataBuffers(LLVMModelData& m) const
{
    // zero out the structure
    LLVMModelData::init(m);

    // set the buffer sizes
    m.numIndFloatingSpecies         = independentFloatingSpeciesSize;

    //mData.numDependentSpecies           = ms.mNumDependentSpecies;
    m.numIndGlobalParameters        = independentGlobalParameterSize;
    m.numReactions                  = reactionsMap.size();
    m.numEvents                     = eventAttributes.size();
    m.numRateRules                  = rateRules.size();
    m.numIndCompartments               = independentCompartmentSize;
    m.numIndBoundarySpecies            = independentBoundarySpeciesSize;

    // in certain cases, the data returned by c++ new may be alligned differently than
    // malloc, so just use calloc here just to be safe, plus calloc returns zero
    // initialized memory.

    m.floatingSpeciesAmountsAlias = (double*)calloc(m.numIndFloatingSpecies, sizeof(double));
    m.floatingSpeciesAmountRates = 0;
    m.rateRuleValuesAlias = (double*)calloc(m.numRateRules, sizeof(double));
    m.rateRuleRates = 0;

    m.reactionRatesAlias = (double*)calloc(m.numReactions, sizeof(double));

    m.globalParametersAlias = (double*)calloc(m.numIndGlobalParameters, sizeof(double));
    m.compartmentVolumesAlias = (double*)calloc(m.numIndCompartments, sizeof(double));

    m.boundarySpeciesAmountsAlias = (double*)calloc(m.numIndBoundarySpecies, sizeof(double));


    // allocate the stoichiometry matrix
    m.stoichiometry = rr::csr_matrix_new(m.numIndFloatingSpecies, getReactionSize(),
            stoichRowIndx, stoichColIndx, vector<double>(stoichRowIndx.size(), 0));
}
*/


const std::vector<uint>& LLVMModelDataSymbols::getStoichRowIndx() const
{
    return stoichRowIndx;
}

const std::vector<uint>& LLVMModelDataSymbols::getStoichColIndx() const
{
    return stoichColIndx;
}

std::vector<std::string> LLVMModelDataSymbols::getCompartmentIds() const
{
    return getIds(compartmentsMap);
}

std::vector<std::string> LLVMModelDataSymbols::getBoundarySpeciesIds() const
{
    return getIds(boundarySpeciesMap);
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

std::list<LLVMModelDataSymbols::SpeciesReferenceInfo>
    LLVMModelDataSymbols::getStoichiometryIndx() const
{
    std::list<SpeciesReferenceInfo> result;

    for (uint i = 0; i < stoichRowIndx.size(); i++)
    {
        SpeciesReferenceInfo entry =
            {stoichRowIndx[i], stoichColIndx[i], stoichTypes[i], stoichIds[i]};
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

std::string LLVMModelDataSymbols::getFloatingSpeciesId(uint indx) const
{
    for (StringUIntMap::const_iterator i = floatingSpeciesMap.begin();
            i != floatingSpeciesMap.end(); ++i)
    {
        if (i->second == indx)
        {
            return i->first;
        }
    }

    throw std::out_of_range("attempted to access floating species id at index " + rr::toString(indx));
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

bool LLVMModelDataSymbols::isBoundarySpecies(
        const std::string& id) const
{
    StringUIntMap::const_iterator i = boundarySpeciesMap.find(id);
    return i != boundarySpeciesMap.end();
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
    if (field >= Size && field <= ReactionRates)
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

std::string rrllvm::LLVMModelDataSymbols::getRateRuleId(uint indx) const
{
    for(StringUIntMap::const_iterator i = rateRules.begin();
            i != rateRules.end(); ++i)
    {
        if (i->second == indx)
        {
            return i->first;
        }
    }

    throw std::out_of_range("attempted to access rate rule id at index " +
            rr::toString(indx));
}

bool LLVMModelDataSymbols::isIndependentElement(const std::string& id) const
{
    return rateRules.find(id) == rateRules.end() &&
            assignmentRules.find(id) == assignmentRules.end();
}

bool LLVMModelDataSymbols::hasAssignmentRule(const std::string& id) const
{
    return assignmentRules.find(id) != assignmentRules.end();
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

void LLVMModelDataSymbols::initGlobalParameters(const libsbml::Model* model,
        bool conservedMoieties)
{
    list<string> indParam;
    list<string> depParam;
    list<string> indInitParam;
    list<string> depInitParam;

    const ListOfParameters *parameters = model->getListOfParameters();

    globalParameterRateRules.resize(parameters->size(), false);

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

        if (isIndependentInitElement(id))
        {
            indInitParam.push_back(id);
        }
        else
        {
            depInitParam.push_back(id);
        }
    }

    // when this is used, we check the size, so works even
    // when consv moieity is not enabled.
    if (conservedMoieties)
    {
        conservedMoietyGlobalParameter.resize(indParam.size(), false);

        // allocate space, could be up this many.
        conservedMoietyGlobalParameterIndex.reserve(indParam.size());
    }

    for (list<string>::const_iterator i = indParam.begin();
            i != indParam.end(); ++i)
    {
        uint pi = globalParametersMap.size();
        globalParametersMap[*i] = pi;

        // CM parameters can only be independent.
        if (conservedMoieties)
        {
            const Parameter* p = parameters->get(*i);
            bool isCons = ConservationExtension::getConservedMoiety(*p);
            conservedMoietyGlobalParameter[pi] = isCons;

            if (isCons)
            {
                conservedMoietyGlobalParameterIndex.push_back(pi);
            }
        }
    }

    for (list<string>::const_iterator i = depParam.begin();
            i != depParam.end(); ++i)
    {
        uint pi = globalParametersMap.size();
        globalParametersMap[*i] = pi;

        // all the independent ones by def have no rate rules.
        globalParameterRateRules[pi] = hasRateRule(*i);
    }


    for (list<string>::const_iterator i = indInitParam.begin();
            i != indInitParam.end(); ++i)
    {
        uint ci = initGlobalParametersMap.size();
        initGlobalParametersMap[*i] = ci;
    }

    for (list<string>::const_iterator i = depInitParam.begin();
            i != depInitParam.end(); ++i)
    {
        uint ci = initGlobalParametersMap.size();
        initGlobalParametersMap[*i] = ci;
    }

    // finally set how many ind compartments we have
    independentGlobalParameterSize = indParam.size();
    independentInitGlobalParameterSize = indInitParam.size();

    if (Logger::LOG_INFORMATION <= getLogger().getLevel())
    {
        LoggingBuffer log(Logger::LOG_DEBUG, __FILE__, __LINE__);

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

void LLVMModelDataSymbols::initBoundarySpecies(const libsbml::Model* model)
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

    // finally set how many we have
    independentBoundarySpeciesSize = indBndSpecies.size();

    if (Logger::LOG_DEBUG <= getLogger().getLevel())
    {
        LoggingBuffer log(Logger::LOG_DEBUG, __FILE__, __LINE__);

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

void LLVMModelDataSymbols::initFloatingSpecies(const libsbml::Model* model,
        bool computeAndAssignConsevationLaws)
{
    const ListOfSpecies *species = model->getListOfSpecies();

    // independent at run time, no rules of any sort
    list<string> indFltSpecies;
    list<string> depFltSpecies;

    list<string> indInitFltSpecies;
    list<string> depInitFltSpecies;

    // figure out 'fully' indendent flt species -- those without rules.
    for (uint i = 0; i < species->size(); ++i)
    {
        const Species *s = species->get(i);
        std::vector<std::string> quantities = ConservationExtension::getConservedQuantities(*s);

        if (s->getBoundaryCondition())
        {
            continue;
        }

        const string& sid = s->getId();

        if (isIndependentElement(sid))
        {
            indFltSpecies.push_back(sid);
            if (quantities.size()) {
                for (uint j=0; j<quantities.size(); ++j) {
                    std::string quantity = quantities.at(j);
                    conservedMoietyIndSpecies[quantity].push_back(indFltSpecies.size()-1);
                }
            }
        }
        else
        {
            depFltSpecies.push_back(sid);
            if (quantities.size()) {
                for (uint j=0; j<quantities.size(); ++j) {
                    std::string quantity = quantities.at(j);
                    conservedMoietyDepSpecies[quantity] = depFltSpecies.size()-1;
                }
            }
        }

        bool conservedMoiety = ConservationExtension::getConservedMoiety(*s);

        bool indInit = (!hasInitialAssignmentRule(sid) &&
                (!hasAssignmentRule(sid) || conservedMoiety));

        // conserved moiety species assignment rules do not apply at
        // time t < 0
        if (indInit)
        {
            indInitFltSpecies.push_back(sid);
        }
        else
        {
            depInitFltSpecies.push_back(sid);
        }

        if (conservedMoiety) {
            conservedMoietySpeciesSet.insert(sid);
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

        // now that we know how many float species we have, we
        // can map these to the conserved moieties.
        // assume that the cm order (T vector) matches the order
        // that the CM species were added.
        if (computeAndAssignConsevationLaws)
        {
            // look in the set we just made
            if (conservedMoietySpeciesSet.find(*i) !=
                    conservedMoietySpeciesSet.end())
            {
                // keep incrementing the size each time we add one.
                uint cmIndex = floatingSpeciesToConservedMoietyIdMap.size();
                floatingSpeciesToConservedMoietyIdMap[si] = cmIndex;
            }
        }
    }

    // stuff the species in the map
    for (list<string>::const_iterator i = indInitFltSpecies.begin();
            i != indInitFltSpecies.end(); ++i)
    {
        uint si = initFloatingSpeciesMap.size();
        initFloatingSpeciesMap[*i] = si;
    }

    for (list<string>::const_iterator i = depInitFltSpecies.begin();
            i != depInitFltSpecies.end(); ++i)
    {
        uint si = initFloatingSpeciesMap.size();
        initFloatingSpeciesMap[*i] = si;
    }

    // finally set how many ind species we've found
    independentFloatingSpeciesSize = indFltSpecies.size();
    independentInitFloatingSpeciesSize = indInitFltSpecies.size();

    // map the float species to their compartments.
    floatingSpeciesCompartmentIndices.resize(floatingSpeciesMap.size());

    for(StringUIntMap::const_iterator i = floatingSpeciesMap.begin();
            i != floatingSpeciesMap.end(); ++i)
    {
        const Species* s = model->getSpecies(i->first);
        assert(s && "known species is NULL");
        StringUIntMap::const_iterator j =
                compartmentsMap.find(s->getCompartment());
        if (j == compartmentsMap.end())
        {
            throw_llvm_exception("species " + s->getId() +
                    " references unknown compartment " + s->getCompartment());
        }

        assert(i->second < floatingSpeciesCompartmentIndices.size());
        assert(j->second < compartmentsMap.size());
        floatingSpeciesCompartmentIndices[i->second] = j->second;
    }

    if (Logger::LOG_DEBUG <= getLogger().getLevel())
    {
        LoggingBuffer log(Logger::LOG_DEBUG, __FILE__, __LINE__);

        log.stream() << "found " << indFltSpecies.size()
                            << " independent and " << depFltSpecies.size()
                            << " dependent floating species." << endl;

        vector<string> ids = getFloatingSpeciesIds();
        for (uint i = 0; i < ids.size(); ++i)
        {
            log.stream() << "floating species [" << i << "] = \'" << ids[i]
                                                                         << "\'" << endl;
        }


        log.stream() << "found " << indInitFltSpecies.size()
                            << " independent and " << depInitFltSpecies.size()
                            << " dependent initial floating species." << endl;
    }
}

void LLVMModelDataSymbols::initCompartments(const libsbml::Model *model)
{
    list<string> indCompartments;
    list<string> depCompartments;

    list<string> indInitCompartments;
    list<string> depInitCompartments;

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

        if (isIndependentInitElement(id))
        {
            indInitCompartments.push_back(id);
        }
        else
        {
            depInitCompartments.push_back(id);
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


    for (list<string>::const_iterator i = indInitCompartments.begin();
            i != indInitCompartments.end(); ++i)
    {
        uint ci = initCompartmentsMap.size();
        initCompartmentsMap[*i] = ci;
    }

    for (list<string>::const_iterator i = depInitCompartments.begin();
            i != depInitCompartments.end(); ++i)
    {
        uint ci = initCompartmentsMap.size();
        initCompartmentsMap[*i] = ci;
    }


    // finally set how many ind compartments we have
    independentCompartmentSize = indCompartments.size();
    independentInitCompartmentSize = indInitCompartments.size();
}

bool LLVMModelDataSymbols::isConservedMoietyParameter(uint id) const
{
    return id < conservedMoietyGlobalParameter.size() ?
            conservedMoietyGlobalParameter[id] : false;
}

bool LLVMModelDataSymbols::isRateRuleGlobalParameter(uint gid) const
{
    return gid < globalParameterRateRules.size()
            ? globalParameterRateRules[gid] : false;
}

std::string LLVMModelDataSymbols::getGlobalParameterId(uint indx) const
{
    for (StringUIntMap::const_iterator i = globalParametersMap.begin();
            i != globalParametersMap.end(); ++i)
    {
        if (i->second == indx)
        {
            return i->first;
        }
    }

    throw std::out_of_range("attempted to access global parameter id at index " + rr::toString(indx));
}

uint LLVMModelDataSymbols::getCompartmentIndexForFloatingSpecies(
        uint floatIndex) const
{
    if (floatIndex >= floatingSpeciesCompartmentIndices.size())
    {
         throw std::out_of_range(std::string("index out of range in ") + __FUNC__);
    }
    return floatingSpeciesCompartmentIndices[floatIndex];
}

bool LLVMModelDataSymbols::isConservedMoietySpecies(uint id, uint& result) const
{
    UIntUIntMap::const_iterator i = floatingSpeciesToConservedMoietyIdMap.find(id);
    if (i != floatingSpeciesToConservedMoietyIdMap.end())
    {
        result = i->second;
        return true;
    }
    return false;
}

/**
 * row is species, column is reaction
 */
void  LLVMModelDataSymbols::setNamedSpeciesReferenceInfo(uint row, uint column,
        SpeciesReferenceType type)
{
    for (StringRefInfoMap::iterator i = namedSpeciesReferenceInfo.begin();
            i != namedSpeciesReferenceInfo.end(); ++i)
    {
        SpeciesReferenceInfo& sr = i->second;
        if (sr.row == row && sr.column == column)
        {
            sr.type = type;
        }
    }
}

typedef std::vector<LLVMModelDataSymbols::SpeciesReferenceType>::size_type ssize_type;
typedef cxx11_ns::unordered_map<uint, ssize_type> UIntUMap;



void LLVMModelDataSymbols::initReactions(const libsbml::Model* model)
{
    // get the reactions
    const ListOfReactions *reactions = model->getListOfReactions();
    for (uint i = 0; i < reactions->size(); i++)
    {
        const Reaction *reaction = reactions->get(i);
        if (reaction->isSetFast() && reaction->getFast()==true) {
          Log(Logger::LOG_WARNING)
            << "Unable to handle SBML fast reactions. Reaction '"
            << reaction->getId() << "' treated as a slow reaction.";
        }
        reactionsMap.insert(StringUIntPair(reaction->getId(), i));

        // keep track of species in this reaction.
        UIntUMap speciesMap;

        // go through the reaction reactants and products to know how much to
        // allocate space for the stochiometry matrix.
        // all species that participate in reactions must be floating.
        const ListOfSpeciesReferences *reactants = reaction->getListOfReactants();
        for (uint j = 0; j < reactants->size(); j++)
        {
            const SimpleSpeciesReference *r = reactants->get(j);

            if (isValidFloatingSpeciesReference(r, "reactant"))
            {
                // at this point, we'd better have a floating species
                uint speciesIdx = getFloatingSpeciesIndex(r->getSpecies());

                UIntUMap::const_iterator si = speciesMap.find(speciesIdx);

                if (si == speciesMap.end())
                {
                    stoichColIndx.push_back(i);
                    stoichRowIndx.push_back(speciesIdx);
                    stoichIds.push_back(r->isSetId() ? r->getId() : "");
                    stoichTypes.push_back(Reactant);

                    // in case this species is both a product and reactant, can look up
                    // index of the just added Reactant
                    speciesMap[speciesIdx] = stoichTypes.size() - 1;

                    if(r->isSetId() && r->getId().length() > 0)
                    {
                        if (namedSpeciesReferenceInfo.find(r->getId()) ==
                                namedSpeciesReferenceInfo.end())
                        {
                            SpeciesReferenceInfo info =
                            {speciesIdx, i, Reactant, r->getId()};
                            namedSpeciesReferenceInfo[r->getId()] = info;
                        }
                        else
                        {
                            string msg = "Species Reference with id ";
                            msg += r->getId();
                            msg += " appears more than once in the model";
                            throw_llvm_exception(msg);
                        }
                    }
                }
                else
                {
                    Log(Logger::LOG_INFORMATION)
                        << "Experimental multi product-reactant stochiometry code"
                        << "with reactant " << r->getSpecies();

                    // species is listed multiple times as reactant
                    stoichTypes[si->second] = MultiReactantProduct;

                    // set all the other ones to Multi...
                    setNamedSpeciesReferenceInfo(speciesIdx, i, MultiReactantProduct);

                    if(r->isSetId() && r->getId().length() > 0)
                    {
                        if (namedSpeciesReferenceInfo.find(r->getId()) ==
                                namedSpeciesReferenceInfo.end())
                        {
                            SpeciesReferenceInfo info =
                            {speciesIdx, i, MultiReactantProduct, r->getId()};
                            namedSpeciesReferenceInfo[r->getId()] = info;
                        }
                        else
                        {
                            string msg = "Species Reference with id ";
                            msg += r->getId();
                            msg += " appears more than once in the model";
                            throw_llvm_exception(msg);
                        }
                    }
                }
            }
        }

        const ListOfSpeciesReferences *products = reaction->getListOfProducts();
        for (uint j = 0; j < products->size(); j++)
        {
            const SimpleSpeciesReference *p = products->get(j);
            // products had better be in the stoich matrix.

            if (isValidFloatingSpeciesReference(p, "product"))
            {
                uint speciesIdx = getFloatingSpeciesIndex(p->getSpecies());

                UIntUMap::const_iterator si = speciesMap.find(speciesIdx);

                if (si == speciesMap.end())
                {
                    // its not already a reactant, can add another
                    // non-zero stoich entry
                    stoichColIndx.push_back(i);
                    stoichRowIndx.push_back(speciesIdx);
                    stoichIds.push_back(p->isSetId() ? p->getId() : "");
                    stoichTypes.push_back(Product);

                    if (p->isSetId() && p->getId().length() > 0)
                    {
                        if (namedSpeciesReferenceInfo.find(p->getId())
                                == namedSpeciesReferenceInfo.end())
                        {
                            SpeciesReferenceInfo info =
                            { speciesIdx, i, Product, p->getId()};
                            namedSpeciesReferenceInfo[p->getId()] = info;
                        }
                        else
                        {
                            string msg = "Species Reference with id ";
                            msg += p->getId();
                            msg += " appears more than once in the model";
                            throw_llvm_exception(msg);
                        }
                    }
                }
                else
                {
                    Log(Logger::LOG_INFORMATION)
                        << "Experimental multi product stochiometry code "
                        << "with product " << p->getSpecies();

                    // species is listed multiple times as product
                    stoichTypes[si->second] = MultiReactantProduct;

                    // set all the other ones to Multi...
                    setNamedSpeciesReferenceInfo(speciesIdx, i, MultiReactantProduct);

                    if(p->isSetId() && p->getId().length() > 0)
                    {
                        if (namedSpeciesReferenceInfo.find(p->getId()) ==
                                namedSpeciesReferenceInfo.end())
                        {
                            SpeciesReferenceInfo info =
                            {speciesIdx, i, MultiReactantProduct, p->getId()};
                            namedSpeciesReferenceInfo[p->getId()] = info;
                        }
                        else
                        {
                            string msg = "Species Reference with id ";
                            msg += p->getId();
                            msg += " appears more than once in the model";
                            throw_llvm_exception(msg);
                        }
                    }
                }
            }
        }
    }
}



bool LLVMModelDataSymbols::isValidFloatingSpeciesReference(
        const libsbml::SimpleSpeciesReference* ref, const std::string& reacOrProd)
{
    string id = ref->getSpecies();

    // can only define a reaction for a floating species
    if (isIndependentFloatingSpecies(id))
    {
        return true;
    }

    if (isBoundarySpecies(id))
    {
        return false;
    }

    string err = "the species reference with id ";
    err += string("\'" + ref->getId() + "\', ");
    err += "which references species ";
    string("\'" + id + "\', ");
    err += "is NOT a valid " + reacOrProd + " reference, ";
    // figure out what kind of thing we have and give a warning
    if (hasAssignmentRule(id))
    {
        err += "it is defined by an assignment rule";
    }
    else if (hasRateRule(id))
    {
        err += "it is defined by rate rule";
    }
    else
    {
        err += "it is not a species";
    }

    Log(Logger::LOG_WARNING) << err;

    return false;
}

void LLVMModelDataSymbols::displayCompartmentInfo()
{
    if (Logger::LOG_DEBUG <= getLogger().getLevel())
    {
        LoggingBuffer log(Logger::LOG_DEBUG, __FILE__, __LINE__);

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
}

void LLVMModelDataSymbols::initEvents(const libsbml::Model* model)
{
    const ListOfEvents *events = model->getListOfEvents();

    if (events->size())
    {
        eventAttributes.resize(events->size());
        eventAssignmentsSize.resize(events->size());

        // load the event attributes
        for (uint i = 0; i < events->size(); ++i)
        {
            const Event *event = events->get(i);
            unsigned char attr = 0;
            if (event->getUseValuesFromTriggerTime())
            {
                attr = attr | EventUseValuesFromTriggerTime;
            }

            const Trigger *trigger = event->getTrigger();
            assert(trigger && "must have trigger");
            if (trigger->isSetInitialValue() && trigger->getInitialValue())
            {
                attr = attr | EventInitialValue;
            }

            // older versions seem to default to persisent
            const SBMLDocument *doc = model->getSBMLDocument();
            if (doc->getLevel() < 3 ||
                    (trigger->isSetPersistent() && trigger->getPersistent()))
            {
                attr = attr | EventPersistent;
            }

            eventAttributes[i] = attr;
            eventAssignmentsSize[i] = event->getListOfEventAssignments()->size();

            if (event->isSetId())
            {
                eventIds.insert(StringUIntPair(event->getId(), i));
            }
        }
    }
}

const std::vector<unsigned char>& LLVMModelDataSymbols::getEventAttributes() const
{
    return eventAttributes;
}

uint LLVMModelDataSymbols::getEventBufferSize(uint eventId) const
{
    assert(eventId <= eventAssignmentsSize.size() && "event id out of range");
    return eventAssignmentsSize[eventId];
}

bool LLVMModelDataSymbols::isNamedSpeciesReference(const std::string& id) const
{
    return namedSpeciesReferenceInfo.find(id) != namedSpeciesReferenceInfo.end();
}

const LLVMModelDataSymbols::SpeciesReferenceInfo&
    LLVMModelDataSymbols::getNamedSpeciesReferenceInfo(const std::string& id) const
{
    StringRefInfoMap::const_iterator i = namedSpeciesReferenceInfo.find(id);
    if (i != namedSpeciesReferenceInfo.end())
    {
        return i->second;
    }
    else
    {
        throw_llvm_exception(id + " is not a named SpeciesReference");
        return *(SpeciesReferenceInfo*)(0); // shutup eclipse warnings
    }
}


uint LLVMModelDataSymbols::getFloatingSpeciesInitIndex(
        const std::string& symbol) const
{
    StringUIntMap::const_iterator i = initFloatingSpeciesMap.find(symbol);
    if(i != initFloatingSpeciesMap.end())
    {
        return i->second;
    }
    else
    {
        throw LLVMException("could not find init floating species with id " + symbol);
    }
    // never get here, just silence eclipse warnings
    return 0;
}

uint LLVMModelDataSymbols::getCompartmentInitIndex(
        const std::string& symbol) const
{
    StringUIntMap::const_iterator i = initCompartmentsMap.find(symbol);
    if(i != initCompartmentsMap.end())
    {
        return i->second;
    }
    else
    {
        throw LLVMException("could not find init compartment with id " + symbol);
    }
}


uint LLVMModelDataSymbols::getGlobalParameterInitIndex(
        const std::string& symbol) const
{
    StringUIntMap::const_iterator i = initGlobalParametersMap.find(symbol);
    if(i != initGlobalParametersMap.end())
    {
        return i->second;
    }
    else
    {
        throw LLVMException("could not find init global parameter with id " + symbol);
    }
}


bool LLVMModelDataSymbols::isConservedMoietySpecies(const std::string& symbol) const
{
    return conservedMoietySpeciesSet.find(symbol) != conservedMoietySpeciesSet.end();
}

bool LLVMModelDataSymbols::isIndependentInitFloatingSpecies(
        const std::string& symbol) const
{
    StringUIntMap::const_iterator i = initFloatingSpeciesMap.find(symbol);
    return i != initFloatingSpeciesMap.end() &&
            i->second < independentInitFloatingSpeciesSize;
}

bool LLVMModelDataSymbols::isIndependentInitCompartment(
        const std::string& symbol) const
{
    StringUIntMap::const_iterator i = initCompartmentsMap.find(symbol);
    return i != initCompartmentsMap.end() &&
            i->second < independentInitCompartmentSize;
}

bool LLVMModelDataSymbols::isIndependentInitGlobalParameter(
        const std::string& symbol) const
{
    StringUIntMap::const_iterator i = initGlobalParametersMap.find(symbol);
    return i != initGlobalParametersMap.end() &&
            i->second < independentInitGlobalParameterSize;
}

bool LLVMModelDataSymbols::isIndependentInitGlobalParameter(uint id) const
{
    return isIndependentInitGlobalParameter(getGlobalParameterId(id));
}

bool LLVMModelDataSymbols::isIndependentInitElement(
        const std::string& id) const
{
    return (initAssignmentRules.find(id) == initAssignmentRules.end() &&
            assignmentRules.find(id) == assignmentRules.end()) ||
            (isConservedMoietySpecies(id) && hasInitialAssignmentRule(id));
}

bool LLVMModelDataSymbols::hasInitialAssignmentRule(const std::string& id) const
{
    return initAssignmentRules.find(id) != initAssignmentRules.end();
}

uint LLVMModelDataSymbols::getInitCompartmentSize() const
{
    return independentInitCompartmentSize;
}

uint LLVMModelDataSymbols::getInitFloatingSpeciesSize() const
{
    return independentInitFloatingSpeciesSize;
}

uint LLVMModelDataSymbols::getInitBoundarySpeciesSize() const
{
    return independentInitBoundarySpeciesSize;
}

uint LLVMModelDataSymbols::getInitGlobalParameterSize() const
{
    return independentInitGlobalParameterSize;
}

std::vector<std::string> LLVMModelDataSymbols::getEventIds() const
{
    return getIds(eventIds);
}

std::string LLVMModelDataSymbols::getEventId(uint indx) const
{
    for (StringUIntMap::const_iterator i = eventIds.begin();
            i != eventIds.end(); ++i)
    {
        if (i->second == indx)
        {
            return i->first;
        }
    }

    throw std::out_of_range("attempted to access event id at index " + rr::toString(indx));
}

uint LLVMModelDataSymbols::getEventIndex(const std::string& id) const
{
    StringUIntMap::const_iterator i = eventIds.find(id);
    if (i != eventIds.end())
    {
        return i->second;
    }
    else
    {
        throw LLVMException("could not find event with id " + id, __FUNC__);
    }
}

uint LLVMModelDataSymbols::getConservedMoietySize() const
{
    return conservedMoietyGlobalParameterIndex.size();
}

uint LLVMModelDataSymbols::getDepSpeciesIndexForConservedMoietyId(std::string id) const
{
    StringUIntMap::const_iterator i = conservedMoietyDepSpecies.find(id);
    if (i != conservedMoietyDepSpecies.end())
    {
        return i->second;
    }
    else
    {
        throw LLVMException("could not find dep species for cm with id " + id, __FUNC__);
    }
}

const std::vector<uint>& LLVMModelDataSymbols::getIndSpeciesIndexForConservedMoietyId(std::string id) const
{
    StringUIntVectorMap::const_iterator i = conservedMoietyIndSpecies.find(id);
    if (i != conservedMoietyIndSpecies.end())
    {
        return i->second;
    }
    else
    {
        throw LLVMException("could not find ind species for cm with id " + id, __FUNC__);
    }
}

uint LLVMModelDataSymbols::getConservedMoietyGlobalParameterIndex(
        uint cmIndex) const
{
    if (cmIndex >= conservedMoietyGlobalParameterIndex.size())
    {
        throw std::out_of_range("attempt to access conserved moiety to global "
                "param array at index" + rr::toString(cmIndex));
    }
    return conservedMoietyGlobalParameterIndex[cmIndex];
}

std::string LLVMModelDataSymbols::getConservedMoietyId(uint indx) const
{
    return getGlobalParameterId(
            getConservedMoietyGlobalParameterIndex(indx));
}

uint LLVMModelDataSymbols::getConservedMoietyIndex(
        const std::string& name) const
{
    // rarely used method, less space than another map...

    // throws if not found.
    uint gp = getGlobalParameterIndex(name);

    for (std::vector<uint>::const_iterator i =
            conservedMoietyGlobalParameterIndex.begin();
            i != conservedMoietyGlobalParameterIndex.end(); ++i)
    {
        if (*i == gp)
        {
            return *i;
        }
    }

    throw std::out_of_range("The symbol \"" + name + "\" is not a conserved moeity");
}

void LLVMModelDataSymbols::saveState(std::ostream& out) const
{
	rr::saveBinary(out, conservedMoietySpeciesSet);
	rr::saveBinary(out, conservedMoietyGlobalParameter);
	rr::saveBinary(out, conservedMoietyGlobalParameterIndex);
	rr::saveBinary(out, floatingSpeciesToConservedMoietyIdMap);
	rr::saveBinary(out, conservedMoietyDepSpecies);	
	rr::saveBinary(out, conservedMoietyIndSpecies);
	rr::saveBinary(out, initAssignmentRules);
	rr::saveBinary(out, initFloatingSpeciesMap);
	rr::saveBinary(out, initBoundarySpeciesMap);
	rr::saveBinary(out, initCompartmentsMap);
	rr::saveBinary(out, initGlobalParametersMap);

	rr::saveBinary(out, independentInitFloatingSpeciesSize);
	rr::saveBinary(out, independentInitBoundarySpeciesSize);
	rr::saveBinary(out, independentInitGlobalParameterSize);
	rr::saveBinary(out, independentInitCompartmentSize);
    
	rr::saveBinary(out, floatingSpeciesCompartmentIndices);

	rr::saveBinary(out, modelName);
	rr::saveBinary(out, floatingSpeciesMap);
	rr::saveBinary(out, boundarySpeciesMap);
	rr::saveBinary(out, compartmentsMap);
	rr::saveBinary(out, globalParametersMap);

	saveStringRefInfoMap(out, namedSpeciesReferenceInfo);

	rr::saveBinary(out, reactionsMap);
	rr::saveBinary(out, stoichColIndx);
	rr::saveBinary(out, stoichRowIndx);

	rr::saveBinary(out, stoichIds);

	rr::saveBinary(out, stoichTypes);

	rr::saveBinary(out, assignmentRules);
	rr::saveBinary(out, rateRules);
	rr::saveBinary(out, globalParameterRateRules);
	rr::saveBinary(out, independentFloatingSpeciesSize);
	rr::saveBinary(out, independentBoundarySpeciesSize);
	rr::saveBinary(out, independentGlobalParameterSize);
	rr::saveBinary(out, independentCompartmentSize);

	rr::saveBinary(out, eventAssignmentsSize);
	rr::saveBinary(out, eventAttributes);
	rr::saveBinary(out, eventIds);
}

void LLVMModelDataSymbols::loadState(std::istream& in) 
{
	rr::loadBinary(in, conservedMoietySpeciesSet);
	rr::loadBinary(in, conservedMoietyGlobalParameter);
	rr::loadBinary(in, conservedMoietyGlobalParameterIndex);
	rr::loadBinary(in, floatingSpeciesToConservedMoietyIdMap);
	rr::loadBinary(in, conservedMoietyDepSpecies);	
	rr::loadBinary(in, conservedMoietyIndSpecies);
	rr::loadBinary(in, initAssignmentRules);
	rr::loadBinary(in, initFloatingSpeciesMap);
	rr::loadBinary(in, initBoundarySpeciesMap);
	rr::loadBinary(in, initCompartmentsMap);
	rr::loadBinary(in, initGlobalParametersMap);

	rr::loadBinary(in, independentInitFloatingSpeciesSize);
	rr::loadBinary(in, independentInitBoundarySpeciesSize);
	rr::loadBinary(in, independentInitGlobalParameterSize);
	rr::loadBinary(in, independentInitCompartmentSize);
    
	rr::loadBinary(in, floatingSpeciesCompartmentIndices);

	rr::loadBinary(in, modelName);
	rr::loadBinary(in, floatingSpeciesMap);
	rr::loadBinary(in, boundarySpeciesMap);
	rr::loadBinary(in, compartmentsMap);
	rr::loadBinary(in, globalParametersMap);

	loadStringRefInfoMap(in, namedSpeciesReferenceInfo);

	rr::loadBinary(in, reactionsMap);
	rr::loadBinary(in, stoichColIndx);
	rr::loadBinary(in, stoichRowIndx);
	rr::loadBinary(in, stoichIds);

	rr::loadBinary(in, stoichTypes);

	rr::loadBinary(in, assignmentRules);

	rr::loadBinary(in, rateRules);
	rr::loadBinary(in, globalParameterRateRules);
	rr::loadBinary(in, independentFloatingSpeciesSize);
	rr::loadBinary(in, independentBoundarySpeciesSize);
	rr::loadBinary(in, independentGlobalParameterSize);
	rr::loadBinary(in, independentCompartmentSize);

	rr::loadBinary(in, eventAssignmentsSize);
	rr::loadBinary(in, eventAttributes);
	rr::loadBinary(in, eventIds);
}

void LLVMModelDataSymbols::saveStringRefInfoMap(std::ostream& out, const StringRefInfoMap& m) const
{
	rr::saveBinary(out, m.size());
	for (std::pair<std::string, SpeciesReferenceInfo> p : m)
	{
		rr::saveBinary(out, p.first);
	    saveBinarySpeciesReferenceInfo(out, p.second);
	}
}

void LLVMModelDataSymbols::loadStringRefInfoMap(std::istream& in, StringRefInfoMap& m)
{
	size_t msize;
	rr::loadBinary(in, msize);
	m.clear();
	for (int i = 0; i < msize; i++) 
	{
		std::string s;
		SpeciesReferenceInfo sri;
		rr::loadBinary(in, s);
		loadBinarySpeciesReferenceInfo(in, sri);
		m.emplace(s, sri);
	}
}

void LLVMModelDataSymbols::saveBinarySpeciesReferenceInfo(std::ostream& out, SpeciesReferenceInfo& sri) const
{
    rr::saveBinary(out, sri.row);
	rr::saveBinary(out, sri.column);
	rr::saveBinary(out, sri.type);
	rr::saveBinary(out, sri.id);
}

void LLVMModelDataSymbols::loadBinarySpeciesReferenceInfo(std::istream& in, SpeciesReferenceInfo& sri)
{
    rr::loadBinary(in, sri.row);
	rr::loadBinary(in, sri.column);
	rr::loadBinary(in, sri.type);
	rr::loadBinary(in, sri.id);
}

} /* namespace rr */
