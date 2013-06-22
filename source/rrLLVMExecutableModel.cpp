/*
 * rrLLVMExecutableModel.cpp
 *
 * Author: Andy Somogyi,
 *     email decode: V1 = "."; V2 = "@"; V3 = V1;
 *     andy V1 somogyi V2 gmail V3 com
 */

#include "rrLLVMExecutableModel.h"

#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/JIT.h"
#include "llvm/LLVMContext.h"
#include "llvm/Module.h"


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

int LLVMExecutableModel::getNumRules()
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

vector<double> LLVMExecutableModel::getCurrentValues()
{
    return vector<double>();
}

double LLVMExecutableModel::getAmounts(const int& i)
{
    return 0;
}

void LLVMExecutableModel::initializeRates()
{
}

void LLVMExecutableModel::assignRates()
{
}

void LLVMExecutableModel::assignRates(vector<double>& rates)
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

void LLVMExecutableModel::evalModel(const double& time, const vector<double>& y)
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

const SymbolList& LLVMExecutableModel::getReactions()
{
    return *(SymbolList*)0;
}

const SymbolList& LLVMExecutableModel::getGlobalParameters()
{
    return *(SymbolList*)0;
}

const SymbolList& LLVMExecutableModel::getBoundarySpecies()
{
    return *(SymbolList*)0;
}

const SymbolList& LLVMExecutableModel::getCompartments()
{
    return *(SymbolList*)0;
}

const SymbolList& LLVMExecutableModel::getConservations()
{
    return *(SymbolList*)0;
}

const SymbolList& LLVMExecutableModel::getFloatingSpeciesAmounts()
{
    return *(SymbolList*)0;
}

const SymbolList& LLVMExecutableModel::getFloatingSpeciesConcentrations()
{
    return *(SymbolList*)0;
}

const StringList LLVMExecutableModel::getCompartmentNames()
{
    return 0;
}

const StringList LLVMExecutableModel::getConservationNames()
{
    return 0;
}

const StringList LLVMExecutableModel::getGlobalParameterNames()
{
    return 0;
}

const StringList LLVMExecutableModel::getReactionNames()
{
    return 0;
}

const StringList LLVMExecutableModel::getFloatingSpeciesConcentrationNames()
{
    return 0;
}

const StringList LLVMExecutableModel::getBoundarySpeciesNames()
{
    return 0;
}

} /* namespace rr */
