#include <fstream>
#include "gtest/gtest.h"

#include "NewtonIteration.h"
#include "BasicNewtonIteration.h"
#include "LinesearchNewtonIteration.h"
#include "rrRoadRunner.h"
#include "TestModelFactory.h"
#include "SteadyStateIntegrationTests.h"

using namespace rr;

class NewtonIterationIntegrationTests : public SteadyStateIntegrationTests {

public:
    NewtonIterationIntegrationTests() : SteadyStateIntegrationTests() {};
};


TEST_F(NewtonIterationIntegrationTests, CheckDecoratorRemovedAfterSolving) {
    // setup with rr
    OpenLinearFlux testModel;
    RoadRunner rr(testModel.str());

    rr.setSteadyStateSolver("newton");
    BasicDictionary opt;

    // will add PresimulationDecorator inside rr.steadyState()
    opt.setItem("allow_presimulation", true);

    rr.steadyState(&opt);
    // this string would be "Presimulation(NewtonIteration)" if decorator was not removed before setadyState returns.
    ASSERT_STREQ("newton", rr.getSteadyStateSolver()->getName().c_str());
}

TEST_F(NewtonIterationIntegrationTests, CheckDecoratorRemovedAfterFailureToSolve) {
    OpenLinearFlux testModel;
    RoadRunner rr(testModel.str());

    rr.setSteadyStateSolver("newton");
    rr.getSteadyStateSolver()->setValue("allow_presimulation", false);
    rr.getSteadyStateSolver()->setValue("allow_approx", false);

    try {
        rr.steadyState();
    } catch (std::runtime_error &err) {
        ASSERT_STREQ("newton", rr.getSteadyStateSolver()->getName().c_str());
    } catch (std::exception) {
        ASSERT_FALSE("Test Failed"); // should never get here
    }

}


/**
 * The OpenLinearFlux does not converge from the initial starting position (0, 0).
 * In this situation we raise an error. Here we test that it does
 */
TEST_F(NewtonIterationIntegrationTests, CheckRaiseErrorWhenNotConverge) {
    // setup with rr
    OpenLinearFlux testModel;
    RoadRunner rr(testModel.str());
    rr.setSteadyStateSolver("newton");
    rr.getSteadyStateSolver()->setValue("allow_presimulation", false);
    rr.getSteadyStateSolver()->setValue("allow_approx", false);
    ASSERT_THROW(rr.steadyState(), std::runtime_error);
}

TEST_F(NewtonIterationIntegrationTests, CheckNewtonIterationIsARegisteredSolver) {
    OpenLinearFlux testModel;
    RoadRunner rr(testModel.str());
    const auto &solverNames = rr.getRegisteredSteadyStateSolverNames();

    // name of bool should be expressive; it shows up on gtest failure.
    bool foundNewtonIterationInRegisteredSolverNames = false;
    if (std::find(solverNames.begin(), solverNames.end(), "newton") != solverNames.end()) {
        foundNewtonIterationInRegisteredSolverNames = true;
    }
    ASSERT_TRUE(foundNewtonIterationInRegisteredSolverNames);
}


/**
 * @brief tests in this fixture all solve for steady state using the
 * "basic" strategy. This corresponds to setting the KIN_NONE flag
 * in kinsol.
 */
class BasicNewtonIterationTests : public NewtonIterationIntegrationTests {
public:

    BasicNewtonIterationTests() : NewtonIterationIntegrationTests() {};
};

/**
 * OpenLinearFlux (-> S1 -> S2 ->) does not require moiety conservation
 * but the NewtonIteration algorithm does not converge when starting
 * values are S1=0, S2=0. To solve this problem we use presimulation.
 */
TEST_F(BasicNewtonIterationTests, CheckCorrectSteadyStateOpenLinearFlux) {
    testSteadyState<OpenLinearFlux>(
            "OpenLinearFlux",
            "newton",
            false
    );
}

TEST_F(BasicNewtonIterationTests, CheckCorrectSteadyStateSimpleFluxManuallyReduced) {
    testSteadyState<SimpleFluxManuallyReduced>(
            "SimpleFluxManuallyReduced",
            "newton",
            false // moiety conservation done manually
    );
}

TEST_F(BasicNewtonIterationTests, CheckCorrectSteadyStateSimpleFlux) {
    testSteadyState<SimpleFlux>("SimpleFlux","newton", true);
}

TEST_F(BasicNewtonIterationTests, CheckCorrectSteadyStateVenkatraman2010) {
    testSteadyState<Venkatraman2010>("Venkatraman2010","newton", false);
}

/**
 * The reduced jacobian for the Brown2004 model is singular. This is the matrix
 * produced by rr.getReducedJacobian:
 *
     SosInactive,Raf1Inactive,MekInactive,boundEGFReceptor,freeNGFReceptor,RasInactive,BRafInactive,ErkInactive,PI3KInactive,Rap1Inactive,P90RskInactive,AktInactive,C3GInactive
    0,0,0,-13.4333,382.691,0,0,0,0,0,0,0,0
    0,-14.2784,0,0,0,0,0,0,0,0,0,0,0
    0,2.30835,-0.655209,0,0,0,34.4364,0,0,0,0,0,0
    0,0,0,-220.307,0,0,0,0,0,0,0,0,0
    0,0,0,0,-0.0716435,0,0,0,0,0,0,0,0
    24.8873,0,0,0,0,-126.447,0,0,0,0,0,0,0
    0,0,0,0,0,0,-4.86736,0,0,0.231518,0,0,0
    0,0,0.553919,0,0,0,0,-0.305147,0,0,0,0,0
    0,0,0,-4.2007,0,0.0236007,0,0,0,0,0,0,0
    0,0,0,0,0,0,0,0,0,-11.0538,0,0,1.28411
    0,0,0,0,0,0,0,0.00290243,0,0,0,0,0
    0,0,0,0,0,0,0,0,0,0,0,0,0
    0,0,0,0,132.676,0,0,0,0,0,0,0,0


 *
 *  But, copasi can solve this problem. So what's going on?
 *
 *  Copasi's reduced jacobian:
 *
 *  	SosInactive	Raf1Inactive	MekInactive	boundEGFReceptor	freeNGFReceptor	RasInactive	BRafInactive	ErkInactive	PI3KInactive	Rap1Inactive	P90RskInactive	AktInactive	C3GInactive
        SosInactive	-215.72574022826169	0	0	-11.183052170708425	381.33685325504786	0	0	0	0	0	0	0	0
        Raf1Inactive	0	-27.635635255568253	0	0	0	0.5813005366518279	0	0	0	0	0	0	0
        MekInactive	0	16.85614964587997	-0.46289602750681763	0	0	0	93.91677635926862	0	0	0	0	0	0
        boundEGFReceptor	0	0	0	-216.810972523873	0	0	0	0	0	0	0	0	0
        freeNGFReceptor	0	0	0	0	-0.0691691849922915	0	0	0	0	0	0	0	0
        RasInactive	24.690978316163786	0	0	0	0	-126.77208151837391	0	0	0	0	0	0	0
        BRafInactive	0	0	0	0	0	0	-4.891744748293693	0	0	0.23033517823179622	0	0	0
        ErkInactive	0	0	1.039767897430876	0	0	0	0	-1.2072683701290614	0	0	0	0	0
        PI3KInactive	0	0	0	-9.403035830110282e-113	0	4.6169163182598105e-115	0	0	-4.6187267432798516	0	0	0	0
        Rap1Inactive	0	0	0	0	0	0	0	0	0	-10.184170980689187	0	0	0
        P90RskInactive	0	0	0	0	0	0	0	-2.282127027990553e-33	0	0	-0.013466984409233013	0	0
        AktInactive	0	0	0	0	0	0	0	0	0	0	0	-0.010391218913956345	0
        C3GInactive	0	0	0	0	-7.420268897137381e-28	0	0	0	0	0	0	0	-102.1313160913126

    The answer is probably that there is a bug in roadrunners conservation analysis.

    This test is disabled until we get a resolution on this problem

    @note: not disabled - commented out, because gtest on mac seems to consider the disabled test a failure :/
 */
TEST_F(BasicNewtonIterationTests, CheckCorrectSteadyStateBrown2004) {
    testSteadyState<Brown2004>("Brown2004", "newton",true);
}


/**
 * @brief tests in this fixture all solve for steady state using the
 * "linesearch" strategy. This corresponds to setting the KIN_LINESEARCH flag
 * in kinsol.
 */
class LineSearchNewtonIterationTests : public NewtonIterationIntegrationTests {
public:

    LineSearchNewtonIterationTests() : NewtonIterationIntegrationTests() {};
};

TEST_F(LineSearchNewtonIterationTests, CheckCorrectSteadyStateOpenLinearFlux) {
    testSteadyState<OpenLinearFlux>("OpenLinearFlux","newton_linesearch", false);
}

TEST_F(LineSearchNewtonIterationTests, CheckCorrectSteadyStateSimpleFluxManuallyReduced) {
    testSteadyState<SimpleFluxManuallyReduced>("SimpleFluxManuallyReduced",
                                               "newton_linesearch",
                                               false// moiety conservation done manually
                                               );
}

TEST_F(LineSearchNewtonIterationTests, CheckCorrectSteadyStateSimpleFlux) {
    testSteadyState<SimpleFlux>("SimpleFlux", "newton_linesearch",true);
}

TEST_F(LineSearchNewtonIterationTests, CheckCorrectSteadyStateVenkatraman2010) {
    testSteadyState<Venkatraman2010>("Venkatraman2010", "newton_linesearch",false);
}

/**
 * See BasicNewtonIterationTests for reason by this is currently disabled.
 */
TEST_F(LineSearchNewtonIterationTests, DISABLED_CheckCorrectSteadyStateBrown2004) {
    testSteadyState<Brown2004>("Brown2004", "newton_linesearch", true);
}

/**
 * Integration style tests verifying interactions between
 * steady state decorators.
 */
class DecoratorIntegrationTests : public ::testing::Test {

public:

    DecoratorIntegrationTests() = default;

    void checkResult(OpenLinearFlux &testModel, RoadRunner &rr) {
        const auto &expectedResult = testModel.steadyState();
        for (auto &r: expectedResult) {
            const auto &speciesName = r.first;
            const auto &expectedValue = r.second.second;
            const auto &actualValue = rr.getValue(speciesName);
            ASSERT_NEAR(expectedValue, actualValue, 1e-5);
        }
    }

};


/**
 * This model cannot be solved without presimulation or approximation
 */
TEST_F(DecoratorIntegrationTests, NoDecoratorsAdded) {
    OpenLinearFlux testModel;
    RoadRunner rr(testModel.str());
    rr.setSteadyStateSolver("newton");

    BasicDictionary opt;
    opt.setItem("allow_approx", false);
    opt.setItem("allow_presimulation", false);

    ASSERT_THROW(rr.steadyState(&opt), std::runtime_error);
}

TEST_F(DecoratorIntegrationTests, PresimulationOnly) {
    OpenLinearFlux testModel;
    RoadRunner rr(testModel.str());
    rr.setSteadyStateSolver("newton");

    BasicDictionary opt;
    opt.setItem("allow_approx", false);
    opt.setItem("allow_presimulation", true);

    rr.steadyState(&opt);
    checkResult(testModel, rr);
}

TEST_F(DecoratorIntegrationTests, ApproximationOnly) {
    OpenLinearFlux testModel;
    RoadRunner rr(testModel.str());
    rr.setSteadyStateSolver("newton");

    BasicDictionary opt;
    opt.setItem("allow_approx", true);
    opt.setItem("allow_presimulation", false);

    rr.steadyState(&opt);
    checkResult(testModel, rr);

}

/**
 *
 */
TEST_F(DecoratorIntegrationTests, CombineApproxAndPresimDecorators) {
    // first try presimulation then will try to approximate
    OpenLinearFlux testModel;
    RoadRunner rr(testModel.str());
    rr.setSteadyStateSolver("newton");

    BasicDictionary opt;
    opt.setItem("allow_approx", true);
    opt.setItem("allow_presimulation", true);

    // this is *not* enough presimulation for the OpenLinearFlux model to solve
    // therefore the algorithm is force to turn to approximation, which it does automatically.
    opt.setItem("presimulation_time", 0.0001);

    rr.steadyState(&opt);
    checkResult(testModel, rr);
}




















