// == PREAMBLE ================================================

// * Licensed under the Apache License, Version 2.0; see README

// == FILEDOC =================================================

/** @file Integrator.h
* @author ETS, WBC, JKM
* @date Sep 7, 2013
* @copyright Apache License, Version 2.0
* @brief Contains the base class for RoadRunner integrators
**/

# ifndef RR_INTEGRATOR_H_
# define RR_INTEGRATOR_H_

// == INCLUDES ================================================

# include "rrLogger.h"
# include "rrOSSpecifics.h"
# include "Dictionary.h"
# include "rrException.h"

# include "tr1proxy/rr_memory.h"
# include <stdexcept>

// == CODE ====================================================

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
		virtual void loadSBMLSettings(const std::string& filename);
		virtual std::string getIntegratorName() const = 0;
		virtual std::string getIntegratorDescription() const = 0;
		virtual std::string getIntegratorHint() const = 0;
		virtual IntegrationMethod getIntegrationMethod() const = 0;
		std::vector<std::string> getSettings();

		virtual Variant getValue(std::string key);
		virtual int getValueAsInt(std::string key);
		virtual unsigned int getValueAsUInt(std::string key);
		virtual long getValueAsLong(std::string key);
		virtual unsigned long getValueAsULong(std::string key);
		virtual float getValueAsFloat(std::string key);
		virtual double getValueAsDouble(std::string key);
		virtual char getValueAsChar(std::string key);
		virtual unsigned char getValueAsUChar(std::string key);
		virtual std::string getValueAsString(std::string key);
		virtual bool getValueAsBool(std::string key);
		virtual void setValue(std::string key, const Variant& value) = 0;
		const std::string& getHint(std::string key) const;
		const std::string& getDescription(std::string key) const;
		const Variant::TypeId getType(std::string key);

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

    /**
     * @author JKM, WBC
     * @brief Handles constructing an integrator and contains meta
     * information about it
     */
    class RR_DECLSPEC IntegratorRegistrar
    {
    protected:
        typedef Integrator* (*IntegratorCtor)(ExecutableModel *model);
    public:
        virtual ~IntegratorRegistrar();

        /**
         * @author JKM, WBC
         * @brief Gets the name associated with this integrator type
         */
        virtual std::string getName() const = 0;

        /**
         * @author JKM, WBC
         * @brief Gets the description associated with this integrator type
         */
        virtual std::string getDescription() const = 0;

        /**
         * @author JKM, WBC
         * @brief Gets the hint associated with this integrator type
         */
        virtual std::string getHint() const = 0;

        /**
         * @author JKM, WBC
         * @brief Constructs a new integrator of a given type
         */
        virtual Integrator* construct(ExecutableModel *model) const = 0;
    };

    /**
     * @author JKM, WBC
     * @brief Constructs new integrators
     * @details Implements the factory and singleton patterns.
     * Constructs a new integrator given the name (e.g. cvode, gillespie)
     * and returns a base pointer to @ref rr::Integrator.
     */
    class RR_DECLSPEC IntegratorFactory
    {
    public:
        virtual ~IntegratorFactory();

        /**
         * @author JKM, WBC
         * @brief Constructs a new integrator given the name
         * (e.g. cvode, gillespie)
         */
        Integrator* New(std::string name, ExecutableModel *m) const;

        /**
         * @author JKM, WBC
         * @brief Registers a new integrator with the factory
         * so that it can be constructed
         * @details Should be called at startup for new integrators.
         */
        void registerIntegrator(IntegratorRegistrar* i);

        /**
         * @author JKM, WBC
         * @brief Returns the singleton instance of the integrator factory
         */
        static IntegratorFactory& getInstance();

        // ** Indexing *********************************************************

        std::size_t getNumIntegrators() const;

        std::string getIntegratorName(std::size_t n) const;

        std::string getIntegratorHint(std::size_t n) const;

        std::string getIntegratorDescription(std::size_t n) const;

    private:
        /**
         * @author JKM, WBC
         * @brief Prevents external instantiation
         */
        IntegratorFactory() {}
        typedef std::vector<IntegratorRegistrar*> IntegratorRegistrars;
        IntegratorRegistrars mRegisteredIntegrators;
    };

}

# endif /* RR_INTEGRATOR_H_ */
