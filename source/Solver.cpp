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

#include "Solver.h"
#include "rrConfig.h"
#include <typeinfo>

// == CODE ====================================================

using namespace std;
namespace rr
{
    void Solver::addSetting(string name, Variant val, string display_name, string hint, string description)
    {
        sorted_settings.push_back(name);
        settings[name] = val;
        hints[name] = hint;
        descriptions[name] = description;
    }

    std::vector<string> Solver::getSettings() const
    {
        std::vector<string> keys;
        for (SettingsList::const_iterator i = sorted_settings.begin(); i != sorted_settings.end(); ++i)
        {
            keys.push_back(*i);
        }
        return keys;
    }

    void Solver::resetSettings()
    {
        sorted_settings.clear();
        settings.clear();
        hints.clear();
        descriptions.clear();
    }

    std::string Solver::getParamName(int n) const
    {
        if (sorted_settings.size() != settings.size())
            throw std::runtime_error("Setting count inconsistency");
        return sorted_settings.at(n);
    }

    std::string Solver::getParamHint(int n) const
    {
        return getHint(getParamName(n));
    }

    std::string Solver::getParamDesc(int n) const
    {
        return getDescription(getParamName(n));
    }

    Variant Solver::getValue(std::string key)
    {
        SettingsMap::const_iterator option = settings.find(key);
        if (option == settings.end())
        {
            throw std::invalid_argument("Solver::getValue: invalid key: " + key);
        }
        return option->second;
    }

    Variant Solver::hasValue(std::string key) const
    {
        return settings.find(key) != settings.end();
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
        if (settings.find(key) ==  settings.end())
            throw std::invalid_argument(getName() + " invalid key: " + key);
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
}