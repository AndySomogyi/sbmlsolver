/*
 * LLVMExecutableModel.cpp
 *
 * Author: Andy Somogyi,
 *     email decode: V1 = "."; V2 = "@"; V3 = V1;
 *     andy V1 somogyi V2 gmail V3 com
 */

#include "LLVMExecutableModel.h"
#include "ModelResources.h"
#include "LLVMIncludes.h"
#include "rrSparse.h"
#include "rrLogger.h"
#include "rrException.h"
#include "LLVMException.h"
#include <iomanip>
#include <cstdlib>

using rr::Logger;
using rr::getLogger;
using rr::LoggingBuffer;

#ifdef _WIN32
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

static void dump_array(std::ostream &os, int n, const double *p)
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

namespace rrllvm
{

static int getValues(LLVMModelData* modelData, double (*funcPtr)(LLVMModelData*, int),
        int len, int const *indx, double *values)
{
    for (int i = 0; i < len; ++i)
    {
        int j = indx ? indx[i] : i;
        values[i] = funcPtr(modelData, j);
    }
    return len;
}

static int setValues(LLVMModelData* modelData, bool (*funcPtr)(LLVMModelData*, int, double),
        int len, int const *indx, const double *values)
{
    int result = 0;
    for (int i = 0; i < len; ++i)
    {
        int j = indx ? indx[i] : i;
        result += funcPtr(modelData, j, values[i]);
    }
    return result;
}

LLVMExecutableModel::LLVMExecutableModel() :
    symbols(0),
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
    setGlobalParameterPtr(0)
{
    // zero out the struct, the generator will fill it out.
    LLVMModelData::init(modelData);

    modelData.time = -1.0;

    std::srand(std::time(0));
}

LLVMExecutableModel::LLVMExecutableModel(
    const std::tr1::shared_ptr<ModelResources>& rc) :
    resources(rc),
    symbols(rc->symbols),
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
    setGlobalParameterPtr(rc->setGlobalParameterPtr)
{
    // zero out the struct,
    LLVMModelData::init(modelData);
    modelData.time = -1.0; // time is initially before simulation starts

    std::srand(std::time(0));

    symbols->initAllocModelDataBuffers(modelData);

    eventAssignTimes.resize(modelData.numEvents);
}

LLVMExecutableModel::~LLVMExecutableModel()
{
    // smart ptr takes care of freeing resources
    LLVMModelData::freeBuffers(modelData);

    Log(Logger::PRIO_DEBUG) << __FUNC__;
}

string LLVMExecutableModel::getModelName()
{
    return symbols->getModelName();
}

void LLVMExecutableModel::setTime(double time)
{
    modelData.time = time;
}

double LLVMExecutableModel::getTime()
{
    return modelData.time;
}

int LLVMExecutableModel::getNumIndependentSpecies()
{
    return modelData.numIndependentSpecies;
}

int LLVMExecutableModel::getNumDependentSpecies()
{
    return modelData.numDependentSpecies;
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
    return modelData.numReactions;
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

double LLVMExecutableModel::getFloatingSpeciesConcentration(int index)
{
    /*
    if (index >= 0 && index < modelData.numFloatingSpecies)
    {
        int compIndex = modelData.floatingSpeciesCompartments[index];
        return modelData.floatingSpeciesAmounts[index] /
                modelData.compartmentVolumes[compIndex];
    }
    else
    {
        Log(Logger::PRIO_ERROR) << "index " << index << "out of range";
        throw Exception(string(__FUNC__) + string(": index out of range"));
    }
    */
    return 0;
}

int LLVMExecutableModel::getFloatingSpeciesConcentrations(int len, int const *indx,
        double *values)
{
    return getValues(&modelData, getFloatingSpeciesConcentrationPtr, len, indx, values);
}

void LLVMExecutableModel::getRateRuleValues(double *rateRuleValues)
{
    memcpy(rateRuleValues, modelData.rateRuleValues, modelData.numRateRules * sizeof(double));
}

void LLVMExecutableModel::setRateRuleValues(const double *rateRuleValues)
{
    memcpy(modelData.rateRuleValues, rateRuleValues, modelData.numRateRules * sizeof(double));
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

void LLVMExecutableModel::evalModel(double time, const double *y, double *dydt)
{
    modelData.time = time;

    if (y && dydt)
    {
        // save and assign state vector
        double *savedRateRules = modelData.rateRuleValues;
        double *savedFloatingSpeciesAmounts = modelData.floatingSpeciesAmounts;

        modelData.rateRuleValues = const_cast<double*>(y);
        modelData.floatingSpeciesAmounts = const_cast<double*>(y + modelData.numRateRules);
        evalVolatileStoichPtr(&modelData);

        double conversionFactor = evalReactionRatesPtr(&modelData);

        // floatingSpeciesAmountRates only valid for the following two
        // functions, this will move to a parameter shortly...

        modelData.floatingSpeciesAmountRates = dydt + modelData.numRateRules;

        csr_matrix_dgemv(conversionFactor, modelData.stoichiometry,
                modelData.reactionRates, 0.0, modelData.floatingSpeciesAmountRates);

        evalConversionFactorPtr(&modelData);

        modelData.floatingSpeciesAmountRates = 0;

        // this will also move to a parameter for the evalRateRules func...
        modelData.rateRuleRates = dydt;
        evalRateRuleRatesPtr(&modelData);
        modelData.rateRuleRates = 0;

        // restore original pointers for state vector
        modelData.rateRuleValues = savedRateRules;
        modelData.floatingSpeciesAmounts = savedFloatingSpeciesAmounts;
    }
    else if (y && !dydt)
    {
        setStateVector(y);
    }

    /*
    if (Logger::PRIO_TRACE <= rr::Logger::GetLogLevel()) {

        LoggingBuffer log(Logger::PRIO_TRACE, __FILE__, __LINE__);

        log.stream() << __FUNC__ << endl;
        log.stream() << "y: ";
        if (y) {
            dump_array(log.stream(), modelData.numRateRules + modelData.numFloatingSpecies, y);
        } else {
            log.stream() << "null";
        }
        log.stream() << endl << "dydt: ";
        if (dydt) {
            dump_array(log.stream(), modelData.numRateRules + modelData.numFloatingSpecies, dydt);
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
    return "";
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
    evalInitialConditionsPtr(&modelData);
}

void LLVMExecutableModel::reset()
{
    // eval the initial conditions and rates
    setTime(0.0);
    evalInitialConditions();
    evalReactionRates();

    // this sets up the event system to pull the initial value
    // before the simulation starts.
    setTime(-1.0);

    Log(Logger::PRIO_TRACE) << __FUNC__ << this;
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
        Log(Logger::PRIO_TRACE) << __FUNC__ << ", stateVector: null, returning "
                << modelData.numRateRules + modelData.numIndependentSpecies;
        return modelData.numRateRules + modelData.numIndependentSpecies;
    }

    getRateRuleValues(stateVector);

    memcpy(stateVector + modelData.numRateRules,
            modelData.floatingSpeciesAmounts,
            modelData.numIndependentSpecies * sizeof(double));


    if (Logger::PRIO_TRACE <= rr::Logger::getLevel()) {

        LoggingBuffer log(Logger::PRIO_TRACE, __FILE__, __LINE__);

        log.stream() << endl << __FUNC__ <<  ", Model: " << endl << this;

        log.stream() << __FUNC__ << ",  out stateVector: ";
        if (stateVector) {
            dump_array(log.stream(), modelData.numRateRules + modelData.numIndependentSpecies, stateVector);
        } else {
            log.stream() << "null";
        }
    }

    return modelData.numRateRules + modelData.numIndependentSpecies;
}

int LLVMExecutableModel::setStateVector(const double* stateVector)
{
    if (stateVector == 0)
    {
        return -1;
    }

    memcpy(modelData.rateRuleValues, stateVector, modelData.numRateRules * sizeof(double));

    memcpy(modelData.floatingSpeciesAmounts,
            stateVector + modelData.numRateRules,
            modelData.numIndependentSpecies * sizeof(double));

    evalVolatileStoichPtr(&modelData);

    /*
    if (Logger::PRIO_TRACE <= rr::Logger::GetLogLevel()) {

        LoggingBuffer log(Logger::PRIO_TRACE, __FILE__, __LINE__);

        log.stream() << endl << __FUNC__ <<  ", Model: " << endl << this;

        log.stream() << __FUNC__ << ",  stateVector: ";
        if (stateVector) {
            dump_array(log.stream(), modelData.numRateRules + modelData.numIndependentSpecies, stateVector);
        } else {
            log.stream() << "null";
        }
    }
    */

    return modelData.numRateRules + modelData.numIndependentSpecies;
}

void LLVMExecutableModel::print(std::ostream &stream)
{
    stream << "LLVMExecutableModel" << endl;
    stream << modelData;
}

LLVMExecutableModel* LLVMExecutableModel::dummy()
{
    return new LLVMExecutableModel();
}

void LLVMExecutableModel::evalReactionRates()
{
    evalReactionRatesPtr(&modelData);
}

int LLVMExecutableModel::getNumRules()
{
    return 0;
}

int LLVMExecutableModel::getFloatingSpeciesAmounts(int len, const int* indx,
        double* values)
{
    return getValues(&modelData, getFloatingSpeciesAmountPtr, len, indx, values);
}

int LLVMExecutableModel::setFloatingSpeciesConcentrations(int len,
        const int* indx, const double* values)
{
    int result = -1;
    if (setFloatingSpeciesConcentrationPtr)
    {
        result = setValues(&modelData, setFloatingSpeciesConcentrationPtr, len, indx, values);
    }
    return result;
}

int LLVMExecutableModel::getBoundarySpeciesAmounts(int len, const int* indx,
        double* values)
{
    return getValues(&modelData, getBoundarySpeciesAmountPtr, len, indx, values);
}

int LLVMExecutableModel::getBoundarySpeciesConcentrations(int len,
        const int* indx, double* values)
{
    return getValues(&modelData, getBoundarySpeciesConcentrationPtr, len, indx, values);
}

int LLVMExecutableModel::setBoundarySpeciesConcentrations(int len,
        const int* indx, const double* values)
{
    int result = -1;
    if (setBoundarySpeciesConcentrationPtr)
    {
        result = setValues(&modelData, setBoundarySpeciesConcentrationPtr, len, indx, values);
    }
    return result;
}

int LLVMExecutableModel::getGlobalParameterValues(int len, const int* indx,
        double* values)
{
    return getValues(&modelData, getGlobalParameterPtr, len, indx, values);
}

int LLVMExecutableModel::setGlobalParameterValues(int len, const int* indx,
        const double* values)
{
    int result = -1;
    if (setGlobalParameterPtr)
    {
        result = setValues(&modelData, setGlobalParameterPtr, len, indx, values);
    }
    return result;
}

int LLVMExecutableModel::getCompartmentVolumes(int len, const int* indx,
        double* values)
{
    return getValues(&modelData, getCompartmentVolumePtr, len, indx, values);
}

int LLVMExecutableModel::getReactionRates(int len, const int* indx,
        double* values)
{
    for (int i = 0; i < len; ++i)
    {
        int j = indx ? indx[i] : i;
        if (j < modelData.numReactions)
        {
            values[i] = modelData.reactionRates[j];
        }
        else
        {
            throw LLVMException("index out of range");
        }
    }
    return len;
}

int LLVMExecutableModel::getNumConservedSums()
{
    return 0;
}

int LLVMExecutableModel::getConservedSumIndex(const string& name)
{
    return 0;
}

string LLVMExecutableModel::getConservedSumId(int index)
{
    return "";
}

int LLVMExecutableModel::getConservedSums(int len, const int* indx,
        double* values)
{
    return 0;
}

int LLVMExecutableModel::setConservedSums(int len, const int* indx,
        const double* values)
{
    return 0;
}

int LLVMExecutableModel::getFloatingSpeciesAmountRates(int len,
        int const *indx, double *values)
{
    /*
    for (int i = 0; i < len; ++i)
    {
        int j = indx ? indx[i] : i;
        if (j < modelData.numFloatingSpecies)
        {
            values[i] = modelData.floatingSpeciesAmountRates[j];
        }
        else
        {
            throw LLVMException("index out of range");
        }
    }
    return len;
    */
    return 0;
}


int LLVMExecutableModel::setFloatingSpeciesAmounts(int len, int const *indx,
        const double *values)
{
    int result = -1;
    if (setFloatingSpeciesAmountPtr)
    {
        result = setValues(&modelData, setFloatingSpeciesAmountPtr, len, indx, values);
    }
    return result;
}


int LLVMExecutableModel::setCompartmentVolumes(int len, const int* indx,
        const double* values)
{
    int result = -1;
    if (setCompartmentVolumePtr)
    {
        result = setValues(&modelData, setCompartmentVolumePtr, len, indx, values);
    }
    return result;
}

int LLVMExecutableModel::setFloatingSpeciesInitConcentrations(int len,
        const int* indx, const double* values)
{
    return 0;
}

int LLVMExecutableModel::getFloatingSpeciesInitConcentrations(int len,
        const int* indx, double* values)
{
    return 0;
}

double LLVMExecutableModel::getStoichiometry(int speciesIndex, int reactionIndex)
{
    double result = csr_matrix_get_nz(modelData.stoichiometry, speciesIndex, reactionIndex);
    return isnan(result) ? 0 : result;
}

int LLVMExecutableModel::getStoichiometryMatrix(int* pRows, int* pCols,
        double** pData)
{
    // m rows x n cols
    // offset = row*NUMCOLS + column
    const unsigned m = modelData.stoichiometry->m;
    const unsigned n = modelData.stoichiometry->n;
    unsigned *rowptr = modelData.stoichiometry->rowptr;
    unsigned *colidx = modelData.stoichiometry->colidx;
    double *values = modelData.stoichiometry->values;

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
    return modelData.numEvents;
}

int LLVMExecutableModel::getEventTriggers(int len, const int *indx, unsigned char *values)
{
    if (len <= 0)
    {
        return modelData.numEvents;
    }
    else
    {
        for (int i = 0; i < len; ++i)
        {
            int j = indx ? indx[i] : i;
            if (j < modelData.numEvents)
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
    modelData.time = timeEnd;
    setStateVector(initialState);

    vector<unsigned char> prevEventState(previousEventStatus,
            previousEventStatus + modelData.numEvents);

    vector<unsigned char> currEventStatus(modelData.numEvents);

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
    modelData.time = timeEnd;
    setStateVector(stateVector);

    vector<unsigned char> prevEventState(modelData.numEvents);
    vector<unsigned char> currEventStatus(modelData.numEvents);

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
    modelData.time = time;

    double *savedRateRules = modelData.rateRuleValues;
    double *savedFloatingSpeciesAmounts = modelData.floatingSpeciesAmounts;

    if (y)
    {
        //memcpy(modelData.rateRuleValues, y,
        //        modelData.numRateRules * sizeof(double));

        //memcpy(modelData.floatingSpeciesAmounts, y + modelData.numRateRules,
        //        modelData.numIndependentSpecies * sizeof(double));

        modelData.rateRuleValues = const_cast<double*>(y);
        modelData.floatingSpeciesAmounts = const_cast<double*>(y + modelData.numRateRules);

        evalVolatileStoichPtr(&modelData);
    }

    for (uint i = 0; i < modelData.numEvents; ++i)
    {
        unsigned char triggered = getEventTriggerPtr(&modelData, i);

        gdot[i] = triggered ? 1.0 : -1.0;
    }

    modelData.rateRuleValues = savedRateRules;
    modelData.floatingSpeciesAmounts = savedFloatingSpeciesAmounts;

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
    for (uint i = 0; i < modelData.numEvents; ++i)
    {
        bool c = getEventTrigger(i);
        currEventState[i] = c;

        Log(Logger::PRIO_DEBUG) << "event " << i << ", previous state: " <<
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



    Log(Logger::PRIO_DEBUG) << "tie break, a: " << eventA << ", b: " <<
            eventB << ", result: " << result;

    return result;
}


/******************************* Events Section *******************************/
#endif /***********************************************************************/
/******************************************************************************/


} /* namespace rr */
