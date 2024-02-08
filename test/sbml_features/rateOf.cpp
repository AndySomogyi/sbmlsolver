#include "gtest/gtest.h"
#include "rrRoadRunner.h"
#include "rrException.h"
#include "rrUtils.h"
#include "rrTestSuiteModelSimulation.h"
#include "sbml/SBMLTypes.h"
#include "sbml/SBMLReader.h"
#include "../test_util.h"
#include <filesystem>
#include "RoadRunnerTest.h"

using namespace testing;
using namespace rr;
using namespace std;
using std::filesystem::path;

class SBMLFeatures : public RoadRunnerTest {
public:
    path SBMLFeaturesDir = rrTestModelsDir_ / "SBMLFeatures";
    SBMLFeatures() = default;
};

TEST_F(SBMLFeatures, RATEOF_AR1)
{
    try
    {
        RoadRunner rri((SBMLFeaturesDir / "rateOf_assignmentRule1.xml").string());
        rri.getSimulateOptions().duration = 2;
        rri.simulate();
        EXPECT_EQ(rri.getValue(rri.createSelection("s2")), 0.0);
    }
    catch (std::exception& ex)
    {
        std::cout << "Exception: " << ex.what() << std::endl;
        EXPECT_TRUE(false);
    }
}
TEST_F(SBMLFeatures, RATEOF_AR2)
{
    try
    {
        RoadRunner rri((SBMLFeaturesDir / "rateOf_assignmentRule2.xml").string());
        rri.validateCurrentSBML();
        rri.getSimulateOptions().duration = 1;
        rri.simulate();
        EXPECT_EQ(rri.getValue(rri.createSelection("s1")), 1.0);
    }
    catch (std::exception& ex)
    {
        std::cout << "Exception: " << ex.what() << std::endl;
        EXPECT_TRUE(false);
    }
}
