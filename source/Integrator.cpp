/*
* Integrator.cpp
*
*  Created on: Apr 25, 2014
*      Author: andy
*/

#include "Integrator.h"
#include "CVODEIntegrator.h"
#include "GillespieIntegrator.h"
#include "RK4Integrator.h"
#include "EulerIntegrator.h"
#include "rrStringUtils.h"
#include "rrConfig.h"
#include "rrUtils.h"

using namespace std;
namespace rr
{
	/*------------------------------------------------------------------------------------------
		INTEGRATOR
	  ------------------------------------------------------------------------------------------*/

	void Integrator::AddSetting(string name, Variant val, string hint, string description)
	{
		settings.insert({ name, val });
		hints.insert({ name, hint });
		descriptions.insert({ name, description });
	}

	void Integrator::loadConfigSettings()
	{
		//	VARIABLE STEP SIZE
		/*	Need better error handling.
			(1) What happens if variable_step_size does not exist in settings?
		*/
		bool bVal = false;
		if (getIntegrationMethod() == Integrator::IntegrationMethod::Deterministic)
		{
			bVal = Config::getBool(Config::SIMULATEOPTIONS_DETERMINISTIC_VARIABLE_STEP);
			Integrator::setValue("variable_step_size", bVal);
		}
		else if (getIntegrationMethod() == Integrator::IntegrationMethod::Stochastic)
		{
			bVal = Config::getBool(Config::SIMULATEOPTIONS_STOCHASTIC_VARIABLE_STEP);
			Integrator::setValue("variable_step_size", bVal);
		}

		// STIFFNESS
		bVal = Config::getBool(Config::SIMULATEOPTIONS_STIFF);
		Integrator::setValue("stiff", bVal);

		// MULTIPLE STEPS
		bVal = Config::getBool(Config::SIMULATEOPTIONS_MULTI_STEP);
		Integrator::setValue("multiple_steps", bVal);

		// ABSOLUTE TOLERANCE
		Integrator::setValue("absolute_tolerance", Config::getDouble(Config::SIMULATEOPTIONS_ABSOLUTE));
		Integrator::setValue("relative_tolerance", Config::getDouble(Config::SIMULATEOPTIONS_RELATIVE));
	}

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

	Variant Integrator::getValue(std::string key)
	{
		std::unordered_map<string, Variant>::const_iterator option = settings.find(key);
		if (option == settings.end())
		{
			throw std::invalid_argument("invalid key: " + key);
		}
		return option->second;
	}

	void Integrator::setValue(std::string key, const Variant& value)
	{
		settings.insert({ key, value });
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
		if (option == hints.end())
		{
			throw std::invalid_argument("invalid key: " + key);
		}
		return option->second;
	}

	/* TODO: Create getType() method. */

	std::string Integrator::toString() const
	{
		std::stringstream ss;
		ss << "< roadrunner.CVODEIntegrator() >" << endl;
		return ss.str();
	}



	/********************************************************************************************
	*	INTEGRATOR FACTORY
	********************************************************************************************/


	Integrator* IntegratorFactory::New(string name, ExecutableModel* m)
	{
		Integrator *result = 0;


		if (name == "cvode")
		{
			result = new CVODEIntegrator(m);
		}
		else
		{
			// Defaulting to CVODE
			result = new CVODEIntegrator(m);
		}

		return result;
	}

}
