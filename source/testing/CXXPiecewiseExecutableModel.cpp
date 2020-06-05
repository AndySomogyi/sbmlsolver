/*
 * CXXPiecewiseExecutableModel.cpp
 *
 *  Created on: Dec 11, 2014
 *      Author: andy
 */

#include "CXXPiecewiseExecutableModel.h"
#include "rrLogger.h"
#include "rrOSSpecifics.h"

using rr::Logger;

namespace rrtesting
{

CXXPiecewiseExecutableModel::CXXPiecewiseExecutableModel(const rr::Dictionary* dict)
{
    Log(Logger::LOG_NOTICE) << __FUNC__;
}

CXXPiecewiseExecutableModel::~CXXPiecewiseExecutableModel()
{
    Log(Logger::LOG_NOTICE) << __FUNC__;
}

std::string CXXPiecewiseExecutableModel::getModelName()
{
    return "C++ Test Model";
}

void CXXPiecewiseExecutableModel::setTime(double _time)
{
}

double CXXPiecewiseExecutableModel::getTime()
{
    return 0;
}

void CXXPiecewiseExecutableModel::reset()
{
}

int CXXPiecewiseExecutableModel::getNumDepFloatingSpecies()
{
    return 0;
}

int CXXPiecewiseExecutableModel::getNumFloatingSpecies()
{
    return 0;
}

int CXXPiecewiseExecutableModel::getFloatingSpeciesIndex(const std::string& eid)
{
    return 0;
}

std::string CXXPiecewiseExecutableModel::getFloatingSpeciesId(int index)
{
    return "";
}

int CXXPiecewiseExecutableModel::getNumIndFloatingSpecies()
{
    return 0;
}

int CXXPiecewiseExecutableModel::getFloatingSpeciesAmounts(size_t len, const int* indx,
        double* values)
{
    return 0;
}

int CXXPiecewiseExecutableModel::setFloatingSpeciesAmounts(size_t len, const int* indx,
        const double* values)
{
    return 0;
}

int CXXPiecewiseExecutableModel::getFloatingSpeciesAmountRates(size_t len, const int* indx,
        double* values)
{
    return 0;
}

int CXXPiecewiseExecutableModel::getFloatingSpeciesConcentrationRates(size_t len,
        const int* indx, double* values)
{
    return 0;
}

int CXXPiecewiseExecutableModel::getFloatingSpeciesConcentrations(size_t len,
        const int* indx, double* values)
{
    return 0;
}

int CXXPiecewiseExecutableModel::setFloatingSpeciesConcentrations(size_t len,
        const int* indx, const double* values)
{
    return 0;
}

int CXXPiecewiseExecutableModel::setFloatingSpeciesInitConcentrations(size_t len,
        const int* indx, const double* values)
{
    return 0;
}

int CXXPiecewiseExecutableModel::getFloatingSpeciesInitConcentrations(size_t len,
        const int* indx, double* values)
{
    return 0;
}

int CXXPiecewiseExecutableModel::setFloatingSpeciesInitAmounts(size_t len, const int* indx,
        const double* values)
{
    return 0;
}

int CXXPiecewiseExecutableModel::getFloatingSpeciesInitAmounts(size_t len, const int* indx,
        double* values)
{
    return 0;
}

int CXXPiecewiseExecutableModel::getNumBoundarySpecies()
{
    return 0;
}

int CXXPiecewiseExecutableModel::getBoundarySpeciesIndex(const std::string& eid)
{
    return 0;
}

std::string CXXPiecewiseExecutableModel::getBoundarySpeciesId(int index)
{
    return "";
}

int CXXPiecewiseExecutableModel::getBoundarySpeciesAmounts(size_t len, const int* indx,
        double* values)
{
    return 0;
}

int CXXPiecewiseExecutableModel::getBoundarySpeciesConcentrations(size_t len,
        const int* indx, double* values)
{
    return 0;
}

int CXXPiecewiseExecutableModel::setBoundarySpeciesConcentrations(size_t len,
        const int* indx, const double* values)
{
    return 0;
}

int CXXPiecewiseExecutableModel::getNumGlobalParameters()
{
    return 0;
}

int CXXPiecewiseExecutableModel::getGlobalParameterIndex(const std::string& eid)
{
    return 0;
}

std::string CXXPiecewiseExecutableModel::getGlobalParameterId(int index)
{
    return "";
}

int CXXPiecewiseExecutableModel::getGlobalParameterValues(size_t len, const int* indx,
        double* values)
{
    return 0;
}

int CXXPiecewiseExecutableModel::setGlobalParameterValues(size_t len, const int* indx,
        const double* values)
{
    return 0;
}

int CXXPiecewiseExecutableModel::getNumCompartments()
{
    return 0;
}

int CXXPiecewiseExecutableModel::getCompartmentIndex(const std::string& eid)
{
    return 0;
}

std::string CXXPiecewiseExecutableModel::getCompartmentId(int index)
{
    return "";
}

int CXXPiecewiseExecutableModel::getCompartmentVolumes(size_t len, const int* indx,
        double* values)
{
    return 0;
}

int CXXPiecewiseExecutableModel::setCompartmentVolumes(size_t len, const int* indx,
        const double* values)
{
    return 0;
}

int CXXPiecewiseExecutableModel::setCompartmentInitVolumes(size_t len, const int* indx,
        const double* values)
{
    return 0;
}

int CXXPiecewiseExecutableModel::getCompartmentInitVolumes(size_t len, const int* indx,
        double* values)
{
    return 0;
}

void CXXPiecewiseExecutableModel::getIds(int types, std::list<std::string>& ids)
{
}

int CXXPiecewiseExecutableModel::getSupportedIdTypes()
{
    return 0;
}

double CXXPiecewiseExecutableModel::getValue(const std::string& id)
{
    return 0;
}

void CXXPiecewiseExecutableModel::setValue(const std::string& id, double value)
{
}

int CXXPiecewiseExecutableModel::getStoichiometryMatrix(int* rows, int* cols,
        double** data)
{
    return 0;
}

double CXXPiecewiseExecutableModel::getStoichiometry(int speciesIndex, int reactionIndex)
{
    return 0;
}

int CXXPiecewiseExecutableModel::getNumConservedMoieties()
{
    return 0;
}

int CXXPiecewiseExecutableModel::getConservedMoietyIndex(const std::string& eid)
{
    return 0;
}

std::string CXXPiecewiseExecutableModel::getConservedMoietyId(int index)
{
    return "";
}

int CXXPiecewiseExecutableModel::getConservedMoietyValues(size_t len, const int* indx,
        double* values)
{
    return 0;
}

int CXXPiecewiseExecutableModel::setConservedMoietyValues(size_t len, const int* indx,
        const double* values)
{
    return 0;
}

int CXXPiecewiseExecutableModel::getNumRateRules()
{
    return 0;
}

int CXXPiecewiseExecutableModel::getNumReactions()
{
    return 0;
}

int CXXPiecewiseExecutableModel::getReactionIndex(const std::string& eid)
{
    return 0;
}

std::string CXXPiecewiseExecutableModel::getReactionId(int index)
{
    return "";
}

int CXXPiecewiseExecutableModel::getReactionRates(size_t len, const int* indx,
        double* values)
{
    return 0;
}

void CXXPiecewiseExecutableModel::getRateRuleValues(double* rateRuleValues)
{
}

std::string CXXPiecewiseExecutableModel::getStateVectorId(int index)
{
    return "";
}

int CXXPiecewiseExecutableModel::getStateVector(double* stateVector)
{
    return 0;
}

int CXXPiecewiseExecutableModel::setStateVector(const double* stateVector)
{
    return 0;
}

void CXXPiecewiseExecutableModel::getStateVectorRate(double time, const double* y,
        double* dydt)
{
}

void CXXPiecewiseExecutableModel::testConstraints()
{
}

std::string CXXPiecewiseExecutableModel::getInfo()
{
    return "C++ Test Model";
}

void CXXPiecewiseExecutableModel::print(std::ostream& stream)
{
}

int CXXPiecewiseExecutableModel::getNumEvents()
{
    return 0;
}

int CXXPiecewiseExecutableModel::getEventTriggers(size_t len, const int* indx,
        unsigned char* values)
{
    return 0;
}

int CXXPiecewiseExecutableModel::applyEvents(double timeEnd,
        const unsigned char* previousEventStatus, const double* initialState,
        double* finalState)
{
    return 0;
}

int CXXPiecewiseExecutableModel::applyPendingEvents(double timeEnd)
{
    return 0;
}

void CXXPiecewiseExecutableModel::getEventRoots(double time, const double* y,
        double* gdot)
{
}

double CXXPiecewiseExecutableModel::getNextPendingEventTime(bool pop)
{
    return 0;
}

int CXXPiecewiseExecutableModel::getPendingEventSize()
{
    return 0;
}

void CXXPiecewiseExecutableModel::resetEvents()
{
}

int CXXPiecewiseExecutableModel::getEventIndex(const std::string& eid)
{
    return 0;
}

std::string CXXPiecewiseExecutableModel::getEventId(int index)
{
    return "";
}

void CXXPiecewiseExecutableModel::setEventListener(int index,
        rr::EventListenerPtr eventHandler)
{
}

rr::EventListenerPtr CXXPiecewiseExecutableModel::getEventListener(int index)
{
    return rr::EventListenerPtr();
}

double CXXPiecewiseExecutableModel::getFloatingSpeciesAmountRate(int index,
        const double* reactionRates)
{
    return 0;
}

void CXXPiecewiseExecutableModel::reset(int options)
{
}

void CXXPiecewiseExecutableModel::setRandomSeed(int64_t longLongInt)
{
}

int64_t CXXPiecewiseExecutableModel::getRandomSeed()
{
    return 0;
}

double CXXPiecewiseExecutableModel::getRandom()
{
    return 0;
}

uint32_t CXXPiecewiseExecutableModel::getFlags() const
{
    return 0;
}

void CXXPiecewiseExecutableModel::setFlags(uint32_t unsignedInt)
{
}

} /* namespace rrtesting */
