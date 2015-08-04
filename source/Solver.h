// == PREAMBLE ================================================

// * Licensed under the Apache License, Version 2.0; see README

// == FILEDOC =================================================

/** @file Solver.h
* @author ETS, WBC, JKM
* @date Sep 7, 2013
* @copyright Apache License, Version 2.0
* @brief Contains the base class for RoadRunner solvers
**/

# ifndef RR_SOLVER_H_
# define RR_SOLVER_H_

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

	class Solver;
	class ExecutableModel;


	/*-------------------------------------------------------------------------------------------
		Solver is an abstract base class that provides an interface to specific steady-state solver
		class implementations.
	---------------------------------------------------------------------------------------------*/
	class RR_DECLSPEC Solver
	{
	public:
		enum SolverMethod
		{
			SteadyState,
			Other
		};

		virtual ~Solver() {};

        virtual double solve(const std::vector<double>& yin) = 0;

		virtual void loadConfigSettings();
		virtual void loadSBMLSettings(const std::string& filename);
		virtual std::string getSolverName() const = 0;
		virtual std::string getSolverDescription() const = 0;
		virtual std::string getSolverHint() const = 0;
		virtual SolverMethod getSolverMethod() const = 0;
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
		virtual void setValue(std::string key, const Variant& value);
		const std::string& getHint(std::string key) const;
		const std::string& getDescription(std::string key) const;
		const Variant::TypeId getType(std::string key);;
		

	protected:
        typedef std::unordered_map<std::string, Variant> SettingsMap;
        typedef std::unordered_map<std::string, std::string> HintMap;
        typedef std::unordered_map<std::string, std::string> DescriptionMap;

		SettingsMap settings;
		HintMap hints;
		DescriptionMap descriptions;

		void addSetting(std::string name, Variant val, std::string hint, std::string description);
	};


	/*class IntegratorException : public std::runtime_error
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
	};*/

    /**
     * @author JKM, WBC
     * @brief Handles constructing an integrator and contains meta
     * information about it
     */
    class RR_DECLSPEC SolverRegistrar
    {
    protected:
        typedef Solver* (*SolverCtor)(ExecutableModel *model);
    public:
        virtual ~SolverRegistrar();

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
        virtual Solver* construct(ExecutableModel *model) const = 0;
    };

    /**
     * @author JKM, WBC
     * @brief Constructs new integrators
     * @details Implements the factory and singleton patterns.
     * Constructs a new integrator given the name (e.g. cvode, gillespie)
     * and returns a base pointer to @ref rr::Solver.
     */
    class RR_DECLSPEC SolverFactory
    {
    public:
        virtual ~SolverFactory();

        /**
         * @author JKM, WBC
         * @brief Constructs a new solver given the name
         * (e.g. cvode, gillespie)
         */
        Solver* New(std::string name, ExecutableModel *m) const;

        /**
         * @author JKM, WBC
         * @brief Registers a new solver with the factory
         * so that it can be constructed
         * @details Should be called at startup for new solvers.
         */
        void registerSolver(SolverRegistrar* i);

        /**
         * @author JKM, WBC
         * @brief Returns the singleton instance of the solver factory
         */
        static SolverFactory& getInstance();

        // ** Indexing *********************************************************

        std::size_t getNumSolvers() const;

		std::vector<std::string> getListSolverNames();

        std::string getSolverName(std::size_t n) const;

        std::string getSolverHint(std::size_t n) const;

        std::string getSolverDescription(std::size_t n) const;

    private:
        /**
         * @author JKM, WBC
         * @brief Prevents external instantiation
         */
        SolverFactory() {}
        typedef std::vector<SolverRegistrar*> SolverRegistrars;
        SolverRegistrars mRegisteredSolvers;
    };

}

# endif /* RR_SOLVER_H_ */
