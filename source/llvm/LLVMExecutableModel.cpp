/*
 * LLVMExecutableModel.cpp
 *
 * Author: Andy Somogyi,
 *     email decode: V1 = "."; V2 = "@"; V3 = V1;
 *     andy V1 somogyi V2 gmail V3 com
 */
#pragma hdrstop
#include "LLVMExecutableModel.h"
#include "ModelResources.h"
#include "LLVMIncludes.h"
#include "rrSparse.h"
#include "rrLogger.h"
#include "rrException.h"
#include "LLVMException.h"
#include "rrSelectionRecord.h"
#include <iomanip>
#include <cstdlib>

using rr::Logger;
using rr::getLogger;
using rr::LoggingBuffer;
using rr::SelectionRecord;

#if defined (_WIN32)
#define isnan _isnan
#else
#define isnan std::isnan
#endif

using namespace std;

// static assertion, taken from
// http://msdn.microsoft.com/en-us/library/windows/desktop/ms679289(v=vs.85).aspx
#ifndef C_ASSERT
#define C_ASSERT(e) typedef char __C_ASSERT__[(e)?1:-1]
#endif

template <typename numeric_type>
static void dump_array(std::ostream &os, int n, const numeric_type *p)
{
    if (p)
    {
        os << setiosflags(ios::floatfield) << setprecision(8);
        os << '[';
        for (int i = 0; i < n; ++i)
        {
            os << p[i];
            if (i < n - 1)
            {
                os << ", ";
            }
        }
        os << ']' << endl;
    }
    else
    {
        os << "NULL" << endl;
    }
}

typedef string (rr::ExecutableModel::*getNamePtr)(int);
typedef int (rr::ExecutableModel::*getNumPtr)();

/**
 * checks if the bitfield value has all the required flags
 * from type
 */
inline bool checkExact(uint32_t type, uint32_t value) {
    return (value & type) == type;
}

// make this static here, hide our implementation...
static void addIds(rr::ExecutableModel *model, int start, int end,
        getNamePtr nameFunc, std::list<std::string>& ids)
{
    for(int i = start; i < end; i++)
    {
        const std::string& name  = (model->*nameFunc)(i);
        ids.push_back(name);
    }
}

// make this static here, hide our implementation...
static void addConcIds(rr::ExecutableModel *model, int start, int end,
        getNamePtr nameFunc, std::list<std::string>& ids)
{
    for(int i = start; i < end; i++)
    {
        const std::string& name  = "[" + (model->*nameFunc)(i) + "]";
        ids.push_back(name);
    }
}


namespace rrllvm
{

int LLVMExecutableModel::getValues(double (*funcPtr)(LLVMModelData*, int),
        int len, const int *indx, double *values)
{
    double value;
    for (int i = 0; i < len; ++i)
    {
        int j = indx ? indx[i] : i;
        value = funcPtr(modelData, j);

        if (isnan(value))
        {
            std::stringstream s;
            s << "error getting value for index " << j << ", probably out of range";
            throw_llvm_exception(s.str());
        }

        values[i] = value;
    }
    return len;
}

int LLVMExecutableModel::setValues(bool (*funcPtr)(LLVMModelData*, int, double),
        GetNameFuncPtr getNameFuncPtr, int len, const int *indx, const double *values)
{
    for (int i = 0; i < len; ++i)
    {
        int j = indx ? indx[i] : i;
        bool result =  funcPtr(modelData, j, values[i]);

        if (!result)
        {
            std::stringstream s;
            string id = (this->*getNameFuncPtr)(j);
            s << "could not set value for " << id;

            if (symbols->hasAssignmentRule(id))
            {
                s << ", it is defined by an assignment rule, can not be set independently.";
            }
            else if (symbols->hasInitialAssignmentRule(id))
            {
                s << ", it is defined by an initial assignment rule and can not be set independently.";
            }
            else if (symbols->hasRateRule(id))
            {
                s << ", it is defined by a rate rule and can not be set independently.";
            }

            throw_llvm_exception(s.str());
        }
    }
    return len;
}

LLVMExecutableModel::LLVMExecutableModel() :
    symbols(0),
    modelData(0),
    evalInitialConditionsPtr(0),
    evalReactionRatesPtr(0),
    getBoundarySpeciesAmountPtr(0),
    getFloatingSpeciesAmountPtr(0),
    getBoundarySpeciesConcentrationPtr(0),
    getFloatingSpeciesConcentrationPtr(0),
    getGlobalParameterPtr(0),
    getCompartmentVolumePtr(0),
    evalRateRuleRatesPtr(0),
    getEventTriggerPtr(0),
    getEventPriorityPtr(0),
    getEventDelayPtr(0),
    eventTriggerPtr(0),
    eventAssignPtr(0),
    evalVolatileStoichPtr(0),
    evalConversionFactorPtr(0),
    setBoundarySpeciesAmountPtr(0),
    setFloatingSpeciesAmountPtr(0),
    setBoundarySpeciesConcentrationPtr(0),
    setFloatingSpeciesConcentrationPtr(0),
    setCompartmentVolumePtr(0),
    setGlobalParameterPtr(0),
    getFloatingSpeciesInitConcentrationsPtr(0),
    setFloatingSpeciesInitConcentrationsPtr(0),
    getFloatingSpeciesInitAmountsPtr(0),
    setFloatingSpeciesInitAmountsPtr(0),
    getCompartmentInitVolumesPtr(0),
    setCompartmentInitVolumesPtr(0)
{
    std::srand(std::time(0));
}

LLVMExecutableModel::LLVMExecutableModel(
    const std::tr1::shared_ptr<ModelResources>& rc, LLVMModelData* modelData) :
    resources(rc),
    symbols(rc->symbols),
    modelData(modelData),
    evalInitialConditionsPtr(rc->evalInitialConditionsPtr),
    evalReactionRatesPtr(rc->evalReactionRatesPtr),
    getBoundarySpeciesAmountPtr(rc->getBoundarySpeciesAmountPtr),
    getFloatingSpeciesAmountPtr(rc->getFloatingSpeciesAmountPtr),
    getBoundarySpeciesConcentrationPtr(rc->getBoundarySpeciesConcentrationPtr),
    getFloatingSpeciesConcentrationPtr(rc->getFloatingSpeciesConcentrationPtr),
    getCompartmentVolumePtr(rc->getCompartmentVolumePtr),
    getGlobalParameterPtr(rc->getGlobalParameterPtr),
    evalRateRuleRatesPtr(rc->evalRateRuleRatesPtr),
    getEventTriggerPtr(rc->getEventTriggerPtr),
    getEventPriorityPtr(rc->getEventPriorityPtr),
    getEventDelayPtr(rc->getEventDelayPtr),
    eventTriggerPtr(rc->eventTriggerPtr),
    eventAssignPtr(rc->eventAssignPtr),
    evalVolatileStoichPtr(rc->evalVolatileStoichPtr),
    evalConversionFactorPtr(rc->evalConversionFactorPtr),
    setBoundarySpeciesAmountPtr(rc->setBoundarySpeciesAmountPtr),
    setFloatingSpeciesAmountPtr(rc->setFloatingSpeciesAmountPtr),
    setBoundarySpeciesConcentrationPtr(rc->setBoundarySpeciesConcentrationPtr),
    setFloatingSpeciesConcentrationPtr(rc->setFloatingSpeciesConcentrationPtr),
    setCompartmentVolumePtr(rc->setCompartmentVolumePtr),
    setGlobalParameterPtr(rc->setGlobalParameterPtr),
    getFloatingSpeciesInitConcentrationsPtr(rc->getFloatingSpeciesInitConcentrationsPtr),
    setFloatingSpeciesInitConcentrationsPtr(rc->setFloatingSpeciesInitConcentrationsPtr),
    getFloatingSpeciesInitAmountsPtr(rc->getFloatingSpeciesInitAmountsPtr),
    setFloatingSpeciesInitAmountsPtr(rc->setFloatingSpeciesInitAmountsPtr),
    getCompartmentInitVolumesPtr(rc->getCompartmentInitVolumesPtr),
    setCompartmentInitVolumesPtr(rc->setCompartmentInitVolumesPtr)
{
    modelData->time = -1.0; // time is initially before simulation starts

    std::srand(std::time(0));

    eventAssignTimes.resize(modelData->numEvents);

    evalInitialConditions();
}

LLVMExecutableModel::~LLVMExecutableModel()
{
    // smart ptr takes care of freeing resources

    LLVMModelData_free(modelData);

    Log(Logger::LOG_DEBUG) << __FUNC__;
}

string LLVMExecutableModel::getModelName()
{
    return symbols->getModelName();
}

void LLVMExecutableModel::setTime(double time)
{
    modelData->time = time;
}

double LLVMExecutableModel::getTime()
{
    return modelData->time;
}


int LLVMExecutableModel::getNumIndFloatingSpecies()
{
    return modelData->numIndFloatingSpecies;
}


int LLVMExecutableModel::getNumDepFloatingSpecies()
{
    return symbols->getFloatingSpeciesSize() - modelData->numIndFloatingSpecies;
}

int LLVMExecutableModel::getNumFloatingSpecies()
{
    return symbols->getFloatingSpeciesSize();
}

int LLVMExecutableModel::getNumBoundarySpecies()
{
    return symbols->getBoundarySpeciesSize();
}

int LLVMExecutableModel::getNumGlobalParameters()
{
    return symbols->getGlobalParametersSize();
}

int LLVMExecutableModel::getNumCompartments()
{
    return symbols->getCompartmentsSize();
}

int LLVMExecutableModel::getNumReactions()
{
    return modelData->numReactions;
}

int LLVMExecutableModel::getNumLocalParameters(int reactionId)
{
    return 0;
}

void LLVMExecutableModel::convertToAmounts()
{
}

void LLVMExecutableModel::computeConservedTotals()
{
}


int LLVMExecutableModel::getFloatingSpeciesConcentrations(int len, int const *indx,
        double *values)
{
    return getValues(getFloatingSpeciesConcentrationPtr, len, indx, values);
}

void LLVMExecutableModel::getRateRuleValues(double *rateRuleValues)
{
    memcpy(rateRuleValues, modelData->rateRuleValuesAlias, modelData->numRateRules * sizeof(double));
}

void LLVMExecutableModel::setRateRuleValues(const double *rateRuleValues)
{
    memcpy(modelData->rateRuleValuesAlias, rateRuleValues, modelData->numRateRules * sizeof(double));
}

void LLVMExecutableModel::convertToConcentrations()
{
}

void LLVMExecutableModel::updateDependentSpeciesValues()
{
}

void LLVMExecutableModel::computeAllRatesOfChange()
{
}

void LLVMExecutableModel::getStateVectorRate(double time, const double *y, double *dydt)
{
    modelData->time = time;

    if (y && dydt)
    {
        // save and assign state vector
        double *savedRateRules = modelData->rateRuleValuesAlias;
        double *savedFloatingSpeciesAmounts = modelData->floatingSpeciesAmountsAlias;

        modelData->rateRuleValuesAlias = const_cast<double*>(y);
        modelData->floatingSpeciesAmountsAlias = const_cast<double*>(y + modelData->numRateRules);
        evalVolatileStoichPtr(modelData);

        double conversionFactor = evalReactionRatesPtr(modelData);

        // floatingSpeciesAmountRates only valid for the following two
        // functions, this will move to a parameter shortly...

        modelData->floatingSpeciesAmountRates = dydt + modelData->numRateRules;

        csr_matrix_dgemv(conversionFactor, modelData->stoichiometry,
                modelData->reactionRatesAlias, 0.0, modelData->floatingSpeciesAmountRates);

        evalConversionFactorPtr(modelData);

        modelData->floatingSpeciesAmountRates = 0;

        // this will also move to a parameter for the evalRateRules func...
        modelData->rateRuleRates = dydt;
        evalRateRuleRatesPtr(modelData);
        modelData->rateRuleRates = 0;

        // restore original pointers for state vector
        modelData->rateRuleValuesAlias = savedRateRules;
        modelData->floatingSpeciesAmountsAlias = savedFloatingSpeciesAmounts;
    }
    else if (y && !dydt)
    {
        setStateVector(y);
    }
    else if (!y && dydt)
    {
        // evaluate dydt using current state

        evalVolatileStoichPtr(modelData);

        double conversionFactor = evalReactionRatesPtr(modelData);

        // floatingSpeciesAmountRates only valid for the following two
        // functions, this will move to a parameter shortly...

        modelData->floatingSpeciesAmountRates = dydt + modelData->numRateRules;

        csr_matrix_dgemv(conversionFactor, modelData->stoichiometry,
                modelData->reactionRatesAlias, 0.0, modelData->floatingSpeciesAmountRates);

        evalConversionFactorPtr(modelData);

        modelData->floatingSpeciesAmountRates = 0;

        // this will also move to a parameter for the evalRateRules func...
        modelData->rateRuleRates = dydt;
        evalRateRuleRatesPtr(modelData);
        modelData->rateRuleRates = 0;
    }

    /*
    if (Logger::LOG_PRIO_TRACE <= rr::Logger::LOG_GetLogLevel()) {

        LoggingBuffer log(Logger::LOG_PRIO_TRACE, __FILE__, __LINE__);

        log.stream() << __FUNC__ << endl;
        log.stream() << "y: ";
        if (y) {
            dump_array(log.stream(), modelData->numRateRules + modelData->numFloatingSpecies, y);
        } else {
            log.stream() << "null";
        }
        log.stream() << endl << "dydt: ";
        if (dydt) {
            dump_array(log.stream(), modelData->numRateRules + modelData->numFloatingSpecies, dydt);
        } else {
            log.stream() << "null";
        }
        log.stream() << endl << "Model: " << endl << this;
    }
    */
}

void LLVMExecutableModel::testConstraints()
{
}

string LLVMExecutableModel::getInfo()
{
    stringstream stream;

    double *tmp;

    int nFloat = getNumFloatingSpecies();
    int nBound = getNumBoundarySpecies();
    int nComp = getNumCompartments();
    int nGlobalParam = getNumGlobalParameters();
    int nEvents = getNumEvents();
    int nReactions = getNumReactions();

    stream << "* Calculated Values *" << endl;

    tmp = new double[nFloat];
    getFloatingSpeciesAmounts(nFloat, 0, tmp);
    stream << "FloatingSpeciesAmounts:" << endl;
    dump_array(stream, nFloat, tmp);

    getFloatingSpeciesConcentrations(nFloat, 0, tmp);
    stream << "FloatingSpeciesConcentrations:" << endl;
    dump_array(stream, nFloat, tmp);

    this->getFloatingSpeciesInitConcentrations(nFloat, 0, tmp);
    stream << "FloatingSpeciesInitConcentrations:" << endl;
    dump_array(stream, nFloat, tmp);
    delete[] tmp;

    tmp = new double[nReactions];
    getReactionRates(nReactions, 0, tmp);
    stream << "Reaction Rates:" << endl;
    dump_array(stream, nReactions, tmp);
    delete[] tmp;

    tmp = new double[nBound];
    getBoundarySpeciesAmounts(nBound, 0, tmp);
    stream << "BoundarySpeciesAmounts:" << endl;
    dump_array(stream, nBound, tmp);

    getBoundarySpeciesConcentrations(nBound, 0, tmp);
    stream << "BoundarySpeciesConcentrations:" << endl;
    dump_array(stream, nBound, tmp);
    delete[] tmp;

    tmp = new double[nComp];
    getCompartmentVolumes(nComp, 0, tmp);
    stream << "CompartmentVolumes:" << endl;
    dump_array(stream, nComp, tmp);

    this->getCompartmentInitVolumes(nComp, 0, tmp);
    stream << "CompartmentInitVolumes:" << endl;
    dump_array(stream, nComp, tmp);
    delete[] tmp;

    tmp = new double[nGlobalParam];
    getGlobalParameterValues(nGlobalParam, 0, tmp);
    stream << "GlobalParameters:" << endl;
    dump_array(stream, nGlobalParam, tmp);
    delete[] tmp;

    tmp = new double[nGlobalParam];
    getGlobalParameterValues(nGlobalParam, 0, tmp);
    stream << "GlobalParameters:" << endl;
    dump_array(stream, nGlobalParam, tmp);
    delete[] tmp;

    unsigned char *tmpEvents = new unsigned char[nEvents];
    getEventTriggers(nEvents, 0, tmpEvents);
    stream << "Events Trigger Status:" << endl;
    dump_array(stream, nEvents, (bool*)tmpEvents);
    delete[] tmpEvents;

    stream << *modelData;

    return stream.str();
}

int LLVMExecutableModel::getFloatingSpeciesIndex(const string& id)
{
    try
    {
        return symbols->getFloatingSpeciesIndex(id, false);
    }
    catch (LLVMException&)
    {
        return -1;
    }
}

string LLVMExecutableModel::getFloatingSpeciesId(int index)
{
    vector<string> ids = symbols->getFloatingSpeciesIds();
    return ids[index];
}

int LLVMExecutableModel::getBoundarySpeciesIndex(const string& id)
{
    try
    {
        return symbols->getBoundarySpeciesIndex(id);
    }
    catch (LLVMException&)
    {
        return -1;
    }
}

string LLVMExecutableModel::getBoundarySpeciesId(int indx)
{
    vector<string> ids = symbols->getBoundarySpeciesIds();
    return ids[indx];
}

int LLVMExecutableModel::getGlobalParameterIndex(const string& id)
{
    try
    {
        return symbols->getGlobalParameterIndex(id);
    }
    catch(LLVMException&)
    {
        return -1;
    }
}

string LLVMExecutableModel::getGlobalParameterId(int id)
{
    vector<string> ids = symbols->getGlobalParameterIds();
    if (id < ids.size())
    {
        return ids[id];
    }
    else
    {
        throw_llvm_exception("index out of range");
        return "";
    }
}

int LLVMExecutableModel::getCompartmentIndex(const string& id)
{
    try
    {
        return symbols->getCompartmentIndex(id);
    }
    catch (LLVMException&)
    {
        return -1;
    }
}

string LLVMExecutableModel::getCompartmentId(int id)
{
    vector<string> ids = symbols->getCompartmentIds();
    if (id < ids.size())
    {
        return ids[id];
    }
    else
    {
        throw_llvm_exception("index out of range");
        return "";
    }
}

int LLVMExecutableModel::getReactionIndex(const string& id)
{
    try
    {
        return symbols->getReactionIndex(id);
    }
    catch (LLVMException&)
    {
        return -1;
    }
}

string LLVMExecutableModel::getReactionId(int id)
{
    vector<string> ids = symbols->getReactionIds();
    if (id < ids.size())
    {
        return ids[id];
    }
    else
    {
        throw_llvm_exception("index out of range");
        return "";
    }
}

void LLVMExecutableModel::evalInitialConditions()
{
    evalInitialConditionsPtr(modelData);
}

void LLVMExecutableModel::reset()
{
    // eval the initial conditions and rates
    setTime(0.0);

    if (getCompartmentInitVolumesPtr && getFloatingSpeciesInitAmountsPtr)
    {
        // have to set compartments first, these are used to
        // convert between concentrations and amounts.
        unsigned size = max(modelData->numIndCompartments,
                modelData->numIndFloatingSpecies);

        double *buffer = new double[size];
        getCompartmentInitVolumes(modelData->numIndCompartments, 0, buffer);
        setCompartmentVolumes(modelData->numIndCompartments, 0, buffer);

        getFloatingSpeciesInitAmounts(modelData->numIndFloatingSpecies, 0, buffer);
        setFloatingSpeciesAmounts(modelData->numIndFloatingSpecies, 0, buffer);

        delete[] buffer;
    }
    else
    {
        evalInitialConditions();
    }

    evalReactionRates();

    // this sets up the event system to pull the initial value
    // before the simulation starts.
    setTime(-1.0);

    Log(Logger::LOG_DEBUG) << __FUNC__ << *modelData;
}

bool LLVMExecutableModel::getConservedSumChanged()
{
    return false;
}

void LLVMExecutableModel::setConservedSumChanged(bool val)
{
}

int LLVMExecutableModel::getStateVector(double* stateVector)
{
    if (stateVector == 0)
    {
        Log(Logger::LOG_TRACE) << __FUNC__ << ", stateVector: null, returning "
                << modelData->numRateRules + modelData->numIndFloatingSpecies;
        return modelData->numRateRules + modelData->numIndFloatingSpecies;
    }

    getRateRuleValues(stateVector);

    memcpy(stateVector + modelData->numRateRules,
            modelData->floatingSpeciesAmountsAlias,
            modelData->numIndFloatingSpecies * sizeof(double));


    if (Logger::LOG_TRACE <= rr::Logger::getLevel()) {

        LoggingBuffer log(Logger::LOG_TRACE, __FILE__, __LINE__);

        log.stream() << endl << __FUNC__ <<  ", Model: " << endl << this;

        log.stream() << __FUNC__ << ",  out stateVector: ";
        if (stateVector) {
            dump_array(log.stream(), modelData->numRateRules + modelData->numIndFloatingSpecies, stateVector);
        } else {
            log.stream() << "null";
        }
    }

    return modelData->numRateRules + modelData->numIndFloatingSpecies;
}

int LLVMExecutableModel::setStateVector(const double* stateVector)
{
    if (stateVector == 0)
    {
        return -1;
    }

    memcpy(modelData->rateRuleValuesAlias, stateVector, modelData->numRateRules * sizeof(double));

    memcpy(modelData->floatingSpeciesAmountsAlias,
            stateVector + modelData->numRateRules,
            modelData->numIndFloatingSpecies * sizeof(double));

    evalVolatileStoichPtr(modelData);

    /*
    if (Logger::LOG_PRIO_TRACE <= rr::Logger::LOG_GetLogLevel()) {

        LoggingBuffer log(Logger::LOG_PRIO_TRACE, __FILE__, __LINE__);

        log.stream() << endl << __FUNC__ <<  ", Model: " << endl << this;

        log.stream() << __FUNC__ << ",  stateVector: ";
        if (stateVector) {
            dump_array(log.stream(), modelData->numRateRules + modelData->numIndFloatingSpecies, stateVector);
        } else {
            log.stream() << "null";
        }
    }
    */

    return modelData->numRateRules + modelData->numIndFloatingSpecies;
}

void LLVMExecutableModel::print(std::ostream &stream)
{
    stream << "LLVMExecutableModel" << endl;
    stream << getInfo();
}

void LLVMExecutableModel::getIds(int types, std::list<std::string> &ids)
{
    if (checkExact(SelectionRecord::FLOATING | SelectionRecord::AMOUNT |
            SelectionRecord::CURRENT, types)
            && (SelectionRecord::INDEPENDENT & types)) {
        addIds(this, 0, symbols->getIndependentFloatingSpeciesSize(),
                &rr::ExecutableModel::getFloatingSpeciesId, ids);
    }

    if (checkExact(SelectionRecord::FLOATING | SelectionRecord::AMOUNT |
            SelectionRecord::CURRENT, types)
            && (SelectionRecord::DEPENDENT & types)) {
        addIds(this, symbols->getIndependentFloatingSpeciesSize(),
                symbols->getFloatingSpeciesSize(),
                &rr::ExecutableModel::getFloatingSpeciesId, ids);
    }

    if (checkExact(SelectionRecord::BOUNDARY | SelectionRecord::AMOUNT |
            SelectionRecord::CURRENT, types)
            && (SelectionRecord::INDEPENDENT & types)) {
        addIds(this, 0, symbols->getIndependentBoundarySpeciesSize(),
                &rr::ExecutableModel::getBoundarySpeciesId, ids);
    }

    if (checkExact(SelectionRecord::BOUNDARY | SelectionRecord::AMOUNT |
            SelectionRecord::CURRENT, types)
            && (SelectionRecord::DEPENDENT & types)) {
        addIds(this, symbols->getIndependentBoundarySpeciesSize(),
                symbols->getBoundarySpeciesSize(),
                &rr::ExecutableModel::getBoundarySpeciesId, ids);
    }

    if (checkExact(SelectionRecord::FLOATING | SelectionRecord::CONCENTRATION |
            SelectionRecord::CURRENT, types)
            && (SelectionRecord::INDEPENDENT & types)) {
        addConcIds(this, 0, symbols->getIndependentFloatingSpeciesSize(),
                &rr::ExecutableModel::getFloatingSpeciesId, ids);
    }

    if (checkExact(SelectionRecord::FLOATING | SelectionRecord::CONCENTRATION |
            SelectionRecord::CURRENT, types)
            && (SelectionRecord::DEPENDENT & types)) {
        addConcIds(this, symbols->getIndependentFloatingSpeciesSize(),
                symbols->getFloatingSpeciesSize(),
                &rr::ExecutableModel::getFloatingSpeciesId, ids);
    }

    if (checkExact(SelectionRecord::BOUNDARY | SelectionRecord::CONCENTRATION |
            SelectionRecord::CURRENT, types)
            && (SelectionRecord::INDEPENDENT & types)) {
        addConcIds(this, 0, symbols->getIndependentBoundarySpeciesSize(),
                &rr::ExecutableModel::getBoundarySpeciesId, ids);
    }

    if (checkExact(SelectionRecord::BOUNDARY | SelectionRecord::CONCENTRATION |
            SelectionRecord::CURRENT, types)
            && (SelectionRecord::DEPENDENT & types)) {
        addConcIds(this, symbols->getIndependentBoundarySpeciesSize(),
                symbols->getBoundarySpeciesSize(),
                &rr::ExecutableModel::getBoundarySpeciesId, ids);
    }


    if (checkExact(SelectionRecord::_COMPARTMENT | SelectionRecord::CURRENT, types)
            && (SelectionRecord::INDEPENDENT & types)) {
        addIds(this, 0, symbols->getIndependentCompartmentSize(),
                &rr::ExecutableModel::getCompartmentId, ids);
    }

    if (checkExact(SelectionRecord::_COMPARTMENT | SelectionRecord::CURRENT, types)
            && (SelectionRecord::DEPENDENT & types)) {
        addIds(this, symbols->getIndependentCompartmentSize(),
                symbols->getCompartmentsSize(),
                &rr::ExecutableModel::getCompartmentId, ids);
    }

    if (checkExact(SelectionRecord::_GLOBAL_PARAMETER | SelectionRecord::CURRENT, types)
            && (SelectionRecord::INDEPENDENT & types)) {
        addIds(this, 0, symbols->getIndependentGlobalParameterSize(),
                &rr::ExecutableModel::getGlobalParameterId, ids);
    }

    if (checkExact(SelectionRecord::_GLOBAL_PARAMETER | SelectionRecord::CURRENT, types)
            && (SelectionRecord::DEPENDENT & types)) {
        addIds(this, symbols->getIndependentGlobalParameterSize(),
                symbols->getGlobalParametersSize(),
                &rr::ExecutableModel::getGlobalParameterId, ids);
    }

    if (checkExact(SelectionRecord::REACTION_RATE, types)) {
        addIds(this, 0, symbols->getReactionSize(),
                &rr::ExecutableModel::getReactionId, ids);
    }

    if (checkExact(SelectionRecord::INITIAL | SelectionRecord::FLOATING |
            SelectionRecord::CONCENTRATION, types) &&
            (SelectionRecord::INDEPENDENT & types)) {
        for (int i = 0; i < symbols->getInitFloatingSpeciesSize(); ++i) {
            ids.push_back("init([" + this->getFloatingSpeciesId(i) + "])");
        }
    }

    if (checkExact(SelectionRecord::INITIAL | SelectionRecord::FLOATING |
            SelectionRecord::CONCENTRATION, types) &&
            (SelectionRecord::DEPENDENT & types)) {
        for (int i = symbols->getInitFloatingSpeciesSize();
                i < symbols->getFloatingSpeciesSize(); ++i) {
            ids.push_back("init([" + this->getFloatingSpeciesId(i) + "])");
        }
    }

    if (checkExact(SelectionRecord::INITIAL | SelectionRecord::FLOATING |
            SelectionRecord::AMOUNT, types) &&
            (SelectionRecord::INDEPENDENT & types)) {
        for (int i = 0; i < symbols->getInitFloatingSpeciesSize(); ++i) {
            ids.push_back("init(" + this->getFloatingSpeciesId(i) + ")");
        }
    }

    if (checkExact(SelectionRecord::INITIAL | SelectionRecord::FLOATING |
            SelectionRecord::AMOUNT, types) &&
            (SelectionRecord::DEPENDENT & types)) {
        for (int i = symbols->getInitFloatingSpeciesSize();
                i < symbols->getFloatingSpeciesSize(); ++i) {
            ids.push_back("init(" + this->getFloatingSpeciesId(i) + ")");
        }
    }

    if (checkExact(SelectionRecord::FLOATING_AMOUNT_RATE, types)) {
        for (int i = 0; i < getNumIndFloatingSpecies(); ++i) {
            ids.push_back(this->getFloatingSpeciesId(i) + "'");
        }
    }
}

int LLVMExecutableModel::getSupportedIdTypes()
{
    return SelectionRecord::TIME |
        SelectionRecord::BOUNDARY_CONCENTRATION |
        SelectionRecord::FLOATING_CONCENTRATION |
        SelectionRecord::REACTION_RATE |
        SelectionRecord::FLOATING_AMOUNT_RATE |
        SelectionRecord::FLOATING_CONCENTRATION_RATE |
        SelectionRecord::COMPARTMENT |
        SelectionRecord::GLOBAL_PARAMETER |
        SelectionRecord::FLOATING_AMOUNT |
        SelectionRecord::BOUNDARY_AMOUNT |
        SelectionRecord::INITIAL_FLOATING_AMOUNT |
        SelectionRecord::INITIAL_FLOATING_CONCENTRATION |
        SelectionRecord::STOICHIOMETRY;
}

double LLVMExecutableModel::getValue(const std::string& id)
{
    SelectionRecord sel(id);

    int index = -1;
    double result = 0;

    if (sel.selectionType == SelectionRecord::UNKNOWN)
    {
        throw LLVMException("invalid selection string " + id);
    }

    // check to see that we have valid selection ids
    switch(sel.selectionType)
    {
    case SelectionRecord::TIME:
        result = getTime();
        break;
    case SelectionRecord::UNKNOWN_ELEMENT:
        // check for sbml element types

        if ((index = getFloatingSpeciesIndex(sel.p1)) >= 0)
        {
            getFloatingSpeciesAmounts(1, &index, &result);
            break;
        }
        else if ((index = getBoundarySpeciesIndex(sel.p1)) >= 0)
        {
            getBoundarySpeciesAmounts(1, &index, &result);
            break;
        }
        else if ((index = getCompartmentIndex(sel.p1)) >= 0)
        {
            getCompartmentVolumes(1, &index, &result);
            break;
        }
        else if ((index = getGlobalParameterIndex(sel.p1)) >= 0)
        {
            getGlobalParameterValues(1, &index, &result);
            break;
        }
        else if ((index = getReactionIndex(sel.p1)) >= 0)
        {
            getReactionRates(1, &index, &result);
            break;
        }
        else
        {
            throw LLVMException("No sbml element exists for symbol '" + id + "'");
            break;
        }
    case SelectionRecord::UNKNOWN_CONCENTRATION:
        if ((index = getFloatingSpeciesIndex(sel.p1)) >= 0)
        {
            getFloatingSpeciesConcentrations(1, &index, &result);
            break;
        }
        else if ((index = getBoundarySpeciesIndex(sel.p1)) >= 0)
        {
            getBoundarySpeciesConcentrations(1, &index, &result);
            break;
        }
        else
        {
            string msg = "No sbml element exists for concentration selection '" + id + "'";
            Log(Logger::LOG_ERROR) << msg;
            throw LLVMException(msg);
            break;
        }
    case SelectionRecord::FLOATING_AMOUNT_RATE:
        if ((index = getFloatingSpeciesIndex(sel.p1)) >= 0)
        {
            getFloatingSpeciesAmountRates(1, &index, &result);
            break;
        }
        else
        {
            throw LLVMException("Invalid id '" + id + "' for floating amount rate");
            break;
        }

    case SelectionRecord::INITIAL_FLOATING_AMOUNT:
        if ((index = getFloatingSpeciesIndex(sel.p1)) >= 0)
        {
            getFloatingSpeciesInitAmounts(1, &index, &result);
            break;
        }
        else if ((index = getCompartmentIndex(sel.p1)) >= 0)
        {
            getCompartmentInitVolumes(1, &index, &result);
            break;
        }
        else
        {
            throw LLVMException("Invalid id '" + id + "' for floating amount rate");
            break;
        }
    case SelectionRecord::INITIAL_FLOATING_CONCENTRATION:
        if ((index = getFloatingSpeciesIndex(sel.p1)) >= 0)
        {
            getFloatingSpeciesInitConcentrations(1, &index, &result);
            break;
        }
        else
        {
            throw LLVMException("Invalid id '" + id + "' for floating species");
            break;
        }


    default:
        Log(Logger::LOG_ERROR) << "A new SelectionRecord should not have this value: "
        << sel.to_repr();
        throw LLVMException("Invalid selection '" + id + "' for setting value");
        break;
    }

    return result;
}

void LLVMExecutableModel::setValue(const std::string& id, double value)
{
    SelectionRecord sel(id);

    int index = -1;

    if (sel.selectionType == SelectionRecord::UNKNOWN)
    {
        throw LLVMException("invalid selection string " + id);
    }

    // check to see that we have valid selection ids
    switch(sel.selectionType)
    {
    case SelectionRecord::TIME:
        setTime(value);
        break;
    case SelectionRecord::UNKNOWN_ELEMENT:
        // check for sbml element types

        if ((index = getFloatingSpeciesIndex(sel.p1)) >= 0)
        {
            setFloatingSpeciesAmounts(1, &index, &value);
            break;
        }
        else if ((index = getCompartmentIndex(sel.p1)) >= 0)
        {
            setCompartmentVolumes(1, &index, &value);
            break;
        }
        else if ((index = getGlobalParameterIndex(sel.p1)) >= 0)
        {
            setGlobalParameterValues(1, &index, &value);
            break;
        }
        else if ((index = getBoundarySpeciesIndex(sel.p1)) >= 0)
        {
            setBoundarySpeciesAmounts(1, &index, &value);
            break;
        }
        else if(this->getReactionIndex(sel.p1) >= 0)
        {
            throw LLVMException("The sbml id '" + id +
                    "' is for a reaction, reaction rates can not be set externally");
            break;
        }
        else
        {
            throw LLVMException("Invalid or non-existant sbml id  '" + id + "' for set value");
            break;
        }
    case SelectionRecord::UNKNOWN_CONCENTRATION:
        if ((index = getFloatingSpeciesIndex(sel.p1)) >= 0)
        {
            setFloatingSpeciesConcentrations(1, &index, &value);
            break;
        }
        else if ((index = getBoundarySpeciesIndex(sel.p1)) >= 0)
        {
            setBoundarySpeciesConcentrations(1, &index, &value);
            break;
        }
        else
        {
            string msg = "No sbml element exists for concentration selection '" + id + "'";
            Log(Logger::LOG_ERROR) << msg;
            throw LLVMException(msg);
            break;
        }

    case SelectionRecord::INITIAL_FLOATING_AMOUNT:
        if ((index = getFloatingSpeciesIndex(sel.p1)) >= 0)
        {
            setFloatingSpeciesInitAmounts(1, &index, &value);
            break;
        }
        else if ((index = getCompartmentIndex(sel.p1)) >= 0)
        {
            setCompartmentInitVolumes(1, &index, &value);
            break;
        }
        else
        {
            throw LLVMException("Invalid id '" + id + "' for floating amount rate");
            break;
        }
    case SelectionRecord::INITIAL_FLOATING_CONCENTRATION:
        if ((index = getFloatingSpeciesIndex(sel.p1)) >= 0)
        {
            setFloatingSpeciesInitConcentrations(1, &index, &value);
            break;
        }
        else
        {
            throw LLVMException("Invalid id '" + id + "' for floating species");
            break;
        }

    default:

        throw LLVMException("Invalid selection '" + sel.to_string() + "' for setting value");
        break;
    }
}

int LLVMExecutableModel::getFloatingSpeciesConcentrationRates(int len,
        const int* indx, double* values)
{
    assert(0);
    return 0;
}

int LLVMExecutableModel::setBoundarySpeciesAmounts(int len, const int* indx,
        const double* values)
{
    int result = -1;
    if (setBoundarySpeciesAmountPtr)
    {
        result = setValues(setBoundarySpeciesAmountPtr,
                &LLVMExecutableModel::getBoundarySpeciesId, len, indx, values);
    }
    return result;
}

LLVMExecutableModel* LLVMExecutableModel::dummy()
{
    return new LLVMExecutableModel();
}

void LLVMExecutableModel::evalReactionRates()
{
    evalReactionRatesPtr(modelData);
}

int LLVMExecutableModel::getNumRules()
{
    return 0;
}

int LLVMExecutableModel::getFloatingSpeciesAmounts(int len, const int* indx,
        double* values)
{
    return getValues(getFloatingSpeciesAmountPtr, len, indx, values);
}

int LLVMExecutableModel::setFloatingSpeciesConcentrations(int len,
        const int* indx, const double* values)
{
    int result = -1;
    if (setFloatingSpeciesConcentrationPtr)
    {
        result = setValues(setFloatingSpeciesConcentrationPtr,
                &LLVMExecutableModel::getFloatingSpeciesId, len, indx, values);
    }
    return result;
}

int LLVMExecutableModel::getBoundarySpeciesAmounts(int len, const int* indx,
        double* values)
{
    return getValues(getBoundarySpeciesAmountPtr, len, indx, values);
}

int LLVMExecutableModel::getBoundarySpeciesConcentrations(int len,
        const int* indx, double* values)
{
    return getValues(getBoundarySpeciesConcentrationPtr, len, indx, values);
}

int LLVMExecutableModel::setBoundarySpeciesConcentrations(int len,
        const int* indx, const double* values)
{
    int result = -1;
    if (setBoundarySpeciesConcentrationPtr)
    {
        result = setValues(setBoundarySpeciesConcentrationPtr,
                &LLVMExecutableModel::getBoundarySpeciesId, len, indx, values);
    }
    return result;
}

int LLVMExecutableModel::getGlobalParameterValues(int len, const int* indx,
        double* values)
{
    return getValues(getGlobalParameterPtr, len, indx, values);
}

int LLVMExecutableModel::setGlobalParameterValues(int len, const int* indx,
        const double* values)
{
    int result = -1;
    if (setGlobalParameterPtr)
    {
        result = setValues(setGlobalParameterPtr,
                &LLVMExecutableModel::getGlobalParameterId, len, indx, values);
    }
    return result;
}

int LLVMExecutableModel::getCompartmentVolumes(int len, const int* indx,
        double* values)
{
    return getValues(getCompartmentVolumePtr, len, indx, values);
}

int LLVMExecutableModel::getReactionRates(int len, const int* indx,
        double* values)
{
    // the reaction rates are a function of the model state, so someone
    // could have changed some parameter, so we need to re-evaluate.
    evalReactionRates();

    for (int i = 0; i < len; ++i)
    {
        int j = indx ? indx[i] : i;
        if (j < modelData->numReactions)
        {
            values[i] = modelData->reactionRatesAlias[j];
        }
        else
        {
            throw LLVMException("index out of range");
        }
    }
    return len;
}

int LLVMExecutableModel::getNumConservedMoieties()
{
    return 0;
}

int LLVMExecutableModel::getConservedMoietyIndex(const string& name)
{
    return -1;
}

string LLVMExecutableModel::getConservedMoietyId(int index)
{
    return "";
}

int LLVMExecutableModel::getConservedMoietyValues(int len, const int* indx,
        double* values)
{
    return 0;
}

int LLVMExecutableModel::setConservedMoietyValues(int len, const int* indx,
        const double* values)
{
    return 0;
}

int LLVMExecutableModel::getFloatingSpeciesAmountRates(int len,
        int const *indx, double *values)
{
    uint dydtSize = modelData->numRateRules + modelData->numIndFloatingSpecies;

    double* dydt = (double*)calloc(dydtSize, sizeof(double));

    // state vector is packed such that first numRateRules are the rate rule rates,
    // and the last numIndFloatingSpecies are the number of independent species.
    this->getStateVectorRate(this->getTime(), 0, dydt);

    double* amountRates = dydt + modelData->numRateRules;

    for (uint i = 0; i < len; ++i)
    {
        uint j = indx ? indx[i] : i;
        assert(j < modelData->numIndFloatingSpecies && "index out of range");
        values[i] = amountRates[j];
    }

    free(dydt);
    return len;
}


int LLVMExecutableModel::setFloatingSpeciesAmounts(int len, int const *indx,
        const double *values)
{
    int result = -1;
    if (setFloatingSpeciesAmountPtr)
    {
        result = setValues(setFloatingSpeciesAmountPtr,
                &LLVMExecutableModel::getFloatingSpeciesId, len, indx, values);
    }
    return result;
}


int LLVMExecutableModel::setCompartmentVolumes(int len, const int* indx,
        const double* values)
{
    int result = -1;
    if (setCompartmentVolumePtr)
    {
        result = setValues(setCompartmentVolumePtr,
                &LLVMExecutableModel::getCompartmentId, len, indx, values);
    }
    return result;
}



double LLVMExecutableModel::getStoichiometry(int speciesIndex, int reactionIndex)
{
    double result = csr_matrix_get_nz(modelData->stoichiometry, speciesIndex, reactionIndex);
    return isnan(result) ? 0 : result;
}

int LLVMExecutableModel::getStoichiometryMatrix(int* pRows, int* pCols,
        double** pData)
{
    // m rows x n cols
    // offset = row*NUMCOLS + column
    const unsigned m = modelData->stoichiometry->m;
    const unsigned n = modelData->stoichiometry->n;
    unsigned *rowptr = modelData->stoichiometry->rowptr;
    unsigned *colidx = modelData->stoichiometry->colidx;
    double *values = modelData->stoichiometry->values;

    double *data = (double*)calloc(m*n, sizeof(double));

    for (unsigned i = 0; i < m; i++)
    {
        double yi = 0.0;
        for (unsigned k = rowptr[i]; k < rowptr[i + 1]; k++)
        {
            int col = colidx[k];
            data[i*n + col] = values[k];

            //yi = yi + alpha * values[k] * x[colidx[k]];
        }
    }

    *pRows = m;
    *pCols = n;
    *pData = data;

    return m*n;
}


/******************************* Events Section *******************************/
#if (1) /**********************************************************************/
/******************************************************************************/

int LLVMExecutableModel::getNumEvents()
{
    return modelData->numEvents;
}

int LLVMExecutableModel::getEventTriggers(int len, const int *indx, unsigned char *values)
{
    if (len <= 0)
    {
        return modelData->numEvents;
    }
    else
    {
        for (int i = 0; i < len; ++i)
        {
            int j = indx ? indx[i] : i;
            if (j < modelData->numEvents)
            {
                values[j] = getEventTrigger(j);
            }
            else
            {
                throw LLVMException("index out of range");
            }
        }
        return len;
    }
}

void LLVMExecutableModel::evalEvents(double timeEnd,
        const unsigned char* previousEventStatus, const double *initialState,
        double* finalState)
{
    modelData->time = timeEnd;
    setStateVector(initialState);

    vector<unsigned char> prevEventState(previousEventStatus,
            previousEventStatus + modelData->numEvents);

    vector<unsigned char> currEventStatus(modelData->numEvents);

    unsigned char *p1 = &prevEventState[0];
    unsigned char *p2 = &currEventStatus[0];

    pendingEvents.make_heap();
    pendingEvents.eraseExpiredEvents();

    while(applyEvents(p1, p2))
    {
        std::swap(p1, p2);
    }

    getStateVector(finalState);
}

/**
 * Each applied event application typically results in a state change.
 *
 * At each state change, we need to re-scan the events and see
 * if any new events become triggered or events expire.
 */
int LLVMExecutableModel::applyPendingEvents(const double *stateVector,
        double timeEnd, double tout)
{
    int assignedEvents = 0;
    modelData->time = timeEnd;
    setStateVector(stateVector);

    vector<unsigned char> prevEventState(modelData->numEvents);
    vector<unsigned char> currEventStatus(modelData->numEvents);

    getEventTriggers(prevEventState.size(), 0, &prevEventState[0]);

    unsigned char *p1 = &prevEventState[0];
    unsigned char *p2 = &currEventStatus[0];

    pendingEvents.make_heap();
    pendingEvents.eraseExpiredEvents();

    while (applyEvents(p1, p2))
    {
        assignedEvents++;
        std::swap(p1, p2);
    }

    return assignedEvents;
}

void  LLVMExecutableModel::evalEventRoots(double time, const double* y, double* gdot)
{
    modelData->time = time;

    double *savedRateRules = modelData->rateRuleValuesAlias;
    double *savedFloatingSpeciesAmounts = modelData->floatingSpeciesAmountsAlias;

    if (y)
    {
        //memcpy(modelData->rateRuleValues, y,
        //        modelData->numRateRules * sizeof(double));

        //memcpy(modelData->floatingSpeciesAmounts, y + modelData->numRateRules,
        //        modelData->numIndFloatingSpecies * sizeof(double));

        modelData->rateRuleValuesAlias = const_cast<double*>(y);
        modelData->floatingSpeciesAmountsAlias = const_cast<double*>(y + modelData->numRateRules);

        evalVolatileStoichPtr(modelData);
    }

    for (uint i = 0; i < modelData->numEvents; ++i)
    {
        unsigned char triggered = getEventTriggerPtr(modelData, i);

        gdot[i] = triggered ? 1.0 : -1.0;
    }

    modelData->rateRuleValuesAlias = savedRateRules;
    modelData->floatingSpeciesAmountsAlias = savedFloatingSpeciesAmounts;

    return;
}

double LLVMExecutableModel::getNextPendingEventTime(bool pop)
{
    return pendingEvents.getNextPendingEventTime();
}

int LLVMExecutableModel::getPendingEventSize()
{
    return pendingEvents.size();
}

void LLVMExecutableModel::resetEvents()
{
}

bool LLVMExecutableModel::applyEvents(unsigned char* prevEventState,
        unsigned char* currEventState)
{
    for (uint i = 0; i < modelData->numEvents; ++i)
    {
        bool c = getEventTrigger(i);
        currEventState[i] = c;

        Log(Logger::LOG_DEBUG) << "event " << i << ", previous state: " <<
                (bool)prevEventState[i] << ", current state: " << (bool)c;

        // transition from non-triggered to triggered
        if (c && !prevEventState[i])
        {
            pendingEvents.push(rrllvm::Event(*this, i));
        }
    }

    // fire the highest priority event, this causes state change
    // return true if we incured a state change
    return pendingEvents.applyEvent();
}

bool LLVMExecutableModel::getEventTieBreak(uint eventA, uint eventB)
{
    /*
    C_ASSERT(sizeof(TieBreakKey) == 8 && sizeof(uint) == 4);

    bool result;
    TieBreakKey keyA = eventA;
    TieBreakKey keyB = eventB;
    TieBreakKey key = keyA << 32 | keyB;
    TieBreakMap::iterator i = tieBreakMap.find(key);
    if (i != tieBreakMap.end())
    {
        result = !i->second;
        i->second = result;
    }
    else
    {
        result = true;
        tieBreakMap.insert(std::pair<TieBreakKey,bool>(key, result));
    }
    */

    bool result = (std::rand() % 2) == 0;



    Log(Logger::LOG_DEBUG) << "tie break, a: " << eventA << ", b: " <<
            eventB << ", result: " << result;

    return result;
}


/******************************* Events Section *******************************/
#endif /***********************************************************************/
/******************************************************************************/

/******************************* Initial Conditions Section *******************/
#if (1) /**********************************************************************/
/******************************************************************************/


int LLVMExecutableModel::setFloatingSpeciesInitConcentrations(int len,
        const int* indx, const double* values)
{
    int result = -1;
    if (setFloatingSpeciesInitConcentrationsPtr)
    {
        result = setValues(setFloatingSpeciesInitConcentrationsPtr,
                &LLVMExecutableModel::getFloatingSpeciesId, len, indx, values);
    }
    return result;
}

int LLVMExecutableModel::getFloatingSpeciesInitConcentrations(int len,
        const int* indx, double* values)
{
    int result = -1;
    if (getFloatingSpeciesInitConcentrationsPtr)
    {
        result = getValues(getFloatingSpeciesInitConcentrationsPtr, len, indx, values);
    }
    return result;
}

int LLVMExecutableModel::setFloatingSpeciesInitAmounts(int len, int const *indx,
            double const *values)
{
    int result = -1;
    if (setFloatingSpeciesInitAmountsPtr)
    {
        result = setValues(setFloatingSpeciesInitAmountsPtr,
                &LLVMExecutableModel::getFloatingSpeciesId, len, indx, values);
    }
    return result;
}

int LLVMExecutableModel::getFloatingSpeciesInitAmounts(int len, int const *indx,
                double *values)
{
    int result = -1;
    if (getFloatingSpeciesInitAmountsPtr)
    {
        result = getValues(getFloatingSpeciesInitAmountsPtr, len, indx, values);
    }
    return result;
}

int LLVMExecutableModel::setCompartmentInitVolumes(int len, const int *indx,
            double const *values)
{
    int result = -1;
    if (setCompartmentInitVolumesPtr)
    {
        result = setValues(setCompartmentInitVolumesPtr,
                &LLVMExecutableModel::getCompartmentId, len, indx, values);
    }
    return result;
}

int LLVMExecutableModel::getCompartmentInitVolumes(int len, const int *indx,
                double *values)
{
    int result = -1;
    if (getCompartmentInitVolumesPtr)
    {
        result = getValues(getCompartmentInitVolumesPtr, len, indx, values);
    }
    return result;
}

/******************************* End Initial Conditions Section ***************/
#endif /***********************************************************************/
/******************************************************************************/


} /* namespace rr */
