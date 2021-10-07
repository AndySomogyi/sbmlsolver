#include "SundialsSteadyStateSolverUnitTest.h"

/**
 * Test the automatic moiety analysis option, which is present in superclass
 * of all steady state solvers.
 *
 * Three components to these tests:
 *  1) The model can either need moiety conservation analysis or not.
 *  2) The auto_moiety_analysis option can be on or off
 *  3) The RoadRunner moiety_conservation_analysis option can be on or off
 *
 * Test are named using this convention:
 *  (Needed or not)(auto_moiety_analysis)(moiety_conservation_analysis option)
 *  i.e. (Needed)(On)(ON) --> OnNeededOn
 *
 *  There are 3 options each with 2 possibilities --> 2^3 = 8 possibilities.
 *
 *  Two different models are used:
 *      1) OpenLinearFlux, which does not require moiety conservation analysis
 *      1) SimpleFlux, which requires moiety conservation analysis
 *
 */
class AutomaticMoietyAnalysisTests : public SundialsSteadyStateSolverUnitTest {
public:
    AutomaticMoietyAnalysisTests()
            : SundialsSteadyStateSolverUnitTest() {};

    template<class ModelType>
    static void check(bool autoMoietyOption, bool moietyOption) {
        ModelType model = ModelType();
        RoadRunner rr(model.str());

        rr.setConservedMoietyAnalysis(moietyOption);

        auto solver = rr.getSteadyStateSolver();
        solver->setValue("auto_moiety_analysis", autoMoietyOption);

        // if moieties are needed (as they are in SimpleFlux), and both autoMoiety and moiety options are false
        // then we expect an error
        if (model.modelName() == "SimpleFlux" &&  !autoMoietyOption && !moietyOption){
            // (can't go more specific error since errors are inherited directly from exception)
            ASSERT_THROW(rr.steadyState(), std::exception);
        } else {
            rr.steadyState();
            checkResults(model.steadyState(), rr.getFloatingSpeciesConcentrationsNamedArray());
        }

    }

};

/*
 * 1) Needed = True
 * 2) auto_moiety_analysis = on
 * 3) moiety_conservation_analysis = on
 */
TEST_F(AutomaticMoietyAnalysisTests, NeededOnOn) {
    check<SimpleFlux>(true, true);
}

/*
 * 1) Needed = True
 * 2) auto_moiety_analysis = on
 * 3) moiety_conservation_analysis = off
 */
TEST_F(AutomaticMoietyAnalysisTests, NeededOnOff) {
    check<SimpleFlux>(true, false);
}

/*
 * 1) Needed = True
 * 2) auto_moiety_analysis = off
 * 3) moiety_conservation_analysis = on
 */
TEST_F(AutomaticMoietyAnalysisTests, NeededOffOn) {
    check<SimpleFlux>(false, true);
}
/*
 * 1) Needed = True
 * 2) auto_moiety_analysis = off
 * 3) moiety_conservation_analysis = off
 */
TEST_F(AutomaticMoietyAnalysisTests, NeededOffOff) {
    // this situation causes error, but we catch it.
        check<SimpleFlux>(false, false);

}
/*
 * 1) Needed = false
 * 2) auto_moiety_analysis = on
 * 3) moiety_conservation_analysis = on
 */
TEST_F(AutomaticMoietyAnalysisTests, NotNeededOnOn) {
    check<OpenLinearFlux>(true, true);

}
/*
 * 1) Needed = false
 * 2) auto_moiety_analysis = on
 * 3) moiety_conservation_analysis = off
 */
TEST_F(AutomaticMoietyAnalysisTests, NotNeededOnOff) {
    check<OpenLinearFlux>(true, false);
}
/*
 * 1) Needed = false
 * 2) auto_moiety_analysis = off
 * 3) moiety_conservation_analysis = on
 */
TEST_F(AutomaticMoietyAnalysisTests, NotNeededOffOn) {
    check<OpenLinearFlux>(false, true);
}

/*
 * 1) Needed = false
 * 2) auto_moiety_analysis = off
 * 3) moiety_conservation_analysis = on
 */
TEST_F(AutomaticMoietyAnalysisTests, NotNeededOffOff) {
    check<OpenLinearFlux>(false, false);
}

