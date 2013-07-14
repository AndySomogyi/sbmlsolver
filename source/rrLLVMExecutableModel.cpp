/*
 * rrLLVMExecutableModel.cpp
 *
 * Author: Andy Somogyi,
 *     email decode: V1 = "."; V2 = "@"; V3 = V1;
 *     andy V1 somogyi V2 gmail V3 com
 */

#include "rrLLVMExecutableModel.h"

#include "rrLLVMIncludes.h"


namespace rr
{

LLVMExecutableModel::LLVMExecutableModel()
{
    // TODO Auto-generated constructor stub

}

LLVMExecutableModel::~LLVMExecutableModel()
{
    // TODO Auto-generated destructor stub
}

string LLVMExecutableModel::getModelName()
{
    return "";
}

void LLVMExecutableModel::setTime(double _time)
{
}

double LLVMExecutableModel::getTime()
{
    return 0;
}

ModelData& LLVMExecutableModel::getModelData()
{
    return *(ModelData*)0;
}

int LLVMExecutableModel::getNumIndependentSpecies()
{
    return 0;
}

int LLVMExecutableModel::getNumDependentSpecies()
{
    return 0;
}

int LLVMExecutableModel::getNumFloatingSpecies()
{
    return 0;
}

int LLVMExecutableModel::getNumBoundarySpecies()
{
    return 0;
}

int LLVMExecutableModel::getNumGlobalParameters()
{
    return 0;
}

int LLVMExecutableModel::getNumCompartments()
{
    return 0;
}

int LLVMExecutableModel::getNumReactions()
{
    return 0;
}


int LLVMExecutableModel::getNumEvents()
{
    return 0;
}

void LLVMExecutableModel::computeEventPriorites()
{
}

void LLVMExecutableModel::setConcentration(int index, double value)
{
}

void LLVMExecutableModel::computeReactionRates(double time, double* y)
{
}

void LLVMExecutableModel::setCompartmentVolumes()
{
}

int LLVMExecutableModel::getNumLocalParameters(int reactionId)
{
    return 0;
}

void LLVMExecutableModel::computeRules(vector<double>& _y)
{
}

void LLVMExecutableModel::computeRules(double* ay, int size)
{
}

void LLVMExecutableModel::initializeInitialConditions()
{
}

void LLVMExecutableModel::setParameterValues()
{
}

void LLVMExecutableModel::setBoundaryConditions()
{
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

double LLVMExecutableModel::getAmounts(const int& i)
{
    return 0;
}

void LLVMExecutableModel::initializeRates()
{
}

void LLVMExecutableModel::setRateRuleValues(const double * rates)
{
}

void LLVMExecutableModel::convertToConcentrations()
{
}

void LLVMExecutableModel::updateDependentSpeciesValues(double* _y)
{
}

void LLVMExecutableModel::computeAllRatesOfChange()
{
}

void LLVMExecutableModel::evalModel(double time, const double *y)
{
}

void LLVMExecutableModel::evalEvents(const double& time,
        const vector<double>& y)
{
}

void LLVMExecutableModel::resetEvents()
{
}

void LLVMExecutableModel::testConstraints()
{
}

void LLVMExecutableModel::initializeRateRuleSymbols()
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
}

int LLVMExecutableModel::popState(unsigned)
{
}

LLVMExecutableModel* LLVMExecutableModel::dummy()
{
    return new LLVMExecutableModel();
}

} /* namespace rr */
