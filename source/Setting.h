//
// Created by Ciaran on 30/04/2021.
//

#ifndef ROADRUNNER_SETTING_H
#define ROADRUNNER_SETTING_H

#include <variant>
#include <iostream>
#include <vector>
#include <sstream>
#include <limits>

namespace rr {

    using setting_t = std::variant<
            std::monostate, /*void, see docs for std::variant*/
            std::string, bool, std::int32_t,
            std::uint32_t, std::int64_t, std::uint64_t,
            float, double,
            char, unsigned char,
            std::vector<double>>;


    /**
     * @brief Store a roadrunner option (or setting) as a Variant type.
     * @details Uses a std::variant to hold one of the following types
     *      , std::monostate, std::string, bool, std::int32_t, std::uint32_t,
     *      std::int64_t, std::uint64_t, float, double,
     *      char, unsigned char, std::vector<double>
     * @note for developers: order of the std::variant template specialization is important.
     * Make sure the order always matches the other of the Setting::TypeId enum.
     * @note The function of this class used to be performed
     * by rr::Variant. Since C++17 we have replaced this with a
     * standard type.
     *
     * @code
     *  // implicit type conversions
     *  Setting setting(4); // is an int
     *  int s = setting.get<int>(); // okay
     *  int t = setting.get<std::string>(); // not okay, setting contains an int
     *  auto u = setting.get_if<int>(); // u is pointer to int if setting is int else nullptr
     *  if (auto v = setting.get_if<int>()){
     *      // do something in context where `setting` is int
     *  }
     *  Setting setting2(-4); // contains an int with negative value
     *  int q = setting2; // okay
     *  long r = setting2; // okay
     *  unsigned int w = setting2; // error, setting2 contains negative value
     *  unsigned long x = setting2; // error, setting2 contains negative value
     *
     *  long biggest_int = std::numeric_limits<int>::max();
     *  long out_of_range = biggest_int * 10;
     *  Setting setting(out_of_range);
     *  int x = setting; // error, int isn't big enough to hold `out_out_range`
     *
     *  // equality works as expected
     *  Setting setting3(std::int64_t(12345678912345));
     *  Setting setting4(std::int64_t(12345678912345));
     *  ASSERT_TRUE(setting3 == setting4); // pass
     *  ASSERT_TRUE(setting3 == 12345678912345); // also okay
     *
     *  // reassign a setting to different type
     *  Setting setting5(123);
     *  setting5 = "Now a string"; // okay
     *  std::string expected = "Now a string";
     *  ASSERT_TRUE(setting5 == expected); // pass
     *
     *  // instantiating a setting from other objects
     *  std::string st = "A String";
     *  Setting setting6(st); //okay
     *  Setting setting7 = st ; // not okay
     *  Setting setting8 = "A string"; // not okay
     *
     *  // check if Setting supports a type
     *  bool supported = Setting::isValidType<int>(); // true
     *  supported = Setting::isValidType<std::string>(); // true
     *  supported = Setting::isValidType<const char*>(); // false
     * @endcode
     */
    class Setting {
    public:

        /**
         * @brief constructor to take any type
         * accepted by setting_t and store as
         * a variant.
         */
        explicit Setting(setting_t value);

//        template<class T>
//        explicit Setting(T value)
//                : value_(setting_t(value)) {};

        /**
         * @brief default constructor. Allows instantiating
         * of an empty setting. The empty setting will
         * have a type of std::monostate, which is index
         * 0 in setting_t
         */
        Setting() = default;


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
            DOUBLEVECTOR = 11,
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
         * @brief determines whether the current value of
         * Setting is of type T.
         * @returns true is Setting is of type T
         */
        template<class T>
        bool isType() {
            return std::holds_alternative<T>(value_);
        }

        template<class T>
        static bool isValidType() {
            return CheckValidVariantType<T, setting_t>();
        }

        /**
         * @brief get the value of this Setting
         * as type T if the value in this Setting is
         * of type T.
         * @details wrapper around std::get_if
         */
        template<class T>
        T *get_if() {
            checkValidType<T>();
            return std::get_if<T>(&value_);
        }

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
            return std::get<SettingType>(value_);
        }

        /**
         * @brief return the value held by this setting as type SettingType.
         * @details const version of SettingType::get()
         * @see SettingType::get()
         */
        template<class SettingType>
        SettingType get() const {
            return std::get<SettingType>(value_);
        }

        /**
         * @brief implicit type conversion support for the Setting class.
         * @details this method is responsible for enabling implicit conversion
         * between Setting and its supported types. Moreover, it enables
         * one to implicitly convert (say) a int to a long, in contexts
         * that make sense (as determined by std::is_convertible_v).
         * Some situations are explicitly prohibited from implicit type
         * conversion. These include
         *  - converting a negative int or long to unsigned int or long
         *  - converting a long that has a value > the largest possible value for int to int
         * @note for developers: clang-tidy will ask you to make this
         * explicit, but this method *must* not be explicit since its
         * entire purpose is implicit type conversion.
         * @note for developers: Any additions to types stored in a setting_t
         * *must* have appropriate unit tests added for the new types in
         * SettingTests.cpp.
         */
        template<typename T>
        operator T() const {
            return std::visit(
                    [&](auto const &val) {
                        if constexpr (std::is_convertible_v<decltype(val), T>) {
                            // if we have an int
                            if (auto intValue = std::get_if<int>(&value_)) {
                                // and its negative,
                                if (*intValue < 0) {
                                    // we cannot convert to unsigned int or unsigned long
                                    if (typeid(T) == typeid(unsigned int) || typeid(T) == typeid(unsigned long)) {
                                        throw std::bad_variant_access{};
                                        return T{};
                                    }
                                }
                            }

                            // if we have a long,
                            if (auto lValue = std::get_if<long>(&value_)) {
                                // and its negative, we cannot convert to unsigned int or unsigned long
                                if (*lValue < 0) {
                                    if (typeid(T) == typeid(unsigned int) || typeid(T) == typeid(unsigned long)) {
                                        throw std::bad_variant_access{};
                                        return T{}; // must be present
                                    }
                                }
                                // furthermore, if we have a long which has a value greater than
                                // that of int32 maximum, we have a problem and throw
                                if (*lValue > std::numeric_limits<int>::max()) {
                                    throw std::bad_variant_access{}; // has annoying private constructor so we can't add arguments
                                }
                            }
                            // if we have a double, with value greater than std::numeric_limits<float>::max
                            // and we try to convert to float, we have an error
                            if (auto lValue = std::get_if<float>(&value_)) {
                                if (*lValue > std::numeric_limits<float>::max()) {
                                    throw std::bad_variant_access{};
                                }
                            }
                            // if all is good, we construct a object of type T and return it.
                            return T(val);
                        } else {
                            // T is not supported conversion type from typeid(value_)
                            throw std::bad_variant_access{};
                            return T{}; // oddly enough, this *is* necessary
                        }
                    }, value_);
        }

        /**
         * @brief equality operatot for the simple case of
         * comparing this Setting against another Setting
         */
        bool operator==(const Setting &rhs) const;
//        bool operator==(Setting rhs) const;

        /**
         * @brief inequality operator for the simple case of
         * comparing this Setting against another Setting
         */
        bool operator!=(const Setting &rhs) const;
//        bool operator!=(Setting rhs) const;


         bool operator==(const std::monostate& other){ return equals<std::monostate>(other);};
         bool operator==(const std::string& other){ return equals<std::string>(other);};
         bool operator==(const bool& other){ return equals<bool>(other);};
         bool operator==(const std::int32_t& other){ return equals<std::int32_t>(other);};
         bool operator==(const std::uint32_t& other){ return equals<std::uint32_t>(other);};
         bool operator==(const std::int64_t& other){ return equals<std::int64_t>(other);};
         bool operator==(const std::uint64_t & other){ return equals<std::uint64_t>(other);};
         bool operator==(const float& other){ return equals<float>(other);};
         bool operator==(const double& other){ return equals<double>(other);};
         bool operator==(const char& other){ return equals<char>(other);};
         bool operator==(const unsigned char& other){ return equals<unsigned char>(other);};
         bool operator==(const std::vector<double>& other){ return equals<std::vector<double>>(other);};

         bool operator!=(const std::monostate& other){ return not_equals<std::monostate>(other);};
         bool operator!=(const std::string& other){ return not_equals<std::string>(other);};
         bool operator!=(const bool& other){ return not_equals<bool>(other);};
         bool operator!=(const std::int32_t& other){ return not_equals<std::int32_t>(other);};
         bool operator!=(const std::uint32_t& other){ return not_equals<std::uint32_t>(other);};
         bool operator!=(const std::int64_t& other){ return not_equals<std::int64_t>(other);};
         bool operator!=(const std::uint64_t & other){ return not_equals<std::uint64_t>(other);};
         bool operator!=(const float& other){ return not_equals<float>(other);};
         bool operator!=(const double& other){ return not_equals<double>(other);};
         bool operator!=(const char& other){ return not_equals<char>(other);};
         bool operator!=(const unsigned char& other){ return not_equals<unsigned char>(other);};
         bool operator!=(const std::vector<double>& other){ return not_equals<std::vector<double>>(other);};

//        template<class T>
//        bool operator==(const T &rhs) {
//            if (auto x = get_if<T>()) {
//                return *x == rhs;
//            } else {
//                return false;
//            }
//        }
//
//        template<class T>
//        bool operator!=(const T &rhs) {
//            return !(operator==<T>(rhs));
//        }

        /**
         * @brief throws std::invalid_argument if
         * type T is not supported by Setting.
         * @see setting_t
         */
        template<class T>
        void checkValidType() {
            if (!Setting::isValidType<T>()) {
                std::ostringstream err;
                err << "Setting does not support ";
                err << "type \"" << typeid(T).name() << "\"";
                std::invalid_argument(err.str());
            }
        }


        template<class T>
        Setting &operator=(T &setting) {
            checkValidType<T>();
            value_ = setting_t(setting);
            // if value contained by Setting is currently a T
//            if (auto t = get_if<T>()) { //
//                // then we can do regular self assignment check
//                if (*this != *t) {
//                    value_ = setting_t(setting);
//                }
//            } else { // otherwise self assignment check isn't needed anyway
//                value_ = setting_t(setting);
//            }
            return *this;
        }

        template<class T>
        Setting &operator=(T &&setting) noexcept {
            checkValidType<T>();
            value_ = std::move(setting_t(setting));
            // if value contained by Setting is currently a T
//            if (auto t = get_if<T>()) { //
//                // then we can do regular self assignment check
//                if (*this != *t) {
//                    value_ = std::move(setting_t(setting));
//                }
//            } else { // otherwise self assignment check isn't needed anyway
//                value_ = std::move(setting_t(setting));
//            }
            return *this;
        }

        /**
         * @brief Assignment operator for Setting
         * with another Setting
         */
        Setting &operator=(const Setting &setting);

        /**
         * @brief Move assignment operator for Setting
         * with another Setting
         */
        Setting &operator=(Setting &&setting) noexcept;

        /**
         * @brief Copy constructor for Setting
         * with another Setting
         */
        Setting(const Setting &setting);

        /**
         * @brief Move constructor for Setting
         * with another Setting
         */
        Setting(Setting &&setting) noexcept;

        /**
         * @brief getter for the std::variant_t underlying this
         * Setting.
         */
        const setting_t &getValue() const;

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

        /**
        * true if this is a double
        */
        bool isDouble() const;

        /**
         * Parses the std::string which must be in JSON format. This is a common
         * way to read a Variant from a file or create a new one from a std::string:
         * @code
         * Variant v = Variant::parse("0.123");
         * @endcode
         * @note this function is a tangent to the main Setting class.
         * We should instead have a JSON parser, is that's the function
         * we need.
         */
        static Setting parse(std::string &val);

    private:

        setting_t value_;


        /**
         * @brief Generic type checking mechanism
         * for membership of type T in variant ALL_T.
         * @details not exposed to user since we provide
         * the isValidType method which uses this under
         * the hood.
         * @code
         *  CheckValidVariantType<int, setting_t > truth;
         *  ASSERT_TRUE(truth); // pass
         * @endcode
         */
        template<typename T, typename ALL_T>
        struct CheckValidVariantType;

        template<typename T, typename... ALL_T>
        struct CheckValidVariantType<T, std::variant<ALL_T...>>
                : public std::disjunction<std::is_same<T, ALL_T>...> {
        };

        template <class T>
        constexpr bool equals(T other){
            if (auto x = get_if<T>()) {
                return *x == other;
            } else {
                return false;
            }
        }
        template <class T>
        constexpr bool not_equals(T other){
            return !(equals<T>(other));
        }

    };

}
#endif //ROADRUNNER_SETTING_H
