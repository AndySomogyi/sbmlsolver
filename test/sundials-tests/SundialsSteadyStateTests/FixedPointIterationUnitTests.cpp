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
    std::cout << "testModel.modelName(): " << testModel.modelName() << std::endl;
    FixedPointIteration solver(rr->getModel());
//    solver.setValue("maa", 0);
//    solver.setValue("damping_aa", 0.5);
    solver.solve();
    std::cout << rr->getFloatingSpeciesConcentrationsNamedArray() << std::endl;
    checkResults(rr->getFloatingSpeciesConcentrationsNamedArray());
}

TEST_F(FixedPointIterationUnitTests, ChangeAndResetSettings) {
    FixedPointIteration solver(rr->getModel());
    solver.setValue("strategy", Setting("linesearch"));
    solver.resetSettings();
    ASSERT_STREQ("basic", solver.getValueAsString("strategy").c_str());
}


TEST_F(FixedPointIterationUnitTests, RegenerateTheModelBeforeCreatingSolver) {
    rr->regenerateModel();
    FixedPointIteration solver(rr->getModel());
    solver.solve();
    checkResults(rr->getFloatingSpeciesConcentrationsNamedArray());
}

TEST_F(FixedPointIterationUnitTests, RegenerateTheModelAfterCreatingSolver) {
    FixedPointIteration solver(rr->getModel());
    rr->regenerateModel();
    // after regeneration, the pointer to the model is different
    // so we must sync with model before solving.
    solver.syncWithModel(rr->getModel());
    solver.solve();
    checkResults(rr->getFloatingSpeciesConcentrationsNamedArray());
}
























