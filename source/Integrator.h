/*
* Integrator.h
*
*  Created on: Sep 7, 2013
*      Author: andy
*/

#ifndef INTEGRATOR_H_
#define INTEGRATOR_H_

#include "rrLogger.h"
#include "rrOSSpecifics.h"
#include "Dictionary.h"
#include "tr1proxy/rr_memory.h"
#include <stdexcept>


namespace rr
{

	class Integrator;
	class ExecutableModel;

	/*-------------------------------------------------------------------------------------------
		IntegratorListener listens for integrator events.
	---------------------------------------------------------------------------------------------*/
	class IntegratorListener
	{
	public:

		/**
		* is called after the internal integrator completes each internal time step.
		*/
		virtual uint onTimeStep(Integrator* integrator, ExecutableModel* model, double time) = 0;

		/**
		* whenever model event occurs and after it is procesed.
		*/
		virtual uint onEvent(Integrator* integrator, ExecutableModel* model, double time) = 0;

		virtual ~IntegratorListener() {};
	};

	typedef cxx11_ns::shared_ptr<IntegratorListener> IntegratorListenerPtr;
	typedef std::unordered_map<std::string, std::string> HintMap;
	typedef std::unordered_map<std::string, std::string> DescriptionMap;

	/*-------------------------------------------------------------------------------------------
		Integrator is an abstract base class that provides an interface to specific integrator
		class implementations.
	---------------------------------------------------------------------------------------------*/
	class RR_DECLSPEC Integrator
	{
	public:
		enum IntegrationMethod
		{
			Deterministic,
			Stochastic,
			Hybrid,
			Other
		};

		virtual ~Integrator() {};

		virtual void loadConfigSettings();
		virtual void loadSBMLSettings(const std::string& filename) = 0;
		virtual std::string getIntegratorName() const = 0;
		virtual std::string getIntegratorDescription() const = 0;
		virtual std::string getIntegratorHint() const = 0;
		virtual IntegrationMethod getIntegrationMethod() const = 0;
		std::vector<std::string> getSettings();

		virtual Variant getValue(std::string key);
		virtual void setValue(std::string key, const Variant& value) = 0;
		const std::string& getHint(std::string key) const;
		const std::string& getDescription(std::string key) const;
		const std::string& getType(std::string key) const;

		virtual double integrate(double t0, double hstep) = 0;
		virtual void restart(double t0) = 0;

		/* CARRYOVER METHODS */
		virtual void setListener(IntegratorListenerPtr) = 0;
		virtual IntegratorListenerPtr getListener() = 0;
		std::string toString() const;
		/* !-- END OF CARRYOVER METHODS */

	protected:
		std::unordered_map<std::string, Variant> settings;
		HintMap hints;
		DescriptionMap descriptions;

		void AddSetting(std::string name, Variant val, std::string hint, std::string description);
	};


	class IntegratorException : public std::runtime_error
	{
	public:
		explicit IntegratorException(const std::string& what) :
			std::runtime_error(what)
		{
				Log(rr::Logger::LOG_ERROR) << __FUNC__ << "what: " << what;
			}

		explicit IntegratorException(const std::string& what, const std::string &where) :
			std::runtime_error(what + "; In " + where)
		{
				Log(rr::Logger::LOG_ERROR) << __FUNC__ << "what: " << what << ", where: " << where;
			}
	};

	/* */
	class RR_DECLSPEC IntegratorFactory
	{
	public:

		static Integrator* New(std::string name, ExecutableModel *m);

		static std::vector<std::string> getIntegratorNames();

	};

}

#endif /* INTEGRATOR_H_ */
