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
#include <iomanip>
#include <utility>

// == CODE ====================================================

namespace rr
{
    Solver::Solver(ExecutableModel* model)
        : mModel(model){}

    void Solver::addSetting(const std::string& name, const Variant& val, std::string display_name, std::string hint, std::string description)
    {
        sorted_settings.push_back(name);
        settings[name] = val;
        display_names_[name] = std::move(display_name);
        hints[name] = std::move(hint);
        descriptions[name] = std::move(description);
    }

    void Solver::updateSettings(Dictionary * inputSettings)
    {
        if (!inputSettings)
            return;
        const std::vector<std::string>& thisSolversSettings = getSettingsKeys();
        for (const auto& setting: thisSolversSettings){
            if (inputSettings->hasKey(setting)){
                setValue(setting, inputSettings->getItem(setting));
            }
        }
    }

    std::unordered_map<std::string, Variant>& Solver::getSettings()
    {
        return settings;
    }

    std::vector<std::string> Solver::getSettingsKeys() const
    {
        std::vector<std::string> keys;
        for (const auto & sorted_setting : sorted_settings)
        {
            keys.push_back(sorted_setting);
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

    size_t Solver::getNumParams() const
    {
        if (sorted_settings.size() != settings.size())
            throw std::runtime_error("Setting count inconsistency");
        return sorted_settings.size();
    }

    std::string Solver::getParamName(size_t n) const
    {
        if (sorted_settings.size() != settings.size())
            throw std::runtime_error("Setting count inconsistency");
        return sorted_settings.at(n);
    }

    std::string Solver::getParamDisplayName(int n) const
    {
        return getDisplayName(getParamName(n));
    }

    std::string Solver::getParamHint(int n) const
    {
        return getHint(getParamName(n));
    }

    std::string Solver::getParamDesc(int n) const
    {
        return getDescription(getParamName(n));
    }

    Variant Solver::getValue(const std::string& key) const
    {
        auto option = settings.find(key);
        if (option == settings.end())
        {
            throw std::invalid_argument("Solver::getValue: invalid key: " + key);
        }
        return option->second;
    }

    Variant Solver::hasValue(const std::string& key) const
    {
        return settings.find(key) != settings.end();
    }

    int Solver::getValueAsInt(const std::string& key)
    {
        return getValue(key).convert<int>();
    }

    unsigned int Solver::getValueAsUInt(const std::string& key)
    {
        return getValue(key).convert<unsigned int>();
    }

    long Solver::getValueAsLong(const std::string& key)
    {
        return getValue(key).convert<long>();
    }

    unsigned long Solver::getValueAsULong(const std::string& key)
    {
        return getValue(key).convert<unsigned long>();
    }

    float Solver::getValueAsFloat(const std::string& key)
    {
        return getValue(key).convert<float>();
    }

    double Solver::getValueAsDouble(const std::string& key)
    {
        return getValue(key).convert<double>();
    }

	std::vector<double> Solver::getValueAsDoubleVector(const std::string& key)
	{
		return getValue(key).convert< std::vector<double> >();
	}

    char Solver::getValueAsChar(const std::string& key)
    {
        return getValue(key).convert<char>();
    }

    unsigned char Solver::getValueAsUChar(const std::string& key)
    {
        return getValue(key).convert<unsigned char>();
    }

    std::string Solver::getValueAsString(const std::string& key)
    {
        return getValue(key).convert<std::string>();
    }

    bool Solver::getValueAsBool(const std::string& key)
    {
        return getValue(key).convert<bool>();
    }

    void Solver::setValue(const std::string& key, const Variant& value)
    {
        if (settings.find(key) ==  settings.end())
            throw std::invalid_argument(getName() + " invalid key: " + key);
        settings[key] = value;
    }


    const std::string& Solver::getDisplayName(const std::string& key) const
    {
        auto option = Solver::display_names_.find(key);
        if (option == display_names_.end())
        {
            throw std::invalid_argument("invalid key: " + key);
        }
        return option->second;
    }

    const std::string& Solver::getHint(const std::string& key) const
    {
        auto option = Solver::hints.find(key);
        if (option == hints.end())
        {
            throw std::invalid_argument("invalid key: " + key);
        }
        return option->second;
    }

    const std::string& Solver::getDescription(const std::string& key) const
    {
        auto option = Solver::descriptions.find(key);
        if (option == descriptions.end())
        {
            throw std::invalid_argument("invalid key: " + key);
        }
        return option->second;
    }

    Variant::TypeId Solver::getType(const std::string& key) const
    {
        return getValue(key).type();
    }

    std::string Solver::getSettingsRepr() const
    {
        std::stringstream ss;
        for(size_t n=0; n<getNumParams(); ++n)
            ss << "    " << std::setw(20) << getParamName(n) << ": " << getValue(getParamName(n)).toString() << "\n";
        return ss.str();
    }

    std::string Solver::settingsPyDictRepr() const
    {
        std::stringstream ss;
        for(size_t n=0; n<getNumParams(); ++n)
            ss << (n ? ", " : "") << "'" << getParamName(n) << "': " << getValue(getParamName(n)).pythonRepr();
        return ss.str();
    }

    std::string Solver::toString() const
    {
        std::stringstream ss;
        ss << "< roadrunner.Solver() >\n";
        ss << "  name: " << getName() << "\n";
        ss << "  settings:\n";
        ss << getSettingsRepr();
        return ss.str();
    }

    std::string Solver::toRepr() const
    {
        std::stringstream ss;
        ss << "< roadrunner.Solver() \"" << getName() << "\" " << settingsPyDictRepr() << " >\n";
        return ss.str();
    }

    ExecutableModel *Solver::getModel() const {
        if (!mModel){
            throw NullPointerException("Solver::getModel(): mModel pointer is null");
        }
        return mModel;
    }
}


