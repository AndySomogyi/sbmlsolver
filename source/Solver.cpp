// == PREAMBLE ================================================

// * Licensed under the Apache License, Version 2.0; see README

// == FILEDOC =================================================

/** @file Solver.cpp
* @author ETS, WBC, JKM
* @date Apr 25, 2014
* @copyright Apache License, Version 2.0
* @brief Contains the base class for RoadRunner Solvers
**/

// == INCLUDES ================================================

#include "Solver.h"
#include "rrStringUtils.h"
#include "rrConfig.h"
#include "rrUtils.h"
#include <typeinfo>

// == CODE ====================================================

using namespace std;
namespace rr
{
	/*------------------------------------------------------------------------------------------
		Solver
	  ------------------------------------------------------------------------------------------*/

	void Solver::addSetting(string name, Variant val, string hint, string description)
	{
		settings.insert({ name, val });
		hints.insert({ name, hint });
		descriptions.insert({ name, description });
	}

	void Solver::loadConfigSettings()
	{
		// empty
	}

	void Solver::loadSBMLSettings(const std::string& filename)
	{
		// Stub for loading SBML settings (can override in derived classes).
	}

	std::vector<string> Solver::getSettings()
	{
		std::vector<string> keys;
		for (auto entry : settings)
		{
			keys.push_back(entry.first);
		}
		return keys;
	}

	Variant Solver::getValue(std::string key)
	{
		std::unordered_map<string, Variant>::const_iterator option = settings.find(key);
		if (option == settings.end())
		{
			throw std::invalid_argument("invalid key: " + key);
		}
		return option->second;
	}

	int Solver::getValueAsInt(std::string key)
	{
		return getValue(key).convert<int>();
	}

	unsigned int Solver::getValueAsUInt(std::string key)
	{
		return getValue(key).convert<unsigned int>();
	}

	long Solver::getValueAsLong(std::string key)
	{
		return getValue(key).convert<long>();
	}

	unsigned long Solver::getValueAsULong(std::string key)
	{
		return getValue(key).convert<unsigned long>();
	}

	float Solver::getValueAsFloat(std::string key)
	{
		return getValue(key).convert<float>();
	}

	double Solver::getValueAsDouble(std::string key)
	{
		return getValue(key).convert<double>();
	}

	char Solver::getValueAsChar(std::string key)
	{
		return getValue(key).convert<char>();
	}

	unsigned char Solver::getValueAsUChar(std::string key)
	{
		return getValue(key).convert<unsigned char>();
	}

	std::string Solver::getValueAsString(std::string key)
	{
		return getValue(key).convert<std::string>();
	}

	bool Solver::getValueAsBool(std::string key)
	{
		return getValue(key).convert<bool>();
	}

	void Solver::setValue(std::string key, const Variant& value)
	{
		settings[key] = value;
	}

	const std::string& Solver::getHint(std::string key) const
	{
		HintMap::const_iterator option = Solver::hints.find(key);
		if (option == hints.end())
		{
			throw std::invalid_argument("invalid key: " + key);
		}
		return option->second;
	}

	const std::string& Solver::getDescription(std::string key) const
	{
		DescriptionMap::const_iterator option = Solver::descriptions.find(key);
		if (option == descriptions.end())
		{
			throw std::invalid_argument("invalid key: " + key);
		}
		return option->second;
	}

	const Variant::TypeId Solver::getType(std::string key)
	{
		return getValue(key).type();
	}

	SolverRegistrar::~SolverRegistrar() {}

    /********************************************************************************************
    * Solver FACTORY
    ********************************************************************************************/

    SolverFactory::~SolverFactory() {
        for (SolverRegistrars::const_iterator it(mRegisteredSolvers.begin()); it != mRegisteredSolvers.end(); ++it) {
            delete *it;
        }
    }

    Solver* SolverFactory::New(std::string name, ExecutableModel* m) const {
        for (SolverRegistrars::const_iterator it(mRegisteredSolvers.begin()); it != mRegisteredSolvers.end(); ++it) {
            if ((*it)->getName() == name) {
                return (*it)->construct(m);
            }
        }
        throw InvalidKeyException("No such Solver: " + name);
    }

    void SolverFactory::registerSolver(SolverRegistrar* i) {
        if (!i)
            throw CoreException("Registrar is null");
        mRegisteredSolvers.push_back(i);
    }

    SolverFactory& SolverFactory::getInstance() {
        // FIXME: not thread safe -- JKM, July 24, 2015.
        static SolverFactory factory;
        return factory;
    }

    std::size_t SolverFactory::getNumSolvers() const {
        return mRegisteredSolvers.size();
    }

	std::vector<std::string> SolverFactory::getListSolverNames()
	{
		std::vector<std::string> intgNames;
		int numIntgs = static_cast<int>(SolverFactory::getInstance().mRegisteredSolvers.size());
		for (int i = 0; i < numIntgs; ++i)
		{
			std::size_t n = static_cast<std::size_t>(i);
			intgNames.push_back(SolverFactory::getInstance().getSolverName(n));
		}
		return intgNames;
	}

    std::string SolverFactory::getSolverName(std::size_t n) const {
        return mRegisteredSolvers.at(n)->getName();
    }

    std::string SolverFactory::getSolverHint(std::size_t n) const {
        return mRegisteredSolvers.at(n)->getHint();
    }

    std::string SolverFactory::getSolverDescription(std::size_t n) const {
        return mRegisteredSolvers.at(n)->getDescription();
    }

}