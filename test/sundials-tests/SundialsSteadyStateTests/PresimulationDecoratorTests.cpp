
#include <rrRoadRunner.h>
#include <CVODEIntegrator.h>
#include "gtest/gtest.h"
#include "SteadyStateSolverDecorator.h"
#include "PresimulationDecorator.h"
#include "NewtonIteration.h"
#include "SBMLTestModelFactory.h"

using namespace rr;

class PresimulationDecoratorTests : public ::testing::Test {
public:
    OpenLinearFlux testModel = OpenLinearFlux();

    PresimulationDecoratorTests() {}
};

TEST_F(PresimulationDecoratorTests, CheckModelNotNullptr) {
    // setup with rr
    RoadRunner rr(testModel.str());
    ASSERT_NE(nullptr, rr.getModel());

    // create steady state solver
    SteadyStateSolver *solver = nullptr;
    NewtonIteration newtonIteration(rr.getModel());
    ASSERT_NE(nullptr, newtonIteration.getModel());
    solver = &newtonIteration;
    ASSERT_NE(nullptr, solver->getModel());

    // wrap solver in decorator
    PresimulationDecorator decorator(solver);
    ASSERT_NE(nullptr, decorator.getModel());
}

TEST_F(PresimulationDecoratorTests, CheckModelTimeAfterPresimulation) {
    RoadRunner rr(testModel.str());
    ASSERT_EQ(0, rr.getModel()->getTime());

    SteadyStateSolver *solver;
    NewtonIteration newtonIteration(rr.getModel());

    newtonIteration.setValue("presimulation_time", 10);
    solver = &newtonIteration;
    PresimulationDecorator presim(solver);
    solver = &presim;
    solver->solve();
    ASSERT_EQ(10, rr.getModel()->getTime());
}

/**
 * This model does not converge to steady state from starting position.
 * After presimulation the model converged to 10, 5.
 *
 * Note: Not a brilliant test because it has very strong dependency on the solver
 * routine. If the solver breaks, this test would break. (Mocking to the rescue ).
 */
TEST_F(PresimulationDecoratorTests, CheckSteadyStateValuesAfterPresimulation) {

    RoadRunner rr(testModel.str());

    SteadyStateSolver* solver;
    NewtonIteration newtonIteration(rr.getModel());

    newtonIteration.setValue("presimulation_time", 10);
    solver = &newtonIteration;
    PresimulationDecorator presim(solver);
    solver = &presim;
    solver->solve();

    const auto& expectedResult = testModel.steadyState();
    for (auto& r: expectedResult){
        const auto& speciesName = r.first;
        const auto& expectedValue = r.second.second;
        const auto& actualValue = rr.getValue(speciesName);
        ASSERT_NEAR(expectedValue, actualValue, 1e-5);
    }
}



TEST_F(PresimulationDecoratorTests,CheckPresimulationDecoratorHasAccessToSolverVariables) {

    RoadRunner rr(testModel.str());

    NewtonIteration* newtonIteration = new NewtonIteration(rr.getModel());

    newtonIteration->setValue("presimulation_time", 14.37);
    ASSERT_EQ(14.37, newtonIteration->getValueAsDouble("presimulation_time"));

    SteadyStateSolver* solver = newtonIteration;
    PresimulationDecorator decorator(solver);
    solver = &decorator;
    solver->getValueAsDouble("presimulation_time");
    ASSERT_EQ(14.37, solver->getValueAsDouble("presimulation_time"));





//    SteadyStateSolver* solver;
//    NewtonIteration* newtonIteration = new NewtonIteration(rr.getModel());
//    newtonIteration->setValue("presimulation_time", 10);
//    solver = newtonIteration;
//
//    SteadyStateSolverDecorator solverDecorator(rr.getSteadyStateSolver());
//    localSolver = &solverDecorator;

}






















