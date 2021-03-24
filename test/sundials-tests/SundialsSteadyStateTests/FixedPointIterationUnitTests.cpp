#include <fstream>
#include "gtest/gtest.h"
#include "SundialsSteadyStateSolverUnitTest.h"

#include "FixedPointIteration.h"
#include "rrRoadRunner.h"
#include "TestModelFactory.h"

using namespace rr;

class FixedPointIterationUnitTests : public SundialsSteadyStateSolverUnitTest {

public:
    FixedPointIterationUnitTests() : SundialsSteadyStateSolverUnitTest() {};
    
    ~FixedPointIterationUnitTests() override = default;
};



TEST_F(FixedPointIterationUnitTests, SolveUsingSolverDirectly) {
    // aka without a RoadRunner instance
    FixedPointIteration solver(rr->getModel());
    solver.solve();
    checkResults(rr->getFloatingSpeciesConcentrationsNamedArray());
}

TEST_F(FixedPointIterationUnitTests, ChangeAndResetSettings) {
    FixedPointIteration solver(rr->getModel());
    solver.setValue("strategy", Variant("linesearch"));
    solver.resetSettings();
    ASSERT_STREQ("basic", solver.getValueAsString("strategy").c_str());
}


TEST_F(FixedPointIterationUnitTests, RegenerateTheModelBeforeCreatingSolver) {
    rr->regenerate();
    FixedPointIteration solver(rr->getModel());
    solver.solve();
    checkResults(rr->getFloatingSpeciesConcentrationsNamedArray());
}

TEST_F(FixedPointIterationUnitTests, RegenerateTheModelAfterCreatingSolver) {
    FixedPointIteration solver(rr->getModel());
    rr->regenerate();
    // after regeneration, the pointer to the model is different
    // so we must sync with model before solving.
    solver.syncWithModel(rr->getModel());
    solver.solve();
    checkResults(rr->getFloatingSpeciesConcentrationsNamedArray());
}
























