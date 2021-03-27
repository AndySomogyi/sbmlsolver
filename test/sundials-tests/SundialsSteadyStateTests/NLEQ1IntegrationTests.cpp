#include "gtest/gtest.h"
#include "SteadyStateIntegrationTests.h"
#include "TestModelFactory.h"

class NLEQ1IntetrationTests : public SteadyStateIntegrationTests {
public:

    NLEQ1IntetrationTests() : SteadyStateIntegrationTests() {};
};

/**
 * OpenLinearFlux (-> S1 -> S2 ->) does not require moiety conservation
 * but the NewtonIteration algorithm does not converge when starting
 * values are S1=0, S2=0. To solve this problem we use presimulation.
 */
TEST_F(NLEQ1IntetrationTests, CheckCorrectSteadyStateOpenLinearFlux) {
    testSteadyState<OpenLinearFlux>(
            "OpenLinearFlux",
            "nleq1"
    );
}

TEST_F(NLEQ1IntetrationTests, CheckCorrectSteadyStateSimpleFluxManuallyReduced) {
    testSteadyState<SimpleFluxManuallyReduced>(
            "SimpleFluxManuallyReduced",
            "nleq1");
}

TEST_F(NLEQ1IntetrationTests, CheckCorrectSteadyStateSimpleFlux) {
    testSteadyState<SimpleFlux>("SimpleFlux", "nleq1");
}

TEST_F(NLEQ1IntetrationTests, CheckCorrectSteadyStateVenkatraman2010) {
    testSteadyState<Venkatraman2010>("Venkatraman2010", "nleq1");
}

TEST_F(NLEQ1IntetrationTests, CheckCorrectSteadyStateBrown2004) {
    testSteadyState<Brown2004>("Brown2004", "nleq1");
}

