/*
 * IIntegrator.h
 *
 *  Created on: April 23, 2015
 *      Author: Wilbert Copeland
 */

#ifndef IINTEGRATOR_H_
#define IINTEGRATOR_H_

#include "rrLogger.h"
#include "rrOSSpecifics.h"
#include "Dictionary.h"
#include "tr1proxy/rr_memory.h"
#include <stdexcept>


namespace rr
{

	class IIntegrator;
	class SimulateOptions;
	class ExecutableModel;

	class IIntegratorListener
	{
	public:

		/**
		 * is called after the internal integrator completes each internal time step.
		 */
		virtual uint onTimeStep(IIntegrator* integrator, ExecutableModel* model, double time) = 0;

		/**
		 * whenever model event occurs and after it is procesed.
		 */
		virtual uint onEvent(IIntegrator* integrator, ExecutableModel* model, double time) = 0;

		virtual ~IIntegratorListener() {};
	};

	typedef cxx11_ns::shared_ptr<IIntegratorListener> IIntegratorListenerPtr;


	class RR_DECLSPEC IIntegrator
	{

	protected:
		typedef std::unordered_map<std::string, std::string> HintMap;
		typedef std::unordered_map<std::string, std::string> DescMap;

	public:
		inline IIntegrator(void) {};
		virtual ~IIntegrator(void) {};

		virtual std::string getIntegratorName() const = 0;
		virtual std::string getIntegratorDescription() const = 0;

		virtual std::string loadSettings() const = 0;
		virtual std::vector<std::string> getAllSettingNames() const = 0;
		
		virtual std::string getSettingValue(std::string optionName) const = 0;
		virtual void setSettingValue(std::string optionName, std::string desc) const = 0;
		
		virtual std::string getSettingDescription(std::string optionName) const = 0;
		virtual void setSettingDescription(std::string optionName, std::string desc) const = 0;

		virtual std::string getSettingHint(std::string optionName) const = 0;
		virtual void setSettingHint(std::string optionName, std::string hint) const = 0;

		virtual std::string getSettingType(std::string optionName) const = 0;

		virtual void setSimulateOptions(const SimulateOptions* options) = 0;
		virtual double integrate(double t0, double hstep) = 0;
		virtual void restart(double t0) = 0;
		virtual void setListener(IIntegratorListenerPtr) = 0;
		virtual IIntegratorListenerPtr getListener() = 0;
		virtual std::string toString() const = 0;
		virtual std::string toRepr() const = 0;

		/**
		 * A set of flags that are common to all integrators. The SimulateOptions::integratorFlags
		 * field is a bitmask that contains these values.
		 */
		enum IntegratorFlags
		{
			/**
			 * Is the model a stiff system? setting this to stiff causes
			 * RoadRunner to load a stiff solver which could potentially be
			 * extremly slow
			 */
			STIFF                   = (0x1 << 0), // => 0x00000001

			/**
			 * The MULTI_STEP option tells the solver to take a series of internal steps
			 * and then return the solution at the point reached by that step.
			 *
			 * In simulate, this option will likely be slower than normal mode,
			 * but may be useful to monitor solutions as they are integrated.
			 *
			 * This is intended to be used in combination with the
			 * IntegratorListener. It this option is set, and there is a
			 * IntegratorListener set, RoadRunner::integrate will run the
			 * integrator in a series of internal steps, and the listner
			 * will by notified at each step.
			 *
			 * Highly Experimental!!!
			 */
			MULTI_STEP                = (0x1 << 1), // => 0x00000010

			/**
			 * Perform a variable time step simulation. This will allow the
			 * integrator to best choose an adaptive time step and the resulting
			 * matrix will have a non-uniform time column
			 */
			VARIABLE_STEP             = (0x1 << 2) // => 0b00000100
		};

	protected:
		HintMap hints;
		DescMap descriptions;
	};

}

#endif /* IINTEGRATOR_H_ */
