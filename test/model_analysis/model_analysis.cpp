#include "rrLogger.h"
#include "rrRoadRunner.h"
#include "rrException.h"
#include "rrStringUtils.h"
#include "gtest/gtest.h"
#include "LLVMExecutableModel.h"
#include <filesystem>
#include "RoadRunnerTest.h"

using std::filesystem::path;

using namespace rr;

class ModelAnalysisTests : public RoadRunnerTest {
public:
    path modelAnalysisModelsDir ;

    ModelAnalysisTests() {
        modelAnalysisModelsDir =  rrTestModelsDir_ / "ModelAnalysis";
    }
};


TEST_F(ModelAnalysisTests, ResetBoundarySpeciesRate) {
    RoadRunner rr((modelAnalysisModelsDir / "boundary_change_rate.xml").string());
    double S1 = rr.getValue("init(S1)");
    EXPECT_EQ(S1, 3);

    rr.simulate();
    S1 = rr.getValue("init(S1)");
    EXPECT_EQ(S1, 3);
    S1 = rr.getValue("S1");
    EXPECT_EQ(S1, 23);

    rr.reset(SelectionRecord::BOUNDARY);
    S1 = rr.getValue("S1");
    EXPECT_EQ(S1, 3);

    //Since it's being changed by a rate rule, it should also be reset by default:
    rr.simulate();
    rr.reset();
    S1 = rr.getValue("S1");
    EXPECT_EQ(S1, 3);

    //Now check what happens when we change the initial value:
    rr.setValue("init(S1)", 10);
    rr.simulate();
    rr.reset();
    S1 = rr.getValue("S1");
    EXPECT_EQ(S1, 10);
}


TEST_F(ModelAnalysisTests, ResetBoundarySpeciesEvent) {
    RoadRunner rr((modelAnalysisModelsDir / "boundary_change_event.xml").string());
    double S1 = rr.getValue("init(S1)");
    EXPECT_EQ(S1, 3);

    rr.simulate();
    S1 = rr.getValue("init(S1)");
    EXPECT_EQ(S1, 3);
    S1 = rr.getValue("S1");
    EXPECT_EQ(S1, 4);

    rr.reset(SelectionRecord::BOUNDARY);
    S1 = rr.getValue("S1");
    EXPECT_EQ(S1, 3);

    //Since it's being changed by an event, it should not be reset by default:
    rr.simulate();
    rr.reset();
    S1 = rr.getValue("S1");
    EXPECT_EQ(S1, 4);

    //Now check what happens when we change the initial value:
    rr.setValue("init(S1)", 10);
    rr.simulate();
    rr.reset(SelectionRecord::BOUNDARY);
    S1 = rr.getValue("S1");
    EXPECT_EQ(S1, 10);
}


TEST_F(ModelAnalysisTests, ResetFloatingSpeciesRate) {
    RoadRunner rr((modelAnalysisModelsDir / "floating_change_rate.xml").string());
    double S1 = rr.getValue("init(S1)");
    EXPECT_EQ(S1, 3);

    rr.simulate();
    S1 = rr.getValue("init(S1)");
    EXPECT_EQ(S1, 3);
    S1 = rr.getValue("S1");
    EXPECT_EQ(S1, 23);

    rr.reset(SelectionRecord::FLOATING);
    S1 = rr.getValue("S1");
    EXPECT_EQ(S1, 3);

    //Since it's being changed by a rate rule, it should definitely be reset by default:
    rr.simulate();
    rr.reset();
    S1 = rr.getValue("S1");
    EXPECT_EQ(S1, 3);

    //Now check what happens when we change the initial value:
    rr.setValue("init(S1)", 10);
    rr.simulate();
    rr.reset();
    S1 = rr.getValue("S1");
    EXPECT_EQ(S1, 10);
}


TEST_F(ModelAnalysisTests, ResetFloatingSpeciesEvent) {
    RoadRunner rr((modelAnalysisModelsDir / "floating_change_event.xml").string());
    double S1 = rr.getValue("init(S1)");
    EXPECT_EQ(S1, 3);

    rr.simulate();
    S1 = rr.getValue("init(S1)");
    EXPECT_EQ(S1, 3);
    S1 = rr.getValue("S1");
    EXPECT_EQ(S1, 4);

    rr.reset(SelectionRecord::FLOATING);
    S1 = rr.getValue("S1");
    EXPECT_EQ(S1, 3);

    //Since it's a floating species, even though it's only being changed by an event, it should still be reset by default:
    rr.simulate();
    rr.reset();
    S1 = rr.getValue("S1");
    EXPECT_EQ(S1, 3);

    //Now check what happens when we change the initial value:
    rr.setValue("init(S1)", 10);
    rr.simulate();
    rr.reset(SelectionRecord::FLOATING);
    S1 = rr.getValue("S1");
    EXPECT_EQ(S1, 10);
}


TEST_F(ModelAnalysisTests, ResetCompartmentFromRate) {
    RoadRunner rr((modelAnalysisModelsDir / "compartment_change_rate.xml").string());
    double comp = rr.getValue("init(C1)");
    EXPECT_EQ(comp, 5);

    rr.simulate();
    comp = rr.getValue("init(C1)");
    EXPECT_EQ(comp, 5);
    comp = rr.getValue("C1");
    EXPECT_EQ(comp, 15);

    rr.reset(SelectionRecord::COMPARTMENT);
    comp = rr.getValue("C1");
    EXPECT_EQ(comp, 5);

    //Since it's being changed by a rate rule, it should also be reset by default:
    rr.simulate();
    rr.reset();
    comp = rr.getValue("C1");
    EXPECT_EQ(comp, 5);

    //Now check what happens when we change the initial value:
    rr.setValue("init(C1)", 10);
    rr.simulate();
    rr.reset(SelectionRecord::COMPARTMENT);
    comp = rr.getValue("C1");
    EXPECT_EQ(comp, 10);

}


TEST_F(ModelAnalysisTests, ResetCompartmentFromEvent) {
    RoadRunner rr((modelAnalysisModelsDir / "compartment_change_event.xml").string());
    double comp = rr.getValue("init(C1)");
    EXPECT_EQ(comp, 5);

    rr.simulate();
    comp = rr.getValue("init(C1)");
    EXPECT_EQ(comp, 5);
    comp = rr.getValue("C1");
    EXPECT_EQ(comp, 2);

    rr.reset(SelectionRecord::COMPARTMENT);
    comp = rr.getValue("C1");
    EXPECT_EQ(comp, 5);

    //Since it's being changed by an event, it should not be reset by default:
    rr.simulate();
    rr.reset();
    comp = rr.getValue("C1");
    EXPECT_EQ(comp, 2);

    //Now check what happens when we change the initial value:
    rr.setValue("init(C1)", 10);
    rr.simulate();
    rr.reset(SelectionRecord::COMPARTMENT);
    comp = rr.getValue("C1");
    EXPECT_EQ(comp, 10);

}


TEST_F(ModelAnalysisTests, ResetParamFromRate) {
    RoadRunner rr((modelAnalysisModelsDir / "parameter_change_rate.xml").string());
    double P1 = rr.getValue("init(P1)");
    EXPECT_EQ(P1, 3);

    rr.simulate();
    P1 = rr.getValue("init(P1)");
    EXPECT_EQ(P1, 3);
    P1 = rr.getValue("P1");
    EXPECT_EQ(P1, 23);

    rr.reset(SelectionRecord::GLOBAL_PARAMETER);
    P1 = rr.getValue("P1");
    EXPECT_EQ(P1, 3);

    //Since it's being changed by a rate rule, it should also be reset by default:
    rr.simulate();
    rr.reset();
    P1 = rr.getValue("P1");
    EXPECT_EQ(P1, 3);

    //Now check what happens when we change the initial value:
    rr.setValue("init(P1)", 10);
    rr.simulate();
    rr.reset(SelectionRecord::GLOBAL_PARAMETER);
    P1 = rr.getValue("P1");
    EXPECT_EQ(P1, 10);

}


TEST_F(ModelAnalysisTests, ResetParamFromEvent) {
    RoadRunner rr((modelAnalysisModelsDir / "parameter_change_event.xml").string());
    double P1 = rr.getValue("init(P1)");
    EXPECT_EQ(P1, 3);

    rr.simulate();
    P1 = rr.getValue("init(P1)");
    EXPECT_EQ(P1, 3);
    P1 = rr.getValue("P1");
    EXPECT_EQ(P1, 4);

    rr.reset(SelectionRecord::GLOBAL_PARAMETER);
    P1 = rr.getValue("P1");
    EXPECT_EQ(P1, 3);

    //Since it's being changed by an event, it should not be reset by default:
    rr.simulate();
    rr.reset();
    P1 = rr.getValue("P1");
    EXPECT_EQ(P1, 4);

    //Now check what happens when we change the initial value:
    rr.setValue("init(P1)", 10);
    rr.simulate();
    rr.reset(SelectionRecord::GLOBAL_PARAMETER);
    P1 = rr.getValue("P1");
    EXPECT_EQ(P1, 10);

}


TEST_F(ModelAnalysisTests, ResetManyCompartments) {
    //Here's the model in Antimony:
    //C1 = 1;
    //E0: at time > 1: C1 = 10;
    //C2: = C4/2
    //C3 = 3
    //C3' = 0.5
    //C4 = 3 + C1
    //C5 = 4 + C1
    //C5' = 1.5
    //C6 = 6
    //E1: at time > 1.1: C6 = 60


    RoadRunner rr((modelAnalysisModelsDir / "many_compartments.xml").string());
    double C1 = rr.getValue("init(C1)");
    EXPECT_EQ(C1, 1);
    double C2 = rr.getValue("init(C2)");
    EXPECT_EQ(C2, 2);
    double C3 = rr.getValue("init(C3)");
    EXPECT_EQ(C3, 3);
    double C4 = rr.getValue("init(C4)");
    EXPECT_EQ(C4, 4);
    double C5 = rr.getValue("init(C5)");
    EXPECT_EQ(C5, 5);
    double C6 = rr.getValue("init(C6)");
    EXPECT_EQ(C6, 6);

    //Simulate and reset all compartments:
    rr.simulate();
    rr.reset(SelectionRecord::COMPARTMENT);
    C1 = rr.getValue("C1");
    EXPECT_EQ(C1, 1);
    C2 = rr.getValue("C2");
    EXPECT_EQ(C2, 2);
    C3 = rr.getValue("C3");
    EXPECT_EQ(C3, 3);
    C4 = rr.getValue("C4");
    EXPECT_EQ(C4, 4);
    C5 = rr.getValue("C5");
    EXPECT_EQ(C5, 5);
    C6 = rr.getValue("C6");
    EXPECT_EQ(C6, 6);

    //Simulate and only reset compartments changed by a rate rule:
    rr.simulate();
    rr.reset();
    C1 = rr.getValue("C1");
    EXPECT_EQ(C1, 10);
    C2 = rr.getValue("C2");
    EXPECT_EQ(C2, 2);
    C3 = rr.getValue("C3");
    EXPECT_EQ(C3, 3);
    C4 = rr.getValue("C4");
    EXPECT_EQ(C4, 4);
    C5 = rr.getValue("C5");
    EXPECT_EQ(C5, 14);
    C6 = rr.getValue("C6");
    EXPECT_EQ(C6, 60);

    //Simulate and only reset compartments changed by a rate rule:
    rr.simulate();
    rr.reset(SelectionRecord::RATE);
    C1 = rr.getValue("C1");
    EXPECT_EQ(C1, 10);
    C2 = rr.getValue("C2");
    EXPECT_EQ(C2, 2);
    C3 = rr.getValue("C3");
    EXPECT_EQ(C3, 3);
    C4 = rr.getValue("C4");
    EXPECT_EQ(C4, 4);
    C5 = rr.getValue("C5");
    EXPECT_EQ(C5, 14);
    C6 = rr.getValue("C6");
    EXPECT_EQ(C6, 60);

    //Now check what happens when we change the initial value:
    rr.setValue("init(C1)", 15);
    rr.simulate();
    rr.reset(SelectionRecord::COMPARTMENT);
    C1 = rr.getValue("C1");
    EXPECT_EQ(C1, 15);
    C2 = rr.getValue("C2");
    EXPECT_EQ(C2, 9);
    C3 = rr.getValue("C3");
    EXPECT_EQ(C3, 3);
    C4 = rr.getValue("C4");
    EXPECT_EQ(C4, 18);
    C5 = rr.getValue("C5");
    EXPECT_EQ(C5, 19);
    C6 = rr.getValue("C6");
    EXPECT_EQ(C6, 6);

}


TEST_F(ModelAnalysisTests, ResetManyFloatingSpecies) {
    RoadRunner rr((modelAnalysisModelsDir / "many_floating_species.xml").string());
    double C1 = rr.getValue("init(C1)");
    EXPECT_EQ(C1, 1);
    double C2 = rr.getValue("init(C2)");
    EXPECT_EQ(C2, 2);
    double C3 = rr.getValue("init(C3)");
    EXPECT_EQ(C3, 3);
    double C4 = rr.getValue("init(C4)");
    EXPECT_EQ(C4, 4);
    double C5 = rr.getValue("init(C5)");
    EXPECT_EQ(C5, 5);
    double C6 = rr.getValue("init(C6)");
    EXPECT_EQ(C6, 6);

    //Simulate and reset all floating species:
    rr.simulate();
    rr.reset(SelectionRecord::FLOATING);
    C1 = rr.getValue("C1");
    EXPECT_EQ(C1, 1);
    C2 = rr.getValue("C2");
    EXPECT_EQ(C2, 2);
    C3 = rr.getValue("C3");
    EXPECT_EQ(C3, 3);
    C4 = rr.getValue("C4");
    EXPECT_EQ(C4, 4);
    C5 = rr.getValue("C5");
    EXPECT_EQ(C5, 5);
    C6 = rr.getValue("C6");
    EXPECT_EQ(C6, 6);

    //Simulate and reset the default, which includes all floating species:
    rr.simulate();
    rr.reset();
    C1 = rr.getValue("C1");
    EXPECT_EQ(C1, 1);
    C2 = rr.getValue("C2");
    EXPECT_EQ(C2, 2);
    C3 = rr.getValue("C3");
    EXPECT_EQ(C3, 3);
    C4 = rr.getValue("C4");
    EXPECT_EQ(C4, 4);
    C5 = rr.getValue("C5");
    EXPECT_EQ(C5, 5);
    C6 = rr.getValue("C6");
    EXPECT_EQ(C6, 6);

    //Simulate and only reset floating species changed by a rate rule:
    rr.simulate();
    rr.reset(SelectionRecord::RATE);
    C1 = rr.getValue("C1");
    EXPECT_EQ(C1, 10);
    C2 = rr.getValue("C2");
    EXPECT_EQ(C2, 2);
    C3 = rr.getValue("C3");
    EXPECT_EQ(C3, 3);
    C4 = rr.getValue("C4");
    EXPECT_EQ(C4, 4);
    C5 = rr.getValue("C5");
    //EXPECT_EQ(C5, 14);
    C6 = rr.getValue("C6");
    EXPECT_EQ(C6, 60);

    //Now check what happens when we change the initial value:
    rr.setValue("init(C1)", 15);
    rr.simulate();
    rr.reset(SelectionRecord::FLOATING);
    C1 = rr.getValue("C1");
    EXPECT_EQ(C1, 15);
    C2 = rr.getValue("C2");
    EXPECT_EQ(C2, 9);
    C3 = rr.getValue("C3");
    EXPECT_EQ(C3, 3);
    C4 = rr.getValue("C4");
    EXPECT_EQ(C4, 18);
    C5 = rr.getValue("C5");
    EXPECT_EQ(C5, 19);
    C6 = rr.getValue("C6");
    EXPECT_EQ(C6, 6);

    //And now check with just the rates:
    rr.reset(SelectionRecord::ALL);
    rr.simulate();
    rr.reset(SelectionRecord::RATE);
    C1 = rr.getValue("C1");
    EXPECT_EQ(C1, 10);
    C2 = rr.getValue("C2");
    EXPECT_EQ(C2, 2);
    C3 = rr.getValue("C3");
    EXPECT_EQ(C3, 3);
    C4 = rr.getValue("C4");
    EXPECT_EQ(C4, 4);
    C5 = rr.getValue("C5");
    EXPECT_EQ(C5, 14);
    C6 = rr.getValue("C6");
    EXPECT_EQ(C6, 60);

}


TEST_F(ModelAnalysisTests, ResetManyBoundarySpecies) {
    RoadRunner rr((modelAnalysisModelsDir / "many_boundary_species.xml").string());
    double C1 = rr.getValue("init(C1)");
    EXPECT_EQ(C1, 1);
    double C2 = rr.getValue("init(C2)");
    EXPECT_EQ(C2, 2);
    double C3 = rr.getValue("init(C3)");
    EXPECT_EQ(C3, 3);
    double C4 = rr.getValue("init(C4)");
    EXPECT_EQ(C4, 4);
    double C5 = rr.getValue("init(C5)");
    EXPECT_EQ(C5, 5);
    double C6 = rr.getValue("init(C6)");
    EXPECT_EQ(C6, 6);

    //Simulate and reset all boundary species:
    rr.simulate();
    rr.reset(SelectionRecord::BOUNDARY);
    C1 = rr.getValue("C1");
    EXPECT_EQ(C1, 1);
    C2 = rr.getValue("C2");
    EXPECT_EQ(C2, 2);
    C3 = rr.getValue("C3");
    EXPECT_EQ(C3, 3);
    C4 = rr.getValue("C4");
    EXPECT_EQ(C4, 4);
    C5 = rr.getValue("C5");
    EXPECT_EQ(C5, 5);
    C6 = rr.getValue("C6");
    EXPECT_EQ(C6, 6);

    //Simulate and reset the default, which includes only boundary species changed by a rate rule:
    rr.simulate();
    rr.reset();
    C1 = rr.getValue("C1");
    EXPECT_EQ(C1, 10);
    C2 = rr.getValue("C2");
    EXPECT_EQ(C2, 2);
    C3 = rr.getValue("C3");
    EXPECT_EQ(C3, 3);
    C4 = rr.getValue("C4");
    EXPECT_EQ(C4, 4);
    C5 = rr.getValue("C5");
    //EXPECT_EQ(C5, 14);
    C6 = rr.getValue("C6");
    EXPECT_EQ(C6, 60);

    //Simulate and only reset boundary species changed by a rate rule:
    rr.simulate();
    rr.reset(SelectionRecord::RATE);
    C1 = rr.getValue("C1");
    EXPECT_EQ(C1, 10);
    C2 = rr.getValue("C2");
    EXPECT_EQ(C2, 2);
    C3 = rr.getValue("C3");
    EXPECT_EQ(C3, 3);
    C4 = rr.getValue("C4");
    EXPECT_EQ(C4, 4);
    C5 = rr.getValue("C5");
    //EXPECT_EQ(C5, 14);
    C6 = rr.getValue("C6");
    EXPECT_EQ(C6, 60);

    //Now check what happens when we change the initial value:
    rr.setValue("init(C1)", 15);
    rr.simulate();
    rr.reset(SelectionRecord::BOUNDARY);
    C1 = rr.getValue("C1");
    EXPECT_EQ(C1, 15);
    C2 = rr.getValue("C2");
    EXPECT_EQ(C2, 9);
    C3 = rr.getValue("C3");
    EXPECT_EQ(C3, 3);
    C4 = rr.getValue("C4");
    EXPECT_EQ(C4, 18);
    C5 = rr.getValue("C5");
    EXPECT_EQ(C5, 19);
    C6 = rr.getValue("C6");
    EXPECT_EQ(C6, 6);

}


TEST_F(ModelAnalysisTests, ResetManyParameters) {
    RoadRunner rr((modelAnalysisModelsDir / "many_parameters.xml").string());
    double C1 = rr.getValue("init(C1)");
    EXPECT_EQ(C1, 1);
    double C2 = rr.getValue("init(C2)");
    EXPECT_EQ(C2, 2);
    double C3 = rr.getValue("init(C3)");
    EXPECT_EQ(C3, 3);
    double C4 = rr.getValue("init(C4)");
    EXPECT_EQ(C4, 4);
    double C5 = rr.getValue("init(C5)");
    EXPECT_EQ(C5, 5);
    double C6 = rr.getValue("init(C6)");
    EXPECT_EQ(C6, 6);

    //Simulate and reset all parameters:
    rr.simulate();
    rr.reset(SelectionRecord::GLOBAL_PARAMETER);
    C1 = rr.getValue("C1");
    EXPECT_EQ(C1, 1);
    C2 = rr.getValue("C2");
    EXPECT_EQ(C2, 2);
    C3 = rr.getValue("C3");
    EXPECT_EQ(C3, 3);
    C4 = rr.getValue("C4");
    EXPECT_EQ(C4, 4);
    C5 = rr.getValue("C5");
    EXPECT_EQ(C5, 5);
    C6 = rr.getValue("C6");
    EXPECT_EQ(C6, 6);

    //Simulate and reset the default, which includes only those parameters changed by a rate rule:
    rr.simulate();
    rr.reset();
    C1 = rr.getValue("C1");
    EXPECT_EQ(C1, 10);
    C2 = rr.getValue("C2");
    EXPECT_EQ(C2, 2);
    C3 = rr.getValue("C3");
    EXPECT_EQ(C3, 3);
    C4 = rr.getValue("C4");
    EXPECT_EQ(C4, 4);
    C5 = rr.getValue("C5");
    EXPECT_EQ(C5, 14);
    C6 = rr.getValue("C6");
    EXPECT_EQ(C6, 60);

    //Simulate and only reset parameters changed by a rate rule:
    rr.simulate();
    rr.reset(SelectionRecord::RATE);
    C1 = rr.getValue("C1");
    EXPECT_EQ(C1, 10);
    C2 = rr.getValue("C2");
    EXPECT_EQ(C2, 2);
    C3 = rr.getValue("C3");
    EXPECT_EQ(C3, 3);
    C4 = rr.getValue("C4");
    EXPECT_EQ(C4, 4);
    C5 = rr.getValue("C5");
    EXPECT_EQ(C5, 14);
    C6 = rr.getValue("C6");
    EXPECT_EQ(C6, 60);

    //Now check what happens when we change the initial value:
    rr.setValue("init(C1)", 15);
    rr.simulate();
    rr.reset(SelectionRecord::GLOBAL_PARAMETER);
    C1 = rr.getValue("C1");
    EXPECT_EQ(C1, 15);
    C2 = rr.getValue("C2");
    EXPECT_EQ(C2, 9);
    C3 = rr.getValue("C3");
    EXPECT_EQ(C3, 3);
    C4 = rr.getValue("C4");
    EXPECT_EQ(C4, 18);
    C5 = rr.getValue("C5");
    EXPECT_EQ(C5, 19);
    C6 = rr.getValue("C6");
    EXPECT_EQ(C6, 6);

}


TEST_F(ModelAnalysisTests, AnalysisFunctionsWithEvent) {
    RoadRunner *rr = new RoadRunner((modelAnalysisModelsDir / "event.xml").string());
    ls::DoubleMatrix jacobian = rr->getFullJacobian();
    EXPECT_EQ(jacobian.CSize(), 2);

    std::vector<ls::Complex> eigens = rr->getFullEigenValues();
    EXPECT_EQ(eigens.size(), 2);

    ls::DoubleMatrix elasticities = rr->getScaledElasticityMatrix();
    EXPECT_EQ(elasticities.CSize(), 2);

    //ls::DoubleMatrix  fluxccm= rr->getScaledFluxControlCoefficientMatrix();
    //CHECK(fluxccm.CSize() == 2);
    //std::cout << "   PASS" << std::endl << std::endl;
    delete rr;
}


TEST_F(ModelAnalysisTests, EigenvalueNamedArrays) {
    RoadRunner* rr = new RoadRunner((modelAnalysisModelsDir / "BIOMD0000000035_url.xml").string());

    ls::DoubleMatrix eigens = rr->getFullEigenValuesNamedArray();
    EXPECT_EQ(eigens.size(), 18);
    EXPECT_EQ(eigens.RSize(), 9);
    EXPECT_EQ(eigens.CSize(), 2);
    for (unsigned int r = 0; r < 9; r++) {
        EXPECT_GT(eigens(r, 0), -105);
        EXPECT_GT(eigens(r, 1), -15);
    }

    delete rr;
}


TEST_F(ModelAnalysisTests, GetEventIDs) {
    RoadRunner *rr = new RoadRunner((modelAnalysisModelsDir / "event.xml").string());

    list<string> eventids;
    rrllvm::LLVMExecutableModel *mod = static_cast<rrllvm::LLVMExecutableModel *>(rr->getModel());
    mod->getEventIds(eventids);
    EXPECT_EQ(eventids.size(), 1);
    EXPECT_EQ("_E0", *eventids.begin());
    delete rr;
}


TEST_F(ModelAnalysisTests, SimulateGillespieMaxRows) {
    RoadRunner *rr = new RoadRunner((modelAnalysisModelsDir / "BIOMD0000000035_url.xml").string());
    rr->setIntegrator("gillespie");
    rr->getIntegrator()->setValue("max_output_rows", 100);
    SimulateOptions opts = rr->getSimulateOptions();

    const ls::DoubleMatrix *results = rr->simulate(&opts);
    EXPECT_EQ(results->RSize(), 100);

    rr->getIntegrator()->setValue("max_output_rows", 1000);
    results = rr->simulate(&opts);
    EXPECT_EQ(results->RSize(), 1000);

    delete rr;
}

TEST_F(ModelAnalysisTests, SimulateGillespieZeroDuration) {
    RoadRunner *rr = new RoadRunner((modelAnalysisModelsDir / "BIOMD0000000035_url.xml").string());
    rr->setIntegrator("gillespie");
    SimulateOptions opts = rr->getSimulateOptions();
    opts.duration = 0;
    opts.steps = 100;

    const ls::DoubleMatrix *results = rr->simulate(&opts);
    EXPECT_EQ(results->RSize(), 100);

    opts.steps = 1000;
    results = rr->simulate(&opts);
    EXPECT_EQ(results->RSize(), 1000);

    delete rr;
}

TEST_F(ModelAnalysisTests, SimulateGillespieDuration) {
    RoadRunner *rr = new RoadRunner((modelAnalysisModelsDir / "BIOMD0000000035_url.xml").string());
    rr->setIntegrator("gillespie");
    SimulateOptions opts = rr->getSimulateOptions();
    opts.duration = 0.5;

    const ls::DoubleMatrix *results = rr->simulate(&opts);
    EXPECT_NEAR(results->Element(results->numRows() - 1, 0), 0.5, 0.0001);

    opts.start = 0;
    opts.duration = 0.7;
    results = rr->simulate(&opts);
    EXPECT_NEAR(results->Element(results->numRows() - 1, 0), 0.7, 0.0001);

    delete rr;
}
