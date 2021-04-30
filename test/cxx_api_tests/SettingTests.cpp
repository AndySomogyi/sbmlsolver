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


TEST_F(SettingTests, TestIsString) {
    Setting setting("CheeseCake");
    ASSERT_TRUE(setting.isString());
}

TEST_F(SettingTests, TestIsIntegerFromInt32) {
    Setting setting(std::int32_t(-1234));
    ASSERT_TRUE(setting.isInteger());
}
TEST_F(SettingTests, TestIsIntegerFromUInt32) {
    Setting setting(std::uint32_t(1234));
    ASSERT_TRUE(setting.isInteger());
}
TEST_F(SettingTests, TestIsIntegerFromInt64) {
    Setting setting(std::int64_t(-3577));
    ASSERT_TRUE(setting.isInteger());
}
TEST_F(SettingTests, TestIsIntegerFromUInt64) {
    Setting setting(std::uint64_t(12346));
    ASSERT_TRUE(setting.isInteger());
}
TEST_F(SettingTests, TestIsIntegerFromUnqualifiedInt) {
    Setting setting(1234);
    ASSERT_TRUE(setting.isInteger());
}

TEST_F(SettingTests, TestDoubleIsNotInteger) {
    Setting setting(0.1234);
    ASSERT_FALSE(setting.isInteger());
}


TEST_F(SettingTests, TestIsNumericInt32) {
    Setting setting(std::int32_t(-1234));
    ASSERT_TRUE(setting.isNumeric());
}
TEST_F(SettingTests, TestIsNumericUInt32) {
    Setting setting(std::uint32_t(1234));
    ASSERT_TRUE(setting.isNumeric());
}
TEST_F(SettingTests, TestIsNumericInt64) {
    Setting setting(std::int64_t(-3577));
    ASSERT_TRUE(setting.isNumeric());
}
TEST_F(SettingTests, TestIsNumericUInt64) {
    Setting setting(std::uint64_t(12346));
    ASSERT_TRUE(setting.isNumeric());
}

TEST_F(SettingTests,  TestIsNumericFloat) {
    Setting setting((float)0.1234);
    ASSERT_TRUE(setting.isNumeric());
}

TEST_F(SettingTests,  TestIsNumericDouble) {
    Setting setting((double)0.1234);
    ASSERT_TRUE(setting.isNumeric());
}
























