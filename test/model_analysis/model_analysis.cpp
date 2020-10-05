#include "rrLogger.h"
#include "rrRoadRunner.h"
#include "rrException.h"
#include "rrStringUtils.h"
#include "gtest/gtest.h"

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
    std::cout << "   PASS" << std::endl << std::endl;
    delete rr;
}


