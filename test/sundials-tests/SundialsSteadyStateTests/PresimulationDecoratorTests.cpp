
#include <rrRoadRunner.h>
#include <CVODEIntegrator.h>
#include "gtest/gtest.h"
#include "SteadyStateSolverDecorator.h"
#include "PresimulationDecorator.h"
#include "NLEQ1Solver.h"
#include "NLEQ2Solver.h"
#include "BasicNewtonIteration.h"
#include "TestModelFactory.h"

using namespace rr;

class PresimulationDecoratorTests : public ::testing::Test {
public:
    OpenLinearFlux testModel = OpenLinearFlux();

    PresimulationDecoratorTests() {}

    void checkResults(RoadRunner& rr) {
        const auto &expectedResult = testModel.steadyState();
        for (auto &r: expectedResult) {
            const auto &speciesName = r.first;
            const auto &expectedValue = r.second;
            const auto &actualValue = rr.getValue(speciesName);
            ASSERT_NEAR(expectedValue, actualValue, 1e-5);
        }
    }
};

TEST_F(PresimulationDecoratorTests, CheckModelNotNullptrBasicNewton) {
    // setup with rr
    RoadRunner rr(testModel.str());
    ASSERT_NE(nullptr, rr.getModel());

    // create steady state solver
    SteadyStateSolver *solver = nullptr;
    BasicNewtonIteration basicNewtonIteration(rr.getModel());
    ASSERT_NE(nullptr, basicNewtonIteration.getModel());
    solver = &basicNewtonIteration;
    ASSERT_NE(nullptr, solver->getModel());

    // wrap solver in decorator
    PresimulationDecorator decorator(solver);
    ASSERT_NE(nullptr, decorator.getModel());
}

TEST_F(PresimulationDecoratorTests, CheckModelNotNullptrNLEQ1) {
    // setup with rr
    RoadRunner rr(testModel.str());
    ASSERT_NE(nullptr, rr.getModel());

    // create steady state solver
    SteadyStateSolver *solver = nullptr;
    NLEQ1Solver nleq1(rr.getModel());
    ASSERT_NE(nullptr, nleq1.getModel());
    solver = &nleq1;
    ASSERT_NE(nullptr, solver->getModel());

    // wrap solver in decorator
    PresimulationDecorator decorator(solver);
    ASSERT_NE(nullptr, decorator.getModel());
}

TEST_F(PresimulationDecoratorTests, CheckPresimulationNameField) {
    // setup with rr
    RoadRunner rr(testModel.str());

    // create steady state solver
    SteadyStateSolver *solver = nullptr;
    BasicNewtonIteration basicNewtonIteration(rr.getModel());
    solver = &basicNewtonIteration;

    PresimulationDecorator decorator(solver);
    solver = &decorator;
    ASSERT_STREQ("Presimulation(newton)", solver->getName().c_str());
}

TEST_F(PresimulationDecoratorTests, CheckModelTimeAfterPresimulation) {
    RoadRunner rr(testModel.str());
    ASSERT_EQ(0, rr.getModel()->getTime());

    SteadyStateSolver *solver;
    BasicNewtonIteration basicNewtonIteration(rr.getModel());
    // this model sucessfully solves after the 0.1 time steps
    basicNewtonIteration.setValue("presimulation_time", Setting(0.1));
    solver = &basicNewtonIteration;
    PresimulationDecorator presim(solver);
    solver = &presim;
    solver->solve();
    ASSERT_EQ(0.1, rr.getModel()->getTime());
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

    SteadyStateSolver *solver;
    BasicNewtonIteration basicNewtonIteration(rr.getModel());

    basicNewtonIteration.setValue("presimulation_time", Setting(10));
    solver = &basicNewtonIteration;
    PresimulationDecorator presim(solver);
    solver = &presim;
    solver->solve();

    checkResults(rr);
}


TEST_F(PresimulationDecoratorTests, CheckPresimulationDecoratorHasAccessToSolverVariables) {

    RoadRunner rr(testModel.str());

    BasicNewtonIteration *basicNewtonIteration = new BasicNewtonIteration(rr.getModel());

    basicNewtonIteration->setValue("presimulation_time", 14.37);
    ASSERT_EQ(14.37, (double)basicNewtonIteration->getValue("presimulation_time"));

    SteadyStateSolver *solver = basicNewtonIteration;
    PresimulationDecorator decorator(solver);
    solver = &decorator;
    ASSERT_EQ(14.37, (double)solver->getValue("presimulation_time"));
    delete basicNewtonIteration;
}

TEST_F(PresimulationDecoratorTests, CheckSolveTwiceTwoRRInstances) {

    // make independent copies of input string to ensure these
    // two rr instances are independent.
    std::string sbml1 = testModel.str();
    std::string sbml2 = testModel.str();

    RoadRunner rr1(sbml1);
    RoadRunner rr2(sbml2);

    BasicNewtonIteration basicNewtonIteration1(rr1.getModel());
    BasicNewtonIteration basicNewtonIteration2(rr2.getModel());

    basicNewtonIteration1.setValue("presimulation_time", 2);
    basicNewtonIteration2.setValue("presimulation_time", 4);

    SteadyStateSolver *solver1 = &basicNewtonIteration1;
    SteadyStateSolver *solver2 = &basicNewtonIteration2;
    PresimulationDecorator decorator1(solver1);
    PresimulationDecorator decorator2(solver2);
    solver1 = &decorator1;
    solver2 = &decorator2;
    solver1->solve();
    solver2->solve();
    checkResults(rr1);
    checkResults(rr2);

}

TEST_F(PresimulationDecoratorTests, CheckSolveTwiceBackToBack) {

    RoadRunner rr(testModel.str());

    BasicNewtonIteration *basicNewtonIteration = new BasicNewtonIteration(rr.getModel());

    basicNewtonIteration->setValue("presimulation_time", 2);

    SteadyStateSolver *solver = basicNewtonIteration;
    PresimulationDecorator decorator1(solver);
    solver = &decorator1;
    solver->solve();
    checkResults(rr);

    rr.setValue("S1", 0);
    rr.setValue("S2", 0);

    solver->solve();
    checkResults(rr);

    delete basicNewtonIteration;

}



























