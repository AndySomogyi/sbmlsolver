// == PREAMBLE ================================================

// * Licensed under the Apache License, Version 2.0; see README

// == FILEDOC =================================================

/** @file Solver.h
* @author JKM
* @date 09/01/2015
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
# include "tr1proxy/rr_unordered_map.h"
# include <stdexcept>

// == CODE ====================================================

namespace rr
{
    /**
     * @author JKM
     * @brief Base class for all integrators and steady state solvers
     */
    class RR_DECLSPEC Solver
    {
    public:

        virtual ~Solver() {};

        /**
        * @author JKM
        * @brief Get the name of this solver
        */
        virtual std::string getName() const = 0;

        /**
        * @author JKM
        * @brief Get the description of this solver
        */
        virtual std::string getDescription() const = 0;

        /**
        * @author JKM
        * @brief Get a (user-readable) hint for this solver
        */
        virtual std::string getHint() const = 0;

        /**
        * @author JKM
        * @brief Get a list of all settings for this solver
        */
        std::vector<std::string> getSettings() const;

        /**
        * @author JKM
        * @brief Reset all settings to their respective default values
        */
        virtual void resetSettings();

        /**
        * @author JKM, WBC
        * @brief Get the value of an integrator setting
        * @note Use one of the type-concrete versions like @ref getValueAsInt
        * to avoid type conversion gotchas
        */
        virtual Variant getValue(std::string key);

        /**
        * @author JKM
        * @brief Return true if this setting is supported by the integrator
        */
        virtual Variant hasValue(std::string key) const;

        /**
        * @author JKM
        * @brief Get the name of the parameter at index n (stored in an
        * unordered container)
        */
        virtual std::string getParamName(int n) const;

        /**
        * @author JKM
        * @brief Get the hint of the parameter at index n (stored in an
        * unordered container)
        */
        virtual std::string getParamHint(int n) const;

        /**
        * @author JKM
        * @brief Get the description of the parameter at index n (stored in an
        * unordered container)
        */
        virtual std::string getParamDesc(int n) const;


        /**
        * @author WBC, JKM
        * @brief Wrapper for @ref getValue which converts output to a specific type
        */
        virtual int getValueAsInt(std::string key);

        /**
        * @author WBC, JKM
        * @brief Wrapper for @ref getValue which converts output to a specific type
        */
        virtual unsigned int getValueAsUInt(std::string key);

        /**
        * @author WBC, JKM
        * @brief Wrapper for @ref getValue which converts output to a specific type
        */
        virtual long getValueAsLong(std::string key);

        /**
        * @author WBC, JKM
        * @brief Wrapper for @ref getValue which converts output to a specific type
        */
        virtual unsigned long getValueAsULong(std::string key);

        /**
        * @author WBC, JKM
        * @brief Wrapper for @ref getValue which converts output to a specific type
        */
        virtual float getValueAsFloat(std::string key);

        /**
        * @author WBC, JKM
        * @brief Wrapper for @ref getValue which converts output to a specific type
        */
        virtual double getValueAsDouble(std::string key);

        /**
        * @author WBC, JKM
        * @brief Wrapper for @ref getValue which converts output to a specific type
        */
        virtual char getValueAsChar(std::string key);

        /**
        * @author WBC, JKM
        * @brief Wrapper for @ref getValue which converts output to a specific type
        */
        virtual unsigned char getValueAsUChar(std::string key);

        /**
        * @author WBC, JKM
        * @brief Wrapper for @ref getValue which converts output to a specific type
        */
        virtual std::string getValueAsString(std::string key);

        /**
        * @author WBC, JKM
        * @brief Wrapper for @ref getValue which converts output to a specific type
        */
        virtual bool getValueAsBool(std::string key);


        virtual void setValue(std::string key, const Variant& value);
        const std::string& getHint(std::string key) const;
        const std::string& getDescription(std::string key) const;
        const Variant::TypeId getType(std::string key);

    protected:
        typedef std::vector<std::string> SettingsList;
        typedef RR_UNORDERED_MAP <std::string, Variant> SettingsMap;
        typedef RR_UNORDERED_MAP <std::string, std::string> HintMap;
        typedef RR_UNORDERED_MAP <std::string, std::string> DescriptionMap;

        SettingsList sorted_settings;
        SettingsMap settings;
        HintMap hints;
        DescriptionMap descriptions;

        void addSetting(std::string name, Variant val, std::string hint, std::string description);
    };

}

# endif /* RR_INTEGRATOR_H_ */
