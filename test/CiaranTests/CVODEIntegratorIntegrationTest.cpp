

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "rrRoadRunner.h"
#include "rrExecutableModel.h"
#include "CVODEIntegrator.h"
#include "Solver.h"


using namespace rr;
using namespace testing;

TEST(CVODEIntegrationTests, CheckThatAModelSimulates){
    RoadRunner r;
    r.addCompartment("Cell", 1.0, false);
    r.addSpecies("S1", "Cell", 100.0, false, false, "", false);
    r.addSpecies("S2", "Cell", 0.0, false, false, "", false);
    r.addParameter("k1", 0.1, false);
    r.addParameter("k2", 1.0, false);
    r.addReaction("R1", {"S1"}, {"S2"}, "k1*S1", false);
    r.addReaction("R2", {"S2"}, {"S1"}, "k2*S2", true);

    SimulateOptions options;
    options.start = 0.0;
    options.steps = 11;
    options.duration = 10;
    auto result = r.simulate(&options);
    std::cout << *result << std::endl;

    double state[2];
    // returns the number of items copied in to the provided buffer
    int numCopied = r.getModel()->getStateVector(state);
    ASSERT_EQ(numCopied, 2);

    double expected[2] = {90.9093,9.09075};
    ASSERT_NEAR(*expected, *state, 0.0001);

}
































