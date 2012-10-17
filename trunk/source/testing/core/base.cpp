#include "UnitTest++.h"
#include "rrLogger.h"
#include "rrRoadRunner.h"
#include "rrException.h"
using namespace UnitTest;
using namespace rr;
RoadRunner* gRR = NULL;
extern string gSBMLModelsPath;

SUITE(Base)
{
	TEST(AllocateRR)
	{
		if(!gRR)
		{
			gRR = new RoadRunner;

		}
		CHECK(gRR!=NULL);

	}

    TEST(VERSIONS)
    {
    	CHECK_EQUAL(getVersion(), 				"1.0.0");
		CHECK_EQUAL(gRR->getlibSBMLVersion(), 	"5.6.0");
    }

    TEST(MODEL_FILES)	//Test that model files for the tests are present
    {
    	CHECK(FileExists(JoinPath(gSBMLModelsPath, "feedback.xml")));
    	CHECK(FileExists(JoinPath(gSBMLModelsPath, "ss_threeSpecies.xml")));
        CHECK(FileExists(JoinPath(gSBMLModelsPath, "ss_TurnOnConservationAnalysis.xml")));
        CHECK(FileExists(JoinPath(gSBMLModelsPath, "squareWaveModel.xml")));
    }

	TEST(LOAD_SBML)
	{
		CHECK(gRR!=NULL);
		string model =  JoinPath(gSBMLModelsPath, "ss_threeSpecies.xml");

		CHECK(gRR->loadSBMLFromFile(model));
	}


	//TEST(FULL_JACOBIAN)
	//{
	//	//Fail a test
	//	CHECK(gRR==NULL);

	//	string fName =  JoinPath(sbmlModelPath, "ss_threeSpecies.xml");
	//	gRR->loadSBMLFromFile(fName);
	//	CHECK(gRR->loadSBMLFromFile(fName));

	//	DoubleMatrix jaco = gRR->getFullJacobian();
	//	//Expected result
	//	Log(lInfo)<<jaco;
	////          S1       S2       S3
	////S1{{   -0.15        0        0}
	////S2 {    0.15     -0.4        0}
	////S3 {       0      0.4    -0.55}}

	//}
}


