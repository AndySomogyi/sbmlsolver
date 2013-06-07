/*
 * rrLLVMExecutableModel.cpp
 *
 *  Created on: Jun 3, 2013
 *      Author: andy
 */

#include "rrLLVMExecutableModel.h"

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

void LLVMExecutableModel::assignCVodeInterface(CvodeInterface* cvodeI)
{
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

CvodeInterface* LLVMExecutableModel::getCvodeInterface()
{
    return 0;
}

int LLVMExecutableModel::getNumIndependentVariables()
{
    return 0;
}

int LLVMExecutableModel::getNumDependentVariables()
{
    return 0;
}

int LLVMExecutableModel::getNumTotalVariables()
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
