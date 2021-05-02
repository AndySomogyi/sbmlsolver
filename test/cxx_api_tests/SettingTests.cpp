//
// Created by Ciaran on 30/04/2021.
//

#include "gtest/gtest.h"
#include "Setting.h"

using namespace rr;

class SettingTests : public ::testing::Test {
public:
    SettingTests() = default;

};

/******************************************************
 * Tests for storage of types used in Roadrunner
 */


TEST_F(SettingTests, TestSettingTypeMonostate) {
    Setting setting;
    ASSERT_EQ(setting.type(), Setting::TypeId::EMPTY);
}

TEST_F(SettingTests, TestSettingTypeString) {
    Setting setting("Really high");
    ASSERT_EQ(setting.type(), Setting::TypeId::STRING);

}

TEST_F(SettingTests, TestSettingTypeBool) {
    Setting setting(true);
    ASSERT_EQ(setting.type(), Setting::TypeId::BOOL);
}

TEST_F(SettingTests, TestSettingTypeInt32_t) {
    Setting setting(std::int32_t(-354));
    ASSERT_EQ(setting.type(), Setting::TypeId::INT32);
}

TEST_F(SettingTests, TestSettingTypeUint32_t) {
    Setting setting(std::uint32_t(1234));
    ASSERT_EQ(setting.type(), Setting::TypeId::UINT32);
}

TEST_F(SettingTests, TestSettingTypeInt64_t) {
    Setting setting(std::int64_t(-4512365));
    ASSERT_EQ(setting.type(), Setting::TypeId::INT64);
}

TEST_F(SettingTests, TestSettingTypeUint64_t) {
    Setting setting(std::uint64_t(24565434));
    ASSERT_EQ(setting.type(), Setting::TypeId::UINT64);
}

TEST_F(SettingTests, TestSettingTypeFloat) {
    Setting setting(float(0.12346));
    ASSERT_EQ(setting.type(), Setting::TypeId::FLOAT);
}

TEST_F(SettingTests, TestSettingTypeDouble) {
    Setting setting(double(0.1324688));
    ASSERT_EQ(setting.type(), Setting::TypeId::DOUBLE);
}

TEST_F(SettingTests, TestSettingTypeChar) {
    Setting setting('c');
    ASSERT_EQ(setting.type(), Setting::TypeId::CHAR);
}

TEST_F(SettingTests, TestSettingTypeUnsignedChar) {
    unsigned char x = 'x';
    Setting setting(x);
    ASSERT_EQ(setting.type(), Setting::TypeId::UCHAR);
}

TEST_F(SettingTests, TestSettingTypeDoubleVector) {
    Setting setting(std::vector<double>({0.1, 0.2, 0.3}));
    ASSERT_EQ(setting.type(), Setting::TypeId::DOUBLEVECTOR);
}


/******************************************************
 * Implicit cast tests, but the simple case.
 * Extracting an int from the setting for instance,
 * and no int to long.
 */

TEST_F(SettingTests, TestImplicitCastToMonostate) {
    Setting setting;
    std::monostate x = setting;
    ASSERT_EQ(typeid(x), typeid(std::monostate));
}

TEST_F(SettingTests, TestImplicitCastToString) {
    Setting setting("Really high");
    std::string x = setting;
    ASSERT_EQ(typeid(x), typeid(std::string));
}

TEST_F(SettingTests, TestImplicitCastToStringAndDoSomethingWithIt) {
    Setting setting("Really high");
    std::string x = setting;
    bool truth = x.find("Really", 0) != std::string::npos;
    ASSERT_TRUE(truth);
}


TEST_F(SettingTests, TestImplicitCastToBool) {
    Setting setting(true);
    bool x = setting;
    ASSERT_EQ(typeid(x), typeid(bool));
}

TEST_F(SettingTests, TestImplicitCastToInt32_t) {
    Setting setting(std::int32_t(-354));
    std::int32_t x = setting;
    ASSERT_EQ(typeid(x), typeid(std::int32_t));
}

TEST_F(SettingTests, TestImplicitCastToUint32_t) {
    Setting setting(std::uint32_t(1234));
    std::uint32_t x = setting;
    ASSERT_EQ(typeid(x), typeid(std::uint32_t));
}

TEST_F(SettingTests, TestImplicitCastToInt64_t) {
    Setting setting(std::int64_t(-451236));
    std::int64_t x = setting;
    ASSERT_EQ(typeid(x), typeid(std::int64_t));
}

TEST_F(SettingTests, TestImplicitCastToUint64_t) {
    Setting setting(std::uint64_t(2456543));
    std::uint64_t x = setting;
    ASSERT_EQ(typeid(x), typeid(std::uint64_t));
}

TEST_F(SettingTests, TestImplicitCastToFloat) {
    Setting setting(float(0.12346));
    float x = setting;
    ASSERT_EQ(typeid(x), typeid(float));
}

TEST_F(SettingTests, TestImplicitCastToDouble) {
    Setting setting(double(0.1324688));
    double x = setting;
    ASSERT_EQ(typeid(x), typeid(double));
}

TEST_F(SettingTests, TestImplicitCastToChar) {
    Setting setting('c');
    char x = setting;
    ASSERT_EQ(typeid(x), typeid(char));
}

TEST_F(SettingTests, TestImplicitCastToUnsignedChar) {
    unsigned char y = 'y';
    Setting setting(y);
    unsigned char x = setting;
    ASSERT_EQ(typeid(x), typeid(unsigned char));
}

TEST_F(SettingTests, TestImplicitCastToDoubleVector) {
    Setting setting(std::vector<double>({0.123, 0.456, 0.789}));
    std::vector<double> x = setting;
    ASSERT_EQ(typeid(x), typeid(std::vector<double>));
}

/*********************************************************
 * Conversions between relevant different types.
 * We do not test all combinations of type pairs,
 * only the ones which are supposed to work.
 * All other pairs, i.e. incompatible pairs like int to string
 * are not tested and should throw std::bad_variant_access.
 */

TEST_F(SettingTests, ImplicitCastIntToUInt) {
    Setting setting(4);
    unsigned int x = setting;
    ASSERT_EQ(typeid(x), typeid(unsigned int));
}

TEST_F(SettingTests, ImplicitCastIntToLong) {
    Setting setting(4);
    long x = setting;
    ASSERT_EQ(typeid(x), typeid(long));
}

TEST_F(SettingTests, ImplicitCastIntToULong) {
    Setting setting(4);
    unsigned long x = setting;
    ASSERT_EQ(typeid(x), typeid(unsigned long));
}

TEST_F(SettingTests, ImplicitCastNegativeIntToUInt) {
    // should raise error
    Setting setting(-4);
    ASSERT_THROW(
            unsigned int x = setting,
            std::bad_variant_access
    );
}

TEST_F(SettingTests, ImplicitCastNegativeIntToULong) {
    // should raise error
    Setting setting(-4);
    ASSERT_THROW(
            unsigned long x = setting,
            std::bad_variant_access
    );
}

TEST_F(SettingTests, ImplicitCastLongToIntOutOfBounds) {
    // should raise error
    long biggest_int = std::numeric_limits<int>::max();
    long out_of_range = biggest_int * 10;
    Setting setting(out_of_range);
    ASSERT_THROW(
            int x = setting,
            std::bad_variant_access
    );
}

TEST_F(SettingTests, ImplicitCastLongToInt) {
    Setting setting(std::int64_t(4));
    int x = setting;
    ASSERT_EQ(typeid(x), typeid(int));
}

TEST_F(SettingTests, ImplicitCastLongToUInt) {
    Setting setting(std::int64_t(4));
    unsigned int x = setting;
    ASSERT_EQ(typeid(x), typeid(unsigned int));
}

TEST_F(SettingTests, ImplicitCastLongToULong) {
    Setting setting(std::int64_t(4));
    unsigned long x = setting;
    ASSERT_EQ(typeid(x), typeid(unsigned long));
}

TEST_F(SettingTests, ImplicitCastStringToConstCharStar) {
    Setting setting(std::int64_t(4));
    unsigned long x = setting;
    ASSERT_EQ(typeid(x), typeid(unsigned long));
}

TEST_F(SettingTests, GetIfTestWithString){
    Setting setting("I'm a string");
    auto string = setting.get_if<std::string>();
    ASSERT_STREQ(string->c_str(), "I'm a string");
}

TEST_F(SettingTests, GetIfTestWithInt){
    Setting setting(1234);
    auto val = setting.get_if<int>();
    ASSERT_EQ(*val, 1234);
}

TEST_F(SettingTests, GetIfWhenValueNotRightType){
    Setting setting(1234);
    auto val = setting.get_if<long>();
    ASSERT_EQ(val, nullptr);
}

TEST_F(SettingTests, PutSettingInAVectorAndEqualityOp) {
    Setting setting("I'm a string");
    std::vector<Setting> v({setting});
    Setting x = v[0];
    ASSERT_EQ(x, setting);
}

TEST_F(SettingTests, ImplicitCastFromSettingInAVector) {
    Setting setting("I'm a string");
    std::vector<Setting> v({setting});
    std::string x = v[0];
    ASSERT_EQ(x, "I'm a string");
}


TEST_F(SettingTests, EqualityOperatorWithStrings) {
    Setting setting1("I'm a setting");
    Setting setting2("I'm a setting");
    ASSERT_EQ(setting1, setting2);
}

TEST_F(SettingTests, InEqualityOperatorWithStrings) {
    Setting setting1("I'm a setting");
    Setting setting2("I'm another setting");
    ASSERT_NE(setting1, setting2);
}

/**
 * Couldnt template the equality operators
 * because doing so worked for all necessary types
 * except for the Setting itself, since it is not
 * a type of setting_t
 */
TEST_F(SettingTests, EqualityOpWithString) {
    Setting setting("I'm a setting");
    std::string x = "I'm a setting";
    ASSERT_TRUE(setting == x);
}

//TEST_F(SettingTests, InEqualityOpWithString) {
//    Setting setting("I'm a setting");
//    std::string x = "I'm not a setting";
//    ASSERT_FALSE(setting == x);
//}

//TEST_F(SettingTests, EqualityWithTemporaryLong) {
//    Setting setting(std::int64_t(12345678912345));
//    ASSERT_TRUE(setting == 12345678912345 );
//}
//
//
//TEST_F(SettingTests, EqualityOperatorWithInt) {
//    Setting setting(345);
//    int x = 345;
//    ASSERT_TRUE(setting == x);
//}

//TEST_F(SettingTests, InEqualityOperatorWithInt) {
//    Setting setting(345);
//    int x = 3456;
//    ASSERT_FALSE(setting == x);
//}

//TEST_F(SettingTests, AssignmentOperator) {
//    Setting setting(345);
//    setting = std::string("string");
//    std::string expected = "string";
//    ASSERT_TRUE(setting == expected);
//}

TEST_F(SettingTests, CheckIsTypeWhenTrue) {
    Setting setting(345);
    bool x = setting.isType<int>();
    ASSERT_TRUE(x);
}
TEST_F(SettingTests, CheckIsTypeWhenFalse) {
    Setting setting(345);
    bool x = setting.isType<std::vector<double>>();
    ASSERT_FALSE(x);
}

TEST_F(SettingTests, CheckTypeIntIsValid) {
    bool truth = Setting::isValidType<int >() ;
    ASSERT_TRUE(truth);
}
TEST_F(SettingTests, CheckTypeConstCharStarIsInvalid) {
    bool truth = Setting::isValidType<const char* >() ;
    ASSERT_FALSE(truth);
}



TEST_F(SettingTests, AssignmentOperatorMonostateToBool) {
    Setting setting;
    setting = false;
    ASSERT_FALSE(setting);
}

TEST_F(SettingTests, AssignmentOperatorMonostateToDouble) {
    Setting setting;
    setting = 0.12345;
    ASSERT_EQ(setting.get<double>(), 0.12345);
}
TEST_F(SettingTests, AssignmentOperatorMonoToFalse) {
    Setting setting;
    setting = false;
    ASSERT_FALSE(setting);
}





















