//
// Created by Ciaran on 07/05/2021.
//

#include "gtest/gtest.h"

// todo move test model factor up one directory
#include "RoadRunnerTest.h"
#include "TestModelFactory.h"

#include "rrRoadRunner.h"
#include "rrConfig.h"

using namespace rr;

/**
 * This is a more of a stub test suite
 * than a full test suite at the moment.
 * It will eventually fully test every
 * aspect of the rr::RoadRunner API
 * For now, it only tests the things
 * that need a new test to fix a bug.
 */

class RoadRunnerAPITests : public RoadRunnerTest {

public:
    OpenLinearFlux openLinearFlux;

    RoadRunnerAPITests() = default;
};

TEST_F(RoadRunnerAPITests, DefaultJacobianMode) {
    Setting x = Config::getValue(
            Config::ROADRUNNER_JACOBIAN_MODE
    );
    ASSERT_TRUE(x.get<int>() == Config::ROADRUNNER_JACOBIAN_MODE_CONCENTRATIONS);
}

TEST_F(RoadRunnerAPITests, SetJacobianModeToAmt) {
    /**
     * Implicit type conversion has got us into a spot of trouble here.
     * The Config keys are being implicitly converted into an int,
     * rather than their original type, an unsigned int.
     */
    Config::setValue(Config::ROADRUNNER_JACOBIAN_MODE, Config::ROADRUNNER_JACOBIAN_MODE_AMOUNTS);
    Setting x = Config::getValue(
            Config::ROADRUNNER_JACOBIAN_MODE
    );
    ASSERT_TRUE(x.get<int>() == Config::ROADRUNNER_JACOBIAN_MODE_AMOUNTS);
}


TEST_F(RoadRunnerAPITests, GetFullJacobianDefaultConfigSettings) {
    RoadRunner rr(openLinearFlux.str());
    auto matrix = rr.getFullJacobian();
    // no modification of roadrunner Config
    checkMatrixEqual(matrix, openLinearFlux.fullJacobianConc());
}

TEST_F(RoadRunnerAPITests, GetFullJacobianUsingConcMode) {
    Config::setValue(Config::ROADRUNNER_JACOBIAN_MODE, Config::ROADRUNNER_JACOBIAN_MODE_CONCENTRATIONS);
    RoadRunner rr(openLinearFlux.str());
    auto matrix = rr.getFullJacobian();
    checkMatrixEqual(matrix, openLinearFlux.fullJacobianConc());
}

TEST_F(RoadRunnerAPITests, GetFullJacobianUsingAmtMode) {
    Config::setValue(Config::ROADRUNNER_JACOBIAN_MODE, Config::ROADRUNNER_JACOBIAN_MODE_AMOUNTS);
    RoadRunner rr(openLinearFlux.str());
    auto matrix = rr.getFullJacobian();
    checkMatrixEqual(matrix, openLinearFlux.fullJacobianAmt());
}

TEST_F(RoadRunnerAPITests, GetFullJacobianUsingAmtModeAsLong) {
    /**
     * Some context for developers:
     * Python uses long for int values. So when
     * Config::setValue is used from Python, the Setting that
     * is implicitly created (second argument to Config::setValue) is a
     * std::int64_t, not an int! The Setting
     * is completely capable of handling an int64, but in other
     * places (getFullJacobian, for example) an int is expected,
     * generating an std::bad_variant_access error.
     * The solution is to use the Setting::getAs<int> command
     * instead of Setting::get<int>, which will do the
     * conversion, if one is needed.
     */
    Config::setValue(Config::ROADRUNNER_JACOBIAN_MODE, std::int64_t(Config::ROADRUNNER_JACOBIAN_MODE_AMOUNTS));
    RoadRunner rr(openLinearFlux.str());
    auto matrix = rr.getFullJacobian();
    checkMatrixEqual(matrix, openLinearFlux.fullJacobianAmt());
}


TEST_F(RoadRunnerAPITests, getIds) {
    std::list<std::string> expected(
            {
                    "S1", "S2", "[S1]", "[S2]", "default_compartment",
                    "kin", "kf", "kout", "kb", "_J0", "_J1", "_J2",
                    "init([S1])", "init([S2])", "init(S1)", "init(S2)",
                    "S1'", "S2'", "eigen(S1)", "eigenReal(S1)",
                    "eigenImag(S1)", "eigen(S2)", "eigenReal(S2)", "eigenImag(S2)",
            });
    RoadRunner rr(openLinearFlux.str());
    std::list<std::string> l;
    rr.getIds(SelectionRecord::ALL, l);
    ASSERT_EQ(expected, l);
}

TEST_F(RoadRunnerAPITests, LoadModelWithOnlyLayoutInformation) {
    LayoutOnly layoutOnly;
    RoadRunner rr(layoutOnly.str());

}



















