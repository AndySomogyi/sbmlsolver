#include "gtest/gtest.h"
#include "SteadyStateIntegrationTests.h"
#include "TestModelFactory.h"

#include "NLEQ1Solver.h"
#include "BasicNewtonIteration.h"

class NLEQ1IntetrationTests : public SteadyStateIntegrationTest {
public:

    NLEQ1IntetrationTests() : SteadyStateIntegrationTest() {};
};

/**
 * OpenLinearFlux (-> S1 -> S2 ->) does not require moiety conservation
 * but the NewtonIteration algorithm does not converge when starting
 * values are S1=0, S2=0. To solve this problem we use presimulation.
 */
TEST_F(NLEQ1IntetrationTests, CheckCorrectSteadyStateOpenLinearFlux) {
    testSteadyState(
            "OpenLinearFlux",
            "nleq1"
    );
}

TEST_F(NLEQ1IntetrationTests, CheckCorrectSteadyStateSimpleFluxManuallyReduced) {
    testSteadyState(
            "SimpleFluxManuallyReduced",
            "nleq1");
}

TEST_F(NLEQ1IntetrationTests, CheckCorrectSteadyStateSimpleFlux) {
    testSteadyState("SimpleFlux", "nleq1");
}

TEST_F(NLEQ1IntetrationTests, CheckCorrectSteadyStateVenkatraman2010) {
    testSteadyState("Venkatraman2010", "nleq1");
}

TEST_F(NLEQ1IntetrationTests, CheckCorrectSteadyStateBrown2004) {
    testSteadyState("Brown2004", "nleq1");
}

TEST_F(NLEQ1IntetrationTests, CheckCorrectSteadyStateBiomolecularEnd) {
    testSteadyState("BiomolecularEnd", "nleq1");
}

TEST_F(NLEQ1IntetrationTests, CheckCorrectSteadyStateFluxBiomolecularEnd) {
    testSteadyStateFluxes("BiomolecularEnd", "nleq1");
}

TEST_F(NLEQ1IntetrationTests, CheckJacobian) {
//    testSteadyStateFluxes("BiomolecularEnd", "nleq1");
    RoadRunner rr(BiomolecularEnd().str());
    rr.reset();
    std::cout << rr.getFullJacobian()<<std::endl;

}








