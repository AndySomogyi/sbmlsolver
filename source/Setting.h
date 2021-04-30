//
// Created by Ciaran on 30/04/2021.
//

#ifndef ROADRUNNER_SETTING_H
#define ROADRUNNER_SETTING_H

#include <variant>
#include <iostream>
#include <vector>

namespace rr {

    using setting_t = std::variant<
            std::monostate, /*void, see docs for std::variant*/
            std::string, bool, std::int32_t,
            std::uint32_t, std::int64_t, std::uint64_t,
            float, double, char, unsigned char,
            std::vector<double>>;

    /**
     * @brief Store a roadrunner option (or setting).
     * @note for developers: order of the std::variant template specialization is important.
     * Make sure the order always matches the other of the Setting::TypeId enum.
     * @note The function of this class used to be performed
     * by rr::Variant. Since C++17 we can replace this with a
     * standard type.
     * @details Uses a std::variant to hold one of the following types
     *      , std::monostate, std::string, bool, std::int32_t, std::uint32_t,
     *      std::int64_t, std::uint64_t, float, double,
     *      char, unsigned char, std::vector<double>
     */
    class Setting : public setting_t {
    public:

        /**
         * @brief takes constructors from superclass std::variant
         */
        using variant::variant;

        /**
         * @brief types that correspond to the index of the
         * position of the type in the variant template.
         * i.e. std::string is index 0.
         */
        enum TypeId {
            EMPTY = 0, // std::monostate. Empty is default when variant instantiated with nothing
            STRING = 1,
            BOOL = 2,
            INT32 = 3,
            UINT32 = 4,
            INT64 = 5,
            UINT64 = 6,
            FLOAT = 7,
            DOUBLE = 8,
            CHAR = 9,
            UCHAR = 10,
            DOUBLEVECTOR = 11
        };

        /**
         * @brief returns the type of std::variant
         * contained within this Setting.
         * @returns an integer which refers to the index
         * of the type contained in this Setting. The index
         * is resolved by looking at the TypeId enum (or indeed
         * the ordering of the type parameters to setting_t.
         */
        [[nodiscard]] TypeId type() const;

        /**
         * @brief return the value held by this Setting
         * as a type SettingType.
         * @details simple wrapper around std::get.
         * @note for developers. This is a replacement for rr::Variant::convert in legacy
         * roadrunner. The name "get" is preferred to "convert" since it
         * follows C++ standard conventions.
         */
        template<class SettingType>
        SettingType get() {
            return std::get<SettingType>(*this);
        }

        /**
         * @brief return the value held by this setting as type SettingType.
         * @details const version of SettingType::get()
         * @see SettingType::get()
         */
        template<class SettingType>
        SettingType get() const {
            return std::get<SettingType>(*this);
        }

        /**
         * Parses the std::string which must be in JSON format. This is a common
         * way to read a Variant from a file or create a new one from a std::string:
         * @code
         * Variant v = Variant::parse("0.123");
         * @endcode
         */
        static Setting parse(std::string &val);

        /**
         * conversion operators, note, MSVC 2012 is buggy and does not support
         * templated conversion operators correctly, so manually implement
         * the supported types here.
         */
#define SETTING_IMPLICIT_CONVERT(type)       \
            operator type() const                    \
            {                                        \
                return get<type>();              \
            }

        /**
         * @brief Implicit type conversion from Setting to std::string
         */
        SETTING_IMPLICIT_CONVERT(std::string);
        /**
         * @brief Implicit type conversion from Setting to bool
         */
        SETTING_IMPLICIT_CONVERT(bool);
        /**
         * @brief Implicit type conversion from Setting to std::int32_t
         */
        SETTING_IMPLICIT_CONVERT(std::int32_t);
        /**
         * @brief Implicit type conversion from Setting to std::uint32_t
         */
        SETTING_IMPLICIT_CONVERT(std::uint32_t);
        /**
         * @brief Implicit type conversion from Setting to std::int64_t
         */
        SETTING_IMPLICIT_CONVERT(std::int64_t);
        /**
         * @brief Implicit type conversion from Setting to std::uint64_t
         */
        SETTING_IMPLICIT_CONVERT(std::uint64_t);
        /**
         * @brief Implicit type conversion from Setting to float
         */
        SETTING_IMPLICIT_CONVERT(float);
        /**
         * @brief Implicit type conversion from Setting to double
         */
        SETTING_IMPLICIT_CONVERT(double);
        /**
         * @brief Implicit type conversion from Setting to char
         */
        SETTING_IMPLICIT_CONVERT(char);
        /**
         * @brief Implicit type conversion from Setting to unsigned char
         */
        SETTING_IMPLICIT_CONVERT(unsigned char);
        /**
         * @brief Implicit type conversion from Setting to std::monostate
         * (void substitute, see C++ docs on variant)
         */
        SETTING_IMPLICIT_CONVERT(std::monostate);
        /**
         * @brief Implicit type conversion from Setting to std::vector<double>
         */
        SETTING_IMPLICIT_CONVERT(std::vector<double>);

        /**
         * @author JKM
         * @brief Convert to Python-compatible representation
         */
        [[nodiscard]] std::string pythonRepr() const;


        /**
         * @brief is this variant a std::string.
         */
        [[nodiscard]] bool isString() const;

        /**
         * @brief was an integer stored here.
         */
        [[nodiscard]] bool isInteger() const;

        /**
         * @brief is this a numeric type.
         */
        [[nodiscard]] bool isNumeric() const;

        /**
         * @brief is this a boolean type.
         */
        [[nodiscard]] bool isBool() const;

        /**
         * @brief true if empty.
         */
        bool isEmpty() const;

        /**
         * @brief true if this is a signed number.
         */
        bool isSigned() const;

        /**
        * true if this is a std::vector of doubles
        */
        bool isDoubleVector() const;

    };

}
#endif //ROADRUNNER_SETTING_H
