/*
 * rrLLVMExecutableModel.cpp
 *
 * Author: Andy Somogyi,
 *     email decode: V1 = "."; V2 = "@"; V3 = V1;
 *     andy V1 somogyi V2 gmail V3 com
 */

#include "rrLLVMExecutableModel.h"
#include "rrLLVMIncludes.h"
#include "rrSparse.h"
#include "rrLogger.h"
#include "rrException.h"

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

namespace rr
{

LLVMExecutableModel::LLVMExecutableModel() :
    symbols(0),
    context(0),
    executionEngine(0),
    errStr(0),
    evalInitialConditionsPtr(0),
    evalReactionRatesPtr(0),
    stackDepth(0)
{
    // zero out the struct, the generator will fill it out.
    initModelData(modelData);
    initModelData(modelDataCopy);
}

LLVMExecutableModel::~LLVMExecutableModel()
{
    Log(Logger::PRIO_TRACE) << __FUNC__ << "ModelName: " << modelData.modelName;

    if (errStr->size() > 0)
    {
        Log(Logger::PRIO_WARNING) << "Non-empty LLVM ExecutionEngine error string: " << *errStr;
    }

    freeModelDataBuffers(modelData);
    delete symbols;
    // the exe engine owns all the functions
    delete executionEngine;
    delete context;
    delete errStr;
}

string LLVMExecutableModel::getModelName()
{
    return modelData.modelName;
}

void LLVMExecutableModel::setTime(double time)
{
    modelData.time = time;
}

double LLVMExecutableModel::getTime()
{
    return modelData.time;
}

ModelData& LLVMExecutableModel::getModelData()
{
    return modelData;
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
    return modelData.numFloatingSpecies;
}

int LLVMExecutableModel::getNumBoundarySpecies()
{
    return modelData.numBoundarySpecies;
}

int LLVMExecutableModel::getNumGlobalParameters()
{
    return modelData.numGlobalParameters;
}

int LLVMExecutableModel::getNumCompartments()
{
    return modelData.numCompartments;
}

int LLVMExecutableModel::getNumReactions()
{
    return modelData.numReactions;
}


int LLVMExecutableModel::getNumEvents()
{
    return modelData.numEvents;
}

void LLVMExecutableModel::computeEventPriorites()
{
}

void LLVMExecutableModel::setConcentration(int index, double value)
{
}

int LLVMExecutableModel::getNumLocalParameters(int reactionId)
{
    return 0;
}

void LLVMExecutableModel::evalInitialAssignments()
{
}

void LLVMExecutableModel::convertToAmounts()
{
}

void LLVMExecutableModel::computeConservedTotals()
{
}

double LLVMExecutableModel::getFloatingSpeciesConcentration(int index)
{
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
}

void LLVMExecutableModel::getRateRuleValues(double *rateRuleValues)
{
}

void LLVMExecutableModel::setRateRuleValues(const double * rates)
{
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
    if (y)
    {
        memcpy(modelData.floatingSpeciesAmounts, y,
                modelData.numFloatingSpecies * sizeof(double));
    }

    evalReactionRates();

    csr_matrix_dgemv(modelData.stoichiometry, modelData.reactionRates,
                     modelData.floatingSpeciesAmountRates);

    if (dydt)
    {
        memcpy(dydt, modelData.rateRules, modelData.numRateRules * sizeof(double));

        memcpy(dydt + modelData.numRateRules, modelData.floatingSpeciesAmountRates,
                modelData.numFloatingSpecies * sizeof(double));
    }

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
}

void LLVMExecutableModel::evalEvents(const double time,
        const double *y)
{
}

void LLVMExecutableModel::resetEvents()
{
}

void LLVMExecutableModel::testConstraints()
{
}

string LLVMExecutableModel::getInfo()
{
    return "";
}

const SymbolList& LLVMExecutableModel::getConservations()
{
    return *(SymbolList*)0;
}

const StringList LLVMExecutableModel::getConservationNames()
{
    return 0;
}

int LLVMExecutableModel::getFloatingSpeciesIndex(const string& allocator)
{
    return 0;
}

string LLVMExecutableModel::getFloatingSpeciesName(int index)
{
    vector<string> ids = symbols->getFloatingSpeciesIds();
    return ids[index];
}

int LLVMExecutableModel::getBoundarySpeciesIndex(const string& allocator)
{
    return 0;
}

string LLVMExecutableModel::getBoundarySpeciesName(int int1)
{
    return string();
}

int LLVMExecutableModel::getBoundarySpeciesCompartmentIndex(int int1)
{
    return 0;
}

int LLVMExecutableModel::getGlobalParameterIndex(const string& allocator)
{
    return 0;
}

string LLVMExecutableModel::getGlobalParameterName(int int1)
{
    return string();
}

int LLVMExecutableModel::getCompartmentIndex(const string& allocator)
{
    return 0;
}

string LLVMExecutableModel::getCompartmentName(int int1)
{
    return string();
}

int LLVMExecutableModel::getReactionIndex(const string& allocator)
{
    return 0;
}

string LLVMExecutableModel::getReactionName(int int1)
{
    return string();
}

double LLVMExecutableModel::getGlobalParameterValue(int index)
{
    return 0;
}

void LLVMExecutableModel::setGlobalParameterValue(int index, double value)
{
}

int LLVMExecutableModel::pushState(unsigned)
{
    modeldata_copy_buffers(&modelDataCopy, &modelData);
    return 0;
}

int LLVMExecutableModel::popState(unsigned)
{
    modeldata_copy_buffers(&modelData, &modelDataCopy);
    return 0;
}

void LLVMExecutableModel::evalInitialConditions()
{
    evalInitialConditionsPtr(&modelData);
}

void LLVMExecutableModel::reset()
{
    setTime(0.0);
    evalInitialConditions();
    evalReactionRates();

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
        Log(Logger::PRIO_TRACE) << __FUNC__ << ", stateVector: null, returning " << modelData.numRateRules + modelData.numFloatingSpecies;
        return modelData.numRateRules + modelData.numFloatingSpecies;
    }

    getRateRuleValues(stateVector);

    memcpy(stateVector + modelData.numRateRules,
            modelData.floatingSpeciesAmounts,
            modelData.numFloatingSpecies * sizeof(double));

    if (Logger::PRIO_TRACE <= rr::Logger::GetLogLevel()) {

        LoggingBuffer log(Logger::PRIO_TRACE, __FILE__, __LINE__);

        log.stream() << endl << __FUNC__ <<  ", Model: " << endl << this;

        log.stream() << __FUNC__ << ",  out stateVector: ";
        if (stateVector) {
            dump_array(log.stream(), modelData.numRateRules + modelData.numFloatingSpecies, stateVector);
        } else {
            log.stream() << "null";
        }

        cout << "pause\n";
    }

    return modelData.numRateRules + modelData.numFloatingSpecies;
}

int LLVMExecutableModel::setStateVector(const double* stateVector)
{
    if (stateVector == 0)
    {
        return -1;
    }

    setRateRuleValues(stateVector);

    memcpy(modelData.floatingSpeciesAmounts,
            stateVector + modelData.numRateRules,
            modelData.numFloatingSpecies * sizeof(double));

    if (Logger::PRIO_TRACE <= rr::Logger::GetLogLevel()) {

        LoggingBuffer log(Logger::PRIO_TRACE, __FILE__, __LINE__);

        log.stream() << endl << __FUNC__ <<  ", Model: " << endl << this;

        log.stream() << __FUNC__ << ",  stateVector: ";
        if (stateVector) {
            dump_array(log.stream(), modelData.numRateRules + modelData.numFloatingSpecies, stateVector);
        } else {
            log.stream() << "null";
        }

        cout << "pause\n";
    }

    return modelData.numRateRules + modelData.numFloatingSpecies;
}

void LLVMExecutableModel::print(std::ostream &stream)
{
    stream << "LLVMExecutableModel" << endl;
    stream << "stackDepth: " << stackDepth << endl;
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


} /* namespace rr */


