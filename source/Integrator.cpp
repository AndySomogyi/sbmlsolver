// == PREAMBLE ================================================

// * Licensed under the Apache License, Version 2.0; see README

// == FILEDOC =================================================

/** @file Integrator.cpp
* @author ETS, WBC, JKM
* @date Apr 25, 2014
* @copyright Apache License, Version 2.0
* @brief Contains the base class for RoadRunner integrators
**/

// == INCLUDES ================================================

#include "Integrator.h"
// #include "CVODEIntegrator.h"
// #include "GillespieIntegrator.h"
// #include "RK4Integrator.h"
// #include "EulerIntegrator.h"
#include "rrExecutableModel.h"
#include "rrStringUtils.h"
#include "rrConfig.h"
#include "rrUtils.h"
#include <typeinfo>

// == CODE ====================================================

using namespace std;
namespace rr
{
	/*------------------------------------------------------------------------------------------
		INTEGRATOR
	  ------------------------------------------------------------------------------------------*/

	void Integrator::addSetting(string name, Variant val, string hint, string description)
	{
		settings.insert({ name, val });
		hints.insert({ name, hint });
		descriptions.insert({ name, description });
	}

	void Integrator::syncWithModel(ExecutableModel* m) {}

	void Integrator::loadConfigSettings() {}

	void Integrator::loadSBMLSettings(const std::string& filename)
	{
		// Stub for loading SBML settings (can override in derived classes).
	}

	std::vector<string> Integrator::getSettings()
	{
		std::vector<string> keys;
		for (auto entry : settings)
		{
			keys.push_back(entry.first);
		}
		return keys;
	}

//     void Integrator::resetSettings()
//     {
//         settings.clear();
//         hints.clear();
//         descriptions.clear();
//     }

	Variant Integrator::getValue(std::string key)
	{
		std::unordered_map<string, Variant>::const_iterator option = settings.find(key);
		if (option == settings.end())
		{
			throw std::invalid_argument("Integrator::getValue: invalid key: " + key);
		}
		return option->second;
	}

	int Integrator::getValueAsInt(std::string key)
	{
		return getValue(key).convert<int>();
	}

	unsigned int Integrator::getValueAsUInt(std::string key)
	{
		return getValue(key).convert<unsigned int>();
	}

	long Integrator::getValueAsLong(std::string key)
	{
		return getValue(key).convert<long>();
	}

	unsigned long Integrator::getValueAsULong(std::string key)
	{
		return getValue(key).convert<unsigned long>();
	}

	float Integrator::getValueAsFloat(std::string key)
	{
		return getValue(key).convert<float>();
	}

	double Integrator::getValueAsDouble(std::string key)
	{
		return getValue(key).convert<double>();
	}

	char Integrator::getValueAsChar(std::string key)
	{
		return getValue(key).convert<char>();
	}

	unsigned char Integrator::getValueAsUChar(std::string key)
	{
		return getValue(key).convert<unsigned char>();
	}

	std::string Integrator::getValueAsString(std::string key)
	{
		return getValue(key).convert<std::string>();
	}

	bool Integrator::getValueAsBool(std::string key)
	{
		return getValue(key).convert<bool>();
	}

	void Integrator::setValue(std::string key, const Variant& value)
	{
		settings[key] = value;
	}

	const std::string& Integrator::getHint(std::string key) const
	{
		HintMap::const_iterator option = Integrator::hints.find(key);
		if (option == hints.end())
		{
			throw std::invalid_argument("invalid key: " + key);
		}
		return option->second;
	}

	const std::string& Integrator::getDescription(std::string key) const
	{
		DescriptionMap::const_iterator option = Integrator::descriptions.find(key);
		if (option == descriptions.end())
		{
			throw std::invalid_argument("invalid key: " + key);
		}
		return option->second;
	}

	const Variant::TypeId Integrator::getType(std::string key)
	{
		return getValue(key).type();
	}

	/* TODO: Create getType() method. */

	std::string Integrator::toString() const
	{
		std::stringstream ss;
		ss << "< roadrunner.Integrator() >" << endl;
		return ss.str();

	}
	IntegratorRegistrar::~IntegratorRegistrar() {}

    /********************************************************************************************
    * INTEGRATOR FACTORY
    ********************************************************************************************/

    IntegratorFactory::~IntegratorFactory() {
        for (IntegratorRegistrars::const_iterator it(mRegisteredIntegrators.begin()); it != mRegisteredIntegrators.end(); ++it) {
            delete *it;
        }
    }

    Integrator* IntegratorFactory::New(std::string name, ExecutableModel* m) const {
        for (IntegratorRegistrars::const_iterator it(mRegisteredIntegrators.begin()); it != mRegisteredIntegrators.end(); ++it) {
            if ((*it)->getName() == name) {
                return (*it)->construct(m);
            }
        }
        throw InvalidKeyException("No such integrator: " + name);
    }

    void IntegratorFactory::registerIntegrator(IntegratorRegistrar* i) {
        if (!i)
            throw CoreException("Registrar is null");
        mRegisteredIntegrators.push_back(i);
    }

    IntegratorFactory& IntegratorFactory::getInstance() {
        // FIXME: not thread safe -- JKM, July 24, 2015.
        static IntegratorFactory factory;
        return factory;
    }

    std::size_t IntegratorFactory::getNumIntegrators() const {
        return mRegisteredIntegrators.size();
    }

    std::string IntegratorFactory::getIntegratorName(std::size_t n) const {
        return mRegisteredIntegrators.at(n)->getName();
    }

    std::string IntegratorFactory::getIntegratorHint(std::size_t n) const {
        return mRegisteredIntegrators.at(n)->getHint();
    }

    std::string IntegratorFactory::getIntegratorDescription(std::size_t n) const {
        return mRegisteredIntegrators.at(n)->getDescription();
    }

}