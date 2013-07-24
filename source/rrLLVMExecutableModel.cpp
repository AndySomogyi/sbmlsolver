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

namespace rr
{

LLVMExecutableModel::LLVMExecutableModel() :
    symbols(0),
    context(0),
    executionEngine(0),
    errStr(0),
    evalInitialConditionsPtr(0),
    evalReactionRatesPtr(0)
{
    // zero out the struct, the generator will fill it out.
    initModelData(modelData);
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

void LLVMExecutableModel::setInitialConditions()
{
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

double LLVMExecutableModel::getConcentration(int index)
{
    return 0;
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

string LLVMExecutableModel::getFloatingSpeciesName(int int1)
{
    return string();
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
    return 0;
}

int LLVMExecutableModel::popState(unsigned)
{
    return 0;
}

void LLVMExecutableModel::evalInitialConditions()
{
    evalInitialConditionsPtr(&modelData);
}

void LLVMExecutableModel::reset()
{
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
        return modelData.numRateRules + modelData.numFloatingSpecies;
    }

    getRateRuleValues(stateVector);

    memcpy(stateVector + modelData.numRateRules,
            modelData.floatingSpeciesAmounts, modelData.numFloatingSpecies);

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
            modelData.numIndependentSpecies);

    return modelData.numRateRules + modelData.numFloatingSpecies;
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


} /* namespace rr */


