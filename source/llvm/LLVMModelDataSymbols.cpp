/*
 * LLVMModelDataSymbols.cpp
 *
 *  Created on: Jun 28, 2013
 *      Author: andy
 */
#include "LLVMModelDataSymbols.h"
#include "lsLibStructural.h"
#include "LLVMException.h"
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
    "RateRuleRates",                            // 12
    "LocalParametersOffsets",                   // 13
    "LocalParametersNum",                       // 14
    "LocalParameters",                          // 15
    "NumFloatingSpecies",                       // 16
    "FloatingSpeciesAmountRates",               // 17
    "FloatingSpeciesAmounts",                   // 18
    "NumBoundarySpecies",                       // 19
    "BoundarySpeciesAmounts",                   // 20
    "NumCompartments",                          // 21
    "CompartmentVolumes",                       // 22
    "Stoichiometry",                            // 23
    "NumEvents",                                // 24
    "StateVectorSize",                          // 25
    "StateVector",                              // 26
    "StateVectorRate",                          // 27
    "EventAssignmentsSize",                     // 28
    "EventAssignments",                         // 29
    "WorkSize",                                 // 30
    "Work",                                     // 31
    "ModelName"                                 // 32
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
        independentCompartmentSize(0),
        eventAssignmentSize(0)
{
}

LLVMModelDataSymbols::LLVMModelDataSymbols(const libsbml::Model *model,
        bool computeAndAssignConsevationLaws) :
        linearlyIndependentFloatingSpeciesSize(0),
        independentFloatingSpeciesSize(0),
        independentBoundarySpeciesSize(0),
        independentGlobalParameterSize(0),
        independentCompartmentSize(0),
        eventAssignmentSize(0)
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
    initCompartments(model);


    // process the floating species
    initFloatingSpecies(model, computeAndAssignConsevationLaws);

    // display compartment info. We need to get the compartments before the
    // so we can get the species compartments. But the struct anal dumps
    // a bunch of stuff, so to keep things looking nice in the log, we
    // display the compartment info here.
    displayCompartmentInfo();

    initBoundarySpecies(model);

    initGlobalParameters(model);

    initReactions(model);

    initEvents(model);
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
    if(i != floatingSpeciesMap.end() && i->second < independentFloatingSpeciesSize)
    {
        return i->second;
    }
    else
    {
        string msg = "could not determine index for floating species with id ";
        msg += string("\'" + id + "\', ");
        if(i != floatingSpeciesMap.end())
        {
            msg += " it is not a floating species";
        }
        else if (i->second >= independentFloatingSpeciesSize)
        {
            msg += " it is a floating species, but not indenpendent"
                    " -- it is defined by a rule";
        }

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
    m.numIndependentSpecies         = independentFloatingSpeciesSize;

    //mData.numDependentSpecies           = ms.mNumDependentSpecies;
    m.numGlobalParameters           = independentGlobalParameterSize;
    m.numReactions                  = reactionsMap.size();
    m.numEvents                     = eventAttributes.size();
    m.numFloatingSpecies            = independentFloatingSpeciesSize;
    m.numRateRules                  = rateRules.size();
    m.numCompartments               = independentCompartmentSize;
    m.numBoundarySpecies            = independentBoundarySpeciesSize;
    m.eventAssignmentsSize          = getEventAssignmentSize();

    m.modelName = strdup(modelName.c_str());

    // in certain cases, the data returned by c++ new may be alligned differently than
    // malloc, so just use calloc here just to be safe, plus calloc returns zero
    // initialized memory.


    m.floatingSpeciesAmounts = (double*)calloc(m.numIndependentSpecies, sizeof(double));
    m.floatingSpeciesAmountRates = (double*)calloc(m.numIndependentSpecies, sizeof(double));
    m.rateRuleValues = (double*)calloc(m.numRateRules, sizeof(double));
    m.rateRuleRates = (double*)calloc(m.numRateRules, sizeof(double));

    m.reactionRates = (double*)calloc(m.numReactions, sizeof(double));
    //m.dependentSpeciesConservedSums = (double*)rrCalloc(m.numDependentSpecies, sizeof(double));
    //m.floatingSpeciesInitConcentrations = (double*)rrCalloc(m.numFloatingSpecies, sizeof(double));
    m.globalParameters = (double*)calloc(m.numGlobalParameters, sizeof(double));
    m.compartmentVolumes = (double*)calloc(m.numCompartments, sizeof(double));
    //m.boundarySpeciesConcentrations = (double*)rrCalloc(m.numBoundarySpecies, sizeof(double));
    m.boundarySpeciesAmounts = (double*)calloc(m.numBoundarySpecies, sizeof(double));

    //m.work = (double*)rrCalloc(m.workSize, sizeof(double));


    // allocate the stoichiometry matrix
    m.stoichiometry = csr_matrix_new(m.numIndependentSpecies, getReactionSize(),
            stoichRowIndx, stoichColIndx, vector<double>(stoichRowIndx.size(), 0));


    m.eventAssignments = (double*)calloc(m.eventAssignmentsSize, sizeof(double));
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

uint LLVMModelDataSymbols::getEventAssignmentOffset(uint eventIndx) const
{
    return eventAssignmentOffsets[eventIndx];
}

uint LLVMModelDataSymbols::getEventAssignmentSize() const
{
    return eventAssignmentSize;
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

void LLVMModelDataSymbols::initGlobalParameters(const libsbml::Model* model)
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

void LLVMModelDataSymbols::initFloatingSpecies(const libsbml::Model* model,
        bool computeAndAssignConsevationLaws)
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

void LLVMModelDataSymbols::initCompartments(const libsbml::Model *model)
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


void LLVMModelDataSymbols::initReactions(const libsbml::Model* model)
{
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

            if (isValidSpeciesReference(r, "reactant"))
            {
                // at this point, we'd better have a floating species
                uint speciesIdx = getFloatingSpeciesIndex(r->getSpecies());
                stoichColIndx.push_back(i);
                stoichRowIndx.push_back(speciesIdx);
            }
        }

        const ListOfSpeciesReferences *products = r->getListOfProducts();
        for (uint j = 0; j < products->size(); j++)
        {
            const SimpleSpeciesReference *p = products->get(j);
            // products had better be in the stoich matrix.

            if (isValidSpeciesReference(p, "product"))
            {
                uint speciesIdx = getFloatingSpeciesIndex(p->getSpecies());
                stoichColIndx.push_back(i);
                stoichRowIndx.push_back(speciesIdx);
            }
        }
    }
}

bool LLVMModelDataSymbols::isValidSpeciesReference(
        const libsbml::SimpleSpeciesReference* ref, const std::string& reacOrProd)
{
    string id = ref->getSpecies();

    // can only define a reaction for a floating species
    if (isIndependentFloatingSpecies(id))
    {
        return true;
    }
    else
    {
        string id = ref->getSpecies();
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
        else if (isIndependentBoundarySpecies(id))
        {
            err += "it is a boundary species";
        }
        else
        {
            err += "it is not a species";
        }

        err += ", it will be ignored.";
        Log(Logger::PRIO_WARNING) << err;
        return false;
    }
}

void LLVMModelDataSymbols::displayCompartmentInfo()
{
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
}

void LLVMModelDataSymbols::initEvents(const libsbml::Model* model)
{
    const ListOfEvents *events = model->getListOfEvents();

    if (events->size())
    {
        eventAssignmentOffsets.resize(events->size());
        eventAttributes.resize(events->size());

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

            if (trigger->isSetPersistent() && trigger->getPersistent())
            {
                attr = attr | EventPersistent;
            }

            eventAttributes[i] = attr;
        }

        // figure out where to store the event assigment values.
        eventAssignmentOffsets[0] = 0;
        for (uint i = 0; i < events->size() - 1; ++i)
        {
            const Event *event = events->get(i);

            eventAssignmentOffsets[i+1] = eventAssignmentOffsets[i] +
                    event->getNumEventAssignments();
        }

        eventAssignmentSize = eventAssignmentOffsets.back() +
                events->get(events->size() - 1)->getNumEventAssignments();
    }
}

const std::vector<unsigned char>& LLVMModelDataSymbols::getEventAttributes() const
{
    return eventAttributes;
}

} /* namespace rr */


