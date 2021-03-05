#include <fstream>
#include "gtest/gtest.h"

#include "FixedPointIteration.h"
#include "rrRoadRunner.h"
#include "SBMLTestModelFactory.h"

using namespace rr;

class FixedPointIterationTests : public ::testing::Test {

public:
    FixedPointIterationTests() = default;
};

//todo remember to delete this method.
void openInCopasi(const std::string &sbml) {
    std::string f = R"(D:\roadrunner\roadrunner\test\sundials-tests\SundialsSteadyStateTests\tmp.cps)";
    std::ofstream cps;
    cps.open(f);
    cps << sbml;
    cps.close();
    std::string cmd = "CopasiUI -i " + f;
    system(cmd.c_str());
}

// todo test ideas:
//     - case where we already have a steady state. Should return a special kinsol code
//
TEST_F(FixedPointIterationTests, TestCalculatedSteadyStateValuesAreCorrect) {
//    auto testModel = (Venkatraman2010*) SBMLTestModelFactory("Venkatraman2010");
    auto *testModel = (OpenLinearFlux *) SBMLTestModelFactory("OpenLinearFlux");
    RoadRunner rr(testModel->str());
    FixedPointIteration fixedPointIteration(rr.getModel());

    //openInCopasi(testModel->str());
    std::cout << rr.getFloatingSpeciesConcentrationsNamedArray() << std::endl;
    fixedPointIteration.solve();
    auto result = rr.getFloatingSpeciesConcentrationsNamedArray();
    auto expectedResult = testModel->steadyState();
    std::vector<std::string> names = result.getColNames();
    for (int i = 0; i < names.size(); i++) {
        std::string speciesID = names[i];
        double actualResult = result[0][i];
        EXPECT_NEAR(expectedResult[speciesID], actualResult, 0.0001);
    }

}

class SettingsTests :
        public FixedPointIterationTests,
        public ::testing::WithParamInterface<std::pair<std::string, Variant>> {
};


TEST(How, t) {
    auto *testModel = (SimpleFlux *) SBMLTestModelFactory("SimpleFlux");
    RoadRunner rr(testModel->str());

    SimulateOptions options;
    options.start = 0;
    options.steps = 1000;
    options.duration = 0.1;
    auto res = rr.simulate(&options);
    std::cout << *res << std::endl;
    auto v = rr.getRatesOfChangeNamedArray();
    std::cout << v << std::endl;

    openInCopasi(testModel->str());

}
//

// Need to write a function to make a call to lapack's dgetrf
// function for computing the determinent.
// Then compute it for the jacobian of this system.
// This operation is potentially computationally intensive,
// so it should be possible to turn this check off.
// However, if it is on, and we find that the determinent is 0,
// (i.e. we have a singular matrix), then we automatically turn on
// moiety conservation analysis, which should* fix the error.
//
// But the question remains, how does moiety conservation analysis reduce the system
// in such a way as to bypass the problem?


// Also, todo: must consider whether stiochoimetry is already considered when
// roadrunner gives us the vector of rates.


// todo test the return type of solve