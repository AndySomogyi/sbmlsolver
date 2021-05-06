//
// Created by Ciaran on 05/05/2021.
//

#ifndef ROADRUNNER_SETTINGTESTSSWIGAPI_H
#define ROADRUNNER_SETTINGTESTSSWIGAPI_H

#include "Setting.h"

//using namespace rr; // swig complains about not knowing anything about namespace rr

/**
 * @brief A little class for using the
 * rr::Setting object in complete isolation
 * from the rest of roadrunner
 * @details The rr::Setting object is deliberately not
 * exposed at the Python level because we instead prefer
 * to extract the contents of a rr::Setting and convert them
 * to native Python objects instead. This poses a problem
 * for testing, because the rr::Setting can only be tested
 * indirectly. This object provides functions which use the
 * rr::Setting object. SettingTests.i provides the bindings
 * to turn this API into Pythonable code which can be tested
 * from Python.
 * The main aspect that needs testing is whether we can
 * convert a rr::Setting to the appropriate Python variable
 * and a Python variable back to a setting.
 * Our strategy is therefore to have one member variable
 * for each valid type on the C++ end with getter and setters.
 */
class SettingTestsSwigAPI {
public:
    SettingTestsSwigAPI() {
        updateSettingsMap(); // apply initial settings
    };

    rr::Setting getStringTypeAsSetting() const;

    void setStringType(rr::Setting stringType);

    rr::Setting getBoolTypeAsSetting() const;

    void setBoolType(rr::Setting boolType);

    rr::Setting getInt32TypeAsSetting() const;

    void setInt32Type(rr::Setting int32Type);

    rr::Setting getUint32TypeAsSetting() const;

    void setUint32Type(rr::Setting uint32Type);

    rr::Setting getInt64TypeAsSetting() const;

    void setInt64Type(rr::Setting int64Type);

    rr::Setting getUint64TypeAsSetting() const;

    void setUint64Type(rr::Setting uint64Type);

    rr::Setting getFloatTypeAsSetting() const;

    void setFloatType(rr::Setting floatType);

    rr::Setting getDoubleTypeAsSetting() const;

    void setDoubleType(rr::Setting doubleType);

    rr::Setting getCharTypeAsSetting() const;

    void setCharType(rr::Setting charType);

    rr::Setting getUCharTypeAsSetting() const;

    /**
     * Not supported. Taking a string from Python
     * string will always convert to std::string
     */
    void setUCharType(rr::Setting uCharType);

    rr::Setting getDoubleVectorTypeAsSetting() const;

    void setDoubleVectorType(rr::Setting doubleVectorType);

    rr::Setting getValueFromSettings(const std::string& key);

    void setValueInSettings(const std::string& key, rr::Setting value);


private:
    rr::Setting stringType = "I'm a string";
    rr::Setting boolType = true;
    rr::Setting int32Type = -1;
    rr::Setting uint32Type = 1;
    rr::Setting int64Type = -2;
    rr::Setting uint64Type = 2;
    rr::Setting floatType = 0.1234;
    rr::Setting doubleType = 0.5678;
    rr::Setting charType = 'c';
    rr::Setting uCharType = 'd';
    rr::Setting doubleVectorType = std::vector<double>{0.1, 0.2, 0.3};

    std::unordered_map<std::string, rr::Setting> settingsMap{};

    /**
     * Quite crude implementation for testing.
     * We call this method every time an
     * option changes with set* commands.
     */
    void updateSettingsMap();


};


#endif //ROADRUNNER_SETTINGTESTSSWIGAPI_H
