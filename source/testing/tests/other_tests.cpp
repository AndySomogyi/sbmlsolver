#include "unit_test/UnitTest++.h"
#include "rrLogger.h"
#include "rrRoadRunner.h"
#include "rrException.h"
#include "rrStringUtils.h"
using namespace UnitTest;
using namespace rr;

extern string             gSBMLModelsPath;

SUITE(Other)
{
    TEST(JacobianEtcWithEvent)
    {
        std::cout << "Testing Jacobian in model with events..." ;
        RoadRunner* rr = new RoadRunner(gSBMLModelsPath + "/../event_mod.xml");
        ls::DoubleMatrix jacobian = rr->getFullJacobian();
        CHECK(jacobian.CSize() == 2);

        std::vector<ls::Complex> eigens = rr->getFullEigenValues();
        CHECK(eigens.size() == 2);

        ls::DoubleMatrix elasticities = rr->getScaledElasticityMatrix();
        CHECK(elasticities.CSize() == 2);

        //ls::DoubleMatrix  fluxccm= rr->getScaledFluxControlCoefficientMatrix();
        //CHECK(fluxccm.CSize() == 2);
        std::cout << "   PASS" << std::endl << std::endl;
        delete rr;
    }
}


