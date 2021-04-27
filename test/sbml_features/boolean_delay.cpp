#include "gtest/gtest.h"
#include "rrRoadRunner.h"
#include "rrException.h"
#include "rrUtils.h"
#include "rrTestSuiteModelSimulation.h"
#include "sbml/SBMLTypes.h"
#include "sbml/SBMLReader.h"
#include "../test_util.h"
#include <filesystem>

using namespace testing;
using namespace rr;
using namespace std;
using std::filesystem::path;

extern path gRRTestDir;
extern path gRROutputDir;

TEST(SBML_FEATURES, BOOLEAN_DELAY_1)
{
    try
    {
        RoadRunner rri((gRRTestDir / "models/SBML_FEATURES/boolean_trigger.l3v2.xml").string());
        rri.validateCurrentSBML();
        rri.getSimulateOptions().duration = 2;
        rri.simulate();
        EXPECT_EQ(rri.getValue(rri.createSelection("x")), 0.0);
        rri.getSimulateOptions().start = 0.1;
        rri.simulate();
        EXPECT_EQ(rri.getValue(rri.createSelection("x")), 3);
    }
    catch (std::exception& ex)
    {
        std::cout << "Exception: " << ex.what() << std::endl;
        EXPECT_TRUE(false);
    }
}
TEST(SBML_FEATURES, BOOLEAN_DELAY_2)
{
    try
    {
        RoadRunner rri((gRRTestDir / "models/SBML_FEATURES/boolean_trigger_2.l3v2.xml").string());
        rri.validateCurrentSBML();
        rri.getSimulateOptions().duration = 1;
        rri.simulate();
        EXPECT_EQ(rri.getValue(rri.createSelection("x")), 0.0);
        rri.getSimulateOptions().start = 0.1;
        rri.simulate();
        EXPECT_EQ(rri.getValue(rri.createSelection("x")), 3);
    }
    catch (std::exception& ex)
    {
        std::cout << "Exception: " << ex.what() << std::endl;
        EXPECT_TRUE(false);
    }
}
