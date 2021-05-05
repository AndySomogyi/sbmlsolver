//
// Created by Ciaran on 05/05/2021.
//

#include "SettingTestsSwigAPI.h"

rr::Setting SettingTestsSwigAPI::getStringTypeAsSetting() const {
    return rr::Setting(stringType);
}

void SettingTestsSwigAPI::setStringType(rr::Setting stringType) {
    SettingTestsSwigAPI::stringType = stringType;
}

rr::Setting SettingTestsSwigAPI::getBoolTypeAsSetting() const {
    return rr::Setting(boolType);
}

void SettingTestsSwigAPI::setBoolType(rr::Setting boolType) {
    SettingTestsSwigAPI::boolType = boolType;
}

rr::Setting SettingTestsSwigAPI::getInt32TypeAsSetting() const {
    return rr::Setting(int32Type);
}

void SettingTestsSwigAPI::setInt32Type(rr::Setting int32Type) {
    SettingTestsSwigAPI::int32Type = int32Type;
}

rr::Setting SettingTestsSwigAPI::getUint32TypeAsSetting() const {
    return rr::Setting(uint32Type);
}

void SettingTestsSwigAPI::setUint32Type(rr::Setting uint32Type) {
    SettingTestsSwigAPI::uint32Type = uint32Type;
}

rr::Setting SettingTestsSwigAPI::getInt64TypeAsSetting() const {
    return rr::Setting(int64Type);
}

void SettingTestsSwigAPI::setInt64Type(rr::Setting int64Type) {
    SettingTestsSwigAPI::int64Type = int64Type;
}

rr::Setting SettingTestsSwigAPI::getUint64TypeAsSetting() const {
    return rr::Setting(uint64Type);
}

void SettingTestsSwigAPI::setUint64Type(rr::Setting uint64Type) {
    SettingTestsSwigAPI::uint64Type = uint64Type;
}

rr::Setting SettingTestsSwigAPI::getFloatTypeAsSetting() const {
    return rr::Setting(floatType);
}

void SettingTestsSwigAPI::setFloatType(rr::Setting floatType) {
    SettingTestsSwigAPI::floatType = floatType;
}

rr::Setting SettingTestsSwigAPI::getDoubleTypeAsSetting() const {
    return rr::Setting(doubleType);
}

void SettingTestsSwigAPI::setDoubleType(rr::Setting doubleType) {
    SettingTestsSwigAPI::doubleType = doubleType;
}

rr::Setting SettingTestsSwigAPI::getCharTypeAsSetting() const {
    return rr::Setting(charType);
}

void SettingTestsSwigAPI::setCharType(rr::Setting charType) {
    SettingTestsSwigAPI::charType = charType;
}

rr::Setting SettingTestsSwigAPI::getUCharTypeAsSetting() const {
    return rr::Setting(uCharType);
}

void SettingTestsSwigAPI::setUCharType(rr::Setting uCharType) {
    SettingTestsSwigAPI::uCharType = uCharType;
}

rr::Setting SettingTestsSwigAPI::getDoubleVectorTypeAsSetting() const {
    return rr::Setting(doubleVectorType);
}

void SettingTestsSwigAPI::setDoubleVectorType(rr::Setting doubleVectorType) {
    SettingTestsSwigAPI::doubleVectorType = doubleVectorType;
}
