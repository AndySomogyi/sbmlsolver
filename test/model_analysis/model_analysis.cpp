#include "rrLogger.h"
#include "rrRoadRunner.h"
#include "rrException.h"
#include "rrStringUtils.h"
#include "gtest/gtest.h"
#include "LLVMExecutableModel.h"

using namespace rr;

extern string gRRTestDir;
extern string gRROutputDir;

TEST(MODEL_ANALYSIS, AnalysisFunctionsWithEvent)
{
    RoadRunner* rr = new RoadRunner(gRRTestDir + "/models/MODEL_ANALYSIS/event.xml");
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


TEST(MODEL_ANALYSIS, GetEventIDs)
{
    RoadRunner* rr = new RoadRunner(gRRTestDir + "/models/MODEL_ANALYSIS/event.xml");
    list<string> eventids;
    rrllvm::LLVMExecutableModel* mod = static_cast<rrllvm::LLVMExecutableModel*>(rr->getModel());
    mod->getEventIds(eventids);
    EXPECT_EQ(eventids.size(), 1);
    EXPECT_EQ("_E0", *eventids.begin());
    delete rr;
}


TEST(MODEL_ANALYSIS, SimulateGillespieMaxRows)
{
    RoadRunner* rr = new RoadRunner(gRRTestDir + "/models/MODEL_ANALYSIS/BIOMD0000000035_url.xml");
    rr->setIntegrator("gillespie");
    rr->getIntegrator()->setValue("max_output_rows", 100);
    SimulateOptions opts = rr->getSimulateOptions();

    const ls::DoubleMatrix* results = rr->simulate(&opts);
    EXPECT_EQ(results->RSize(), 100);

    rr->getIntegrator()->setValue("max_output_rows", 1000);
    results = rr->simulate(&opts);
    EXPECT_EQ(results->RSize(), 1000);

    delete rr;
}

TEST(MODEL_ANALYSIS, SimulateGillespieZeroDuration)
{
    RoadRunner* rr = new RoadRunner(gRRTestDir + "/models/MODEL_ANALYSIS/BIOMD0000000035_url.xml");
    rr->setIntegrator("gillespie");
    SimulateOptions opts = rr->getSimulateOptions();
    opts.duration = 0;
    opts.steps = 100;

    const ls::DoubleMatrix* results = rr->simulate(&opts);
    EXPECT_EQ(results->RSize(), 100);

    opts.steps = 1000;
    results = rr->simulate(&opts);
    EXPECT_EQ(results->RSize(), 1000);

    delete rr;
}

TEST(MODEL_ANALYSIS, SimulateGillespieDuration)
{
    RoadRunner* rr = new RoadRunner(gRRTestDir + "/models/MODEL_ANALYSIS/BIOMD0000000035_url.xml");
    rr->setIntegrator("gillespie");
    SimulateOptions opts = rr->getSimulateOptions();
    opts.duration = 0.5;

    const ls::DoubleMatrix* results = rr->simulate(&opts);
    EXPECT_NEAR(results->Element(results->numRows()-1, 0), 0.5, 0.0001);

    opts.start = 0;
    opts.duration = 0.7;
    results = rr->simulate(&opts);
    EXPECT_NEAR(results->Element(results->numRows() - 1, 0), 0.7, 0.0001);

    delete rr;
}
