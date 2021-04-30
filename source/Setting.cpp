//
// Created by Ciaran on 30/04/2021.
//

#include "Setting.h"
#include <algorithm>
#include "rrStringUtils.h"

namespace rr {


    Setting::TypeId Setting::type() const {
        return (Setting::TypeId) index();
    }

    /**
     * @brief removes leading and trailing white space.
     * The input string is modified inplace.
     */
    static std::string strip(std::string in) {
        // removes leading white space.
        in.erase(in.begin(), std::find_if(in.begin(), in.end(), std::bind1st(std::not_equal_to<char>(), ' ')));
        in.erase(std::find_if(in.rbegin(), in.rend(), std::bind1st(std::not_equal_to<char>(), ' ')).base(), in.end());
        return in;
    }

    Setting Setting::parse(std::string &s) {

        std::string str = strip(s);

        const char *input = str.c_str();
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
        std::string bstr = str;
        std::transform(bstr.begin(), bstr.end(), bstr.begin(), ::toupper);

        if (bstr == "TRUE") {
            return Setting(true);
        }

        if (bstr == "FALSE") {
            return Setting(false);
        }

        //Check if std::vector of doubles
        if (str[0] == '[') {
            return Setting(toDoubleVector(str));
        }

        // its a std::string
        return Setting(str);
    }

    bool Setting::isString() const {
        return type() == STRING;
    }

    bool Setting::isInteger() const {
        return type() == INT32 || type() == UINT32 || type() == INT64 || type() == UINT64;
    }

    bool Setting::isNumeric() const {
        return isInteger() || type() == FLOAT ||type() == DOUBLE;
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


}
