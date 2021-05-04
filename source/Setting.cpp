//
// Created by Ciaran on 30/04/2021.
//

#include "Setting.h"
#include <algorithm>
#include <utility>
#include "rrStringUtils.h"

namespace rr {

    Setting::Setting(setting_t value)
            : value_(std::move(setting_t(std::move(value)))) {};


    Setting::TypeId Setting::type() const {
        return (Setting::TypeId) value_.index();
    }

    bool Setting::isString() const {
        return type() == STRING;
    }

    bool Setting::isInteger() const {
        return type() == INT32 || type() == UINT32 || type() == INT64 || type() == UINT64;
    }

    bool Setting::isNumeric() const {
        return isInteger() || type() == FLOAT || type() == DOUBLE;
    }

    bool Setting::isBool() const {
        return type() == BOOL;
    }

    bool Setting::isEmpty() const {
        return type() == EMPTY;
    }

    bool Setting::isSigned() const {
        return type() == INT32 || type() == INT64 || type() == UCHAR;
    }

    bool Setting::isDoubleVector() const {
        return type() == DOUBLEVECTOR;
    }

    bool Setting::isDouble() const {
        return type() == DOUBLE;
    }

    std::string Setting::pythonRepr() const {
        if (isBool()) {
            if (get<bool>()) {
                return "True";
            } else {
                return "False";
            }
        } else if (isString()) {
            return "'" + get<std::string>() + "'";
        }
        return get<std::string>();
    }

    const setting_t &Setting::getValue() const {
        return value_;
    }

    Setting Setting::parse(std::string &s) {

        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::bind1st(std::not_equal_to<char>(), ' ')));
        s.erase(std::find_if(s.rbegin(), s.rend(), std::bind1st(std::not_equal_to<char>(), ' ')).base(), s.end());


        const char *input = s.c_str();
        char *end = nullptr;

        // check for int
        int i = (int) strtol(input, &end, 0);
        if (*input != '\0' && end != input && *end == '\0') {
            return Setting(i);
        }

        // check for double
        double d = strtod(input, &end);
        if (*input != '\0' && end != input && *end == '\0') {
            return Setting(d);
        }

        // check for bool
        std::string bstr = s;
        std::transform(bstr.begin(), bstr.end(), bstr.begin(), ::toupper);

        if (bstr == "TRUE") {
            return Setting(true);
        }

        if (bstr == "FALSE") {
            return Setting(false);
        }

        //Check if std::vector of doubles
        if (s[0] == '[') {
            return Setting(toDoubleVector(s));
        }

        // its a std::string
        return Setting(s);
    }

    Setting &Setting::operator=(Setting &setting) {
        if (this != &setting) {
            value_ = setting.getValue();
        };
        return *this;
    }

    Setting &Setting::operator=(Setting &&setting) noexcept {
        if (this != &setting) {
            value_ = setting.getValue();
        };
        return *this;
    }

    Setting::Setting(const Setting &setting)
            : value_(setting.getValue()) {}

    Setting::Setting(Setting &&setting) noexcept
            : value_(std::move(setting.getValue())) {}


}
