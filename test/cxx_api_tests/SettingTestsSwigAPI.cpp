//
// Created by Ciaran on 05/05/2021.
//

#include "SettingTestsSwigAPI.h"

rr::Setting SettingTestsSwigAPI::getStringTypeAsSetting() const {
    return rr::Setting(stringType);
}

void SettingTestsSwigAPI::setStringType(rr::Setting stringType) {
    SettingTestsSwigAPI::stringType = stringType;
    updateSettingsMap();
}

rr::Setting SettingTestsSwigAPI::getBoolTypeAsSetting() const {
    return rr::Setting(boolType);
}

void SettingTestsSwigAPI::setBoolType(rr::Setting boolType) {
    SettingTestsSwigAPI::boolType = boolType;
    updateSettingsMap();
}

rr::Setting SettingTestsSwigAPI::getInt32TypeAsSetting() const {
    return rr::Setting(int32Type);
}

void SettingTestsSwigAPI::setInt32Type(rr::Setting int32Type) {
    SettingTestsSwigAPI::int32Type = int32Type;
    updateSettingsMap();
}

rr::Setting SettingTestsSwigAPI::getUint32TypeAsSetting() const {
    return rr::Setting(uint32Type);
}

void SettingTestsSwigAPI::setUint32Type(rr::Setting uint32Type) {
    SettingTestsSwigAPI::uint32Type = uint32Type;
    updateSettingsMap();
}

rr::Setting SettingTestsSwigAPI::getInt64TypeAsSetting() const {
    return rr::Setting(int64Type);
}

void SettingTestsSwigAPI::setInt64Type(rr::Setting int64Type) {
    SettingTestsSwigAPI::int64Type = int64Type;
    updateSettingsMap();
}

rr::Setting SettingTestsSwigAPI::getUint64TypeAsSetting() const {
    return rr::Setting(uint64Type);
}

void SettingTestsSwigAPI::setUint64Type(rr::Setting uint64Type) {
    SettingTestsSwigAPI::uint64Type = uint64Type;
    updateSettingsMap();
}

rr::Setting SettingTestsSwigAPI::getFloatTypeAsSetting() const {
    return rr::Setting(floatType);
}

void SettingTestsSwigAPI::setFloatType(rr::Setting floatType) {
    SettingTestsSwigAPI::floatType = floatType;
    updateSettingsMap();
}

rr::Setting SettingTestsSwigAPI::getDoubleTypeAsSetting() const {
    return rr::Setting(doubleType);
}

void SettingTestsSwigAPI::setDoubleType(rr::Setting doubleType) {
    SettingTestsSwigAPI::doubleType = doubleType;
    updateSettingsMap();
}

rr::Setting SettingTestsSwigAPI::getCharTypeAsSetting() const {
    return rr::Setting(charType);
}

void SettingTestsSwigAPI::setCharType(rr::Setting charType) {
    SettingTestsSwigAPI::charType = charType;
    updateSettingsMap();
}

rr::Setting SettingTestsSwigAPI::getUCharTypeAsSetting() const {
    return rr::Setting(uCharType);
}

void SettingTestsSwigAPI::setUCharType(rr::Setting uCharType) {
    SettingTestsSwigAPI::uCharType = uCharType;
    updateSettingsMap();
}

rr::Setting SettingTestsSwigAPI::getDoubleVectorTypeAsSetting() const {
    return rr::Setting(doubleVectorType);
}

void SettingTestsSwigAPI::setDoubleVectorType(rr::Setting doubleVectorType) {
    SettingTestsSwigAPI::doubleVectorType = doubleVectorType;
    updateSettingsMap();
}

/**
 * 1) Python user passes in a Python string, which
 *    std_string.i automatically converts into std::string.
 * 2) The std::string is used to collect the value
 *    from the settings map
 */
rr::Setting SettingTestsSwigAPI::getValueFromSettings(const std::string &key) {
    return settingsMap[key];
}

void SettingTestsSwigAPI::setValueInSettings(const std::string& key, rr::Setting value) {
    std::cout <<  "SettingTestsSwigAPI::setValueInSettings: key: " << key << "; settings value: " << value.toString() << std::endl;
    settingsMap[key] = std::move(value);
}

void SettingTestsSwigAPI::updateSettingsMap() {
    settingsMap["stringType"] = stringType;
    settingsMap["boolType"] = boolType;
    settingsMap["int32Type"] = int32Type;
    settingsMap["uint32Type"] = uint32Type;
    settingsMap["int64Type"] = int64Type;
    settingsMap["uint64Type"] = uint64Type;
    settingsMap["floatType"] = floatType;
    settingsMap["doubleType"] = doubleType;
    settingsMap["charType"] = charType;
    settingsMap["uCharType"] = uCharType;
    settingsMap["doubleVectorType"] = doubleVectorType;
}
