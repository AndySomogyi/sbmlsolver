#include "UnitTest++.h"
#include "rrLogger.h"
#include "rrRoadRunner.h"
#include "rrException.h"
using namespace UnitTest;
using namespace rr;
RoadRunner* gRR = NULL;
const string sbmlModelPath = "r:\\roadrunnerwork\\models";
SUITE(base)
{
	TEST(AllocateRR)
	{
		if(!gRR)
		{
			gRR = new RoadRunner;

		}
		CHECK(gRR!=NULL);
		
	}

	TEST(LOAD_SBML)
	{
		CHECK(gRR!=NULL);
		string fName =  "..\\models\\ss_threeSpecies.xml";
		CHECK_THROW(gRR->loadSBMLFromFile(fName), Exception);	
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


