/*
 * CXXExecutableModel.cpp
 *
 *  Created on: Dec 11, 2014
 *      Author: andy
 */

#include "CXXExecutableModel.h"
#include "rrLogger.h"
#include "rrOSSpecifics.h"

using rr::Logger;

namespace rrtesting
{

CXXExecutableModel::CXXExecutableModel(const rr::Dictionary* dict)
{
    Log(Logger::LOG_NOTICE) << __FUNC__;
}

CXXExecutableModel::~CXXExecutableModel()
{
    Log(Logger::LOG_NOTICE) << __FUNC__;
}

std::string CXXExecutableModel::getModelName()
{
    return "C++ Test Model";
}

void CXXExecutableModel::setTime(double _time)
{
}

double CXXExecutableModel::getTime()
{
    return 0;
}

void CXXExecutableModel::reset()
{
}

int CXXExecutableModel::getNumDepFloatingSpecies()
{
    return 0;
}

int CXXExecutableModel::getNumFloatingSpecies()
{
    return 0;
}

int CXXExecutableModel::getFloatingSpeciesIndex(const std::string& eid)
{
    return 0;
}

std::string CXXExecutableModel::getFloatingSpeciesId(int index)
{
    return "";
}

int CXXExecutableModel::getNumIndFloatingSpecies()
{
    return 0;
}

int CXXExecutableModel::getFloatingSpeciesAmounts(int len, const int* indx,
        double* values)
{
    return 0;
}

int CXXExecutableModel::setFloatingSpeciesAmounts(int len, const int* indx,
        const double* values)
{
    return 0;
}

int CXXExecutableModel::getFloatingSpeciesAmountRates(int len, const int* indx,
        double* values)
{
    return 0;
}

int CXXExecutableModel::getFloatingSpeciesConcentrationRates(int len,
        const int* indx, double* values)
{
    return 0;
}

int CXXExecutableModel::getFloatingSpeciesConcentrations(int len,
        const int* indx, double* values)
{
    return 0;
}

int CXXExecutableModel::setFloatingSpeciesConcentrations(int len,
        const int* indx, const double* values)
{
    return 0;
}

int CXXExecutableModel::setFloatingSpeciesInitConcentrations(int len,
        const int* indx, const double* values)
{
    return 0;
}

int CXXExecutableModel::getFloatingSpeciesInitConcentrations(int len,
        const int* indx, double* values)
{
    return 0;
}

int CXXExecutableModel::setFloatingSpeciesInitAmounts(int len, const int* indx,
        const double* values)
{
    return 0;
}

int CXXExecutableModel::getFloatingSpeciesInitAmounts(int len, const int* indx,
        double* values)
{
    return 0;
}

int CXXExecutableModel::getNumBoundarySpecies()
{
    return 0;
}

int CXXExecutableModel::getBoundarySpeciesIndex(const std::string& eid)
{
    return 0;
}

std::string CXXExecutableModel::getBoundarySpeciesId(int index)
{
    return "";
}

int CXXExecutableModel::getBoundarySpeciesAmounts(int len, const int* indx,
        double* values)
{
    return 0;
}

int CXXExecutableModel::getBoundarySpeciesConcentrations(int len,
        const int* indx, double* values)
{
    return 0;
}

int CXXExecutableModel::setBoundarySpeciesConcentrations(int len,
        const int* indx, const double* values)
{
    return 0;
}

int CXXExecutableModel::getNumGlobalParameters()
{
    return 0;
}

int CXXExecutableModel::getGlobalParameterIndex(const std::string& eid)
{
    return 0;
}

std::string CXXExecutableModel::getGlobalParameterId(int index)
{
    return "";
}

int CXXExecutableModel::getGlobalParameterValues(int len, const int* indx,
        double* values)
{
    return 0;
}

int CXXExecutableModel::setGlobalParameterValues(int len, const int* indx,
        const double* values)
{
    return 0;
}

int CXXExecutableModel::getNumCompartments()
{
    return 0;
}

int CXXExecutableModel::getCompartmentIndex(const std::string& eid)
{
    return 0;
}

std::string CXXExecutableModel::getCompartmentId(int index)
{
    return "";
}

int CXXExecutableModel::getCompartmentVolumes(int len, const int* indx,
        double* values)
{
    return 0;
}

int CXXExecutableModel::setCompartmentVolumes(int len, const int* indx,
        const double* values)
{
    return 0;
}

int CXXExecutableModel::setCompartmentInitVolumes(int len, const int* indx,
        const double* values)
{
    return 0;
}

int CXXExecutableModel::getCompartmentInitVolumes(int len, const int* indx,
        double* values)
{
    return 0;
}

void CXXExecutableModel::getIds(int types, std::list<std::string>& ids)
{
}

int CXXExecutableModel::getSupportedIdTypes()
{
    return 0;
}

double CXXExecutableModel::getValue(const std::string& id)
{
    return 0;
}

void CXXExecutableModel::setValue(const std::string& id, double value)
{
}

int CXXExecutableModel::getStoichiometryMatrix(int* rows, int* cols,
        double** data)
{
    return 0;
}

double CXXExecutableModel::getStoichiometry(int speciesIndex, int reactionIndex)
{
    return 0;
}

int CXXExecutableModel::getNumConservedMoieties()
{
    return 0;
}

int CXXExecutableModel::getConservedMoietyIndex(const std::string& eid)
{
    return 0;
}

std::string CXXExecutableModel::getConservedMoietyId(int index)
{
    return "";
}

int CXXExecutableModel::getConservedMoietyValues(int len, const int* indx,
        double* values)
{
    return 0;
}

int CXXExecutableModel::setConservedMoietyValues(int len, const int* indx,
        const double* values)
{
    return 0;
}

int CXXExecutableModel::getNumRateRules()
{
    return 0;
}

int CXXExecutableModel::getNumReactions()
{
    return 0;
}

int CXXExecutableModel::getReactionIndex(const std::string& eid)
{
    return 0;
}

std::string CXXExecutableModel::getReactionId(int index)
{
    return "";
}

int CXXExecutableModel::getReactionRates(int len, const int* indx,
        double* values)
{
    return 0;
}

void CXXExecutableModel::getRateRuleValues(double* rateRuleValues)
{
}

std::string CXXExecutableModel::getStateVectorId(int index)
{
    return "";
}

int CXXExecutableModel::getStateVector(double* stateVector)
{
    return 0;
}

int CXXExecutableModel::setStateVector(const double* stateVector)
{
    return 0;
}

void CXXExecutableModel::getStateVectorRate(double time, const double* y,
        double* dydt)
{
}

void CXXExecutableModel::testConstraints()
{
}

std::string CXXExecutableModel::getInfo()
{
    return "C++ Test Model";
}

void CXXExecutableModel::print(std::ostream& stream)
{
}

int CXXExecutableModel::getNumEvents()
{
    return 0;
}

int CXXExecutableModel::getEventTriggers(int len, const int* indx,
        unsigned char* values)
{
    return 0;
}

int CXXExecutableModel::applyEvents(double timeEnd,
        const unsigned char* previousEventStatus, const double* initialState,
        double* finalState)
{
    return 0;
}

int CXXExecutableModel::applyPendingEvents(double timeEnd)
{
    return 0;
}

void CXXExecutableModel::getEventRoots(double time, const double* y,
        double* gdot)
{
}

double CXXExecutableModel::getNextPendingEventTime(bool pop)
{
    return 0;
}

int CXXExecutableModel::getPendingEventSize()
{
    return 0;
}

void CXXExecutableModel::resetEvents()
{
}

int CXXExecutableModel::getEventIndex(const std::string& eid)
{
    return 0;
}

std::string CXXExecutableModel::getEventId(int index)
{
    return "";
}

void CXXExecutableModel::setEventListener(int index,
        rr::EventListenerPtr eventHandler)
{
}

rr::EventListenerPtr CXXExecutableModel::getEventListener(int index)
{
    return rr::EventListenerPtr();
}

double CXXExecutableModel::getFloatingSpeciesAmountRate(int index,
        const double* reactionRates)
{
    return 0;
}

void CXXExecutableModel::reset(int options)
{
}

void CXXExecutableModel::setRandomSeed(int64_t longLongInt)
{
}

int64_t CXXExecutableModel::getRandomSeed()
{
    return 0;
}

double CXXExecutableModel::getRandom()
{
    return 0;
}

uint32_t CXXExecutableModel::getFlags() const
{
    return 0;
}

void CXXExecutableModel::setFlags(uint32_t unsignedInt)
{
}

} /* namespace rrtesting */
