#include "unit_test/UnitTest++.h"
#include "rrLogger.h"
#include "rrRoadRunner.h"
#include "rrException.h"
#include "rrStringUtils.h"
using namespace UnitTest;
using namespace rr;

extern RoadRunner* 		gRR;
extern string 			gSBMLModelsPath;
extern string 			gCompiler;
extern string 			gSupportCodeFolder;
extern string 			gTempFolder;
extern vector<string> 	gModels;
SUITE(Base)
{
    TEST(VERSIONS)
    {
    	//Static functions, don't need a handle, (gRR) ?
    	CHECK_EQUAL(getVersion(), 				"1.0.0");
		CHECK_EQUAL(gRR->getlibSBMLVersion(), 	"5.6.0");
    }

	TEST(AllocateRR)
	{
		if(!gRR)
		{
			gRR = new RoadRunner(gSupportCodeFolder, gCompiler, gTempFolder);
		}

		CHECK(gRR!=NULL);
	}

	TEST(AllocateDeAllocateRR)
	{
    	int memoryBefore = 0;
        int memoryAfter  = 10;
        for(int i = 0; i < 1000; i++)
        {
            if(gRR)
            {
                delete gRR;
                gRR = NULL;
            }
			gRR = new RoadRunner(gSupportCodeFolder, gCompiler, gTempFolder);
        }

		//To check this properly, we will need to measure memory before and after somehow..
		CHECK_CLOSE(memoryBefore, memoryAfter, 10);
	}


    TEST(MODEL_FILES)	//Test that model files for the tests are present
    {
    	//Populate models
        gModels.clear();
        gModels.push_back("feedback.xml");
        gModels.push_back("ss_threeSpecies.xml");
        gModels.push_back("ss_TurnOnConservationAnalysis.xml");
        gModels.push_back("squareWaveModel.xml");
        gModels.push_back("test_1.xml");

    	for(int i = 0 ; i < gModels.size(); i++)
        {
    		CHECK(FileExists(JoinPath(gSBMLModelsPath, gModels[i])));
        }
    }

	TEST(LOAD_SBML)
	{
    	for(int i = 0 ; i < gModels.size(); i++)
        {
			string model =  JoinPath(gSBMLModelsPath, gModels[i]);
			CHECK(gRR->loadSBMLFromFile(model));
        }
	}

    TEST(SS_SYMBOLS)
    {
        string model =  JoinPath(gSBMLModelsPath, "ss_threeSpecies.xml");
        gRR->ComputeAndAssignConservationLaws(true);
		CHECK(gRR->loadSBMLFromFile(model));
    	NewArrayList aList = gRR->getAvailableSteadyStateSymbols();

        string symbols = aList.AsString();
		string expectedSymbols = "{{\"Floating Species\",{\"S1\",\"S3\",\"S2\"}},{\"Boundary Species\",{\"Xo\",\"X1\"}},{\"Floating Species (amount)\",{\"[S1]\",\"[S3]\",\"[S2]\"}},{\"Boundary Species (amount)\",{\"[Xo]\",\"[X1]\"}},{\"Global Parameters\",{\"k1\",\"k2\",\"k3\",\"k4\"}},{\"Volumes\",{\"compartment\"}},{\"Fluxes\",{\"_J1\",\"_J2\",\"_J3\",\"_J4\"}},{\"Flux Control Coefficients\",{{\"_J1\",{\"CC:_J1,k1\",\"CC:_J1,k2\",\"CC:_J1,k3\",\"CC:_J1,k4\",\"CC:_J1,Xo\",\"CC:_J1,X1\"}},{\"_J2\",{\"CC:_J2,k1\",\"CC:_J2,k2\",\"CC:_J2,k3\",\"CC:_J2,k4\",\"CC:_J2,Xo\",\"CC:_J2,X1\"}},{\"_J3\",{\"CC:_J3,k1\",\"CC:_J3,k2\",\"CC:_J3,k3\",\"CC:_J3,k4\",\"CC:_J3,Xo\",\"CC:_J3,X1\"}},{\"_J4\",{\"CC:_J4,k1\",\"CC:_J4,k2\",\"CC:_J4,k3\",\"CC:_J4,k4\",\"CC:_J4,Xo\",\"CC:_J4,X1\"}}}},{\"Concentration Control Coefficients\",{{\"S1\",{\"CC:S1,k1\",\"CC:S1,k2\",\"CC:S1,k3\",\"CC:S1,k4\",\"CC:S1,Xo\",\"CC:S1,X1\"}},{\"S3\",{\"CC:S3,k1\",\"CC:S3,k2\",\"CC:S3,k3\",\"CC:S3,k4\",\"CC:S3,Xo\",\"CC:S3,X1\"}},{\"S2\",{\"CC:S2,k1\",\"CC:S2,k2\",\"CC:S2,k3\",\"CC:S2,k4\",\"CC:S2,Xo\",\"CC:S2,X1\"}}}},{\"Unscaled Concentration Control Coefficients\",{{\"S1\",{\"uCC:S1,k1\",\"uCC:S1,k2\",\"uCC:S1,k3\",\"uCC:S1,k4\",\"uCC:S1,Xo\",\"uCC:S1,X1\"}},{\"S3\",{\"uCC:S3,k1\",\"uCC:S3,k2\",\"uCC:S3,k3\",\"uCC:S3,k4\",\"uCC:S3,Xo\",\"uCC:S3,X1\"}},{\"S2\",{\"uCC:S2,k1\",\"uCC:S2,k2\",\"uCC:S2,k3\",\"uCC:S2,k4\",\"uCC:S2,Xo\",\"uCC:S2,X1\"}}}},{\"Elasticity Coefficients\",{{\"_J1\",{\"EE:_J1,S1\",\"EE:_J1,S3\",\"EE:_J1,S2\",\"EE:_J1,Xo\",\"EE:_J1,X1\",\"EE:_J1,k1\",\"EE:_J1,k2\",\"EE:_J1,k3\",\"EE:_J1,k4\"}},{\"_J2\",{\"EE:_J2,S1\",\"EE:_J2,S3\",\"EE:_J2,S2\",\"EE:_J2,Xo\",\"EE:_J2,X1\",\"EE:_J2,k1\",\"EE:_J2,k2\",\"EE:_J2,k3\",\"EE:_J2,k4\"}},{\"_J3\",{\"EE:_J3,S1\",\"EE:_J3,S3\",\"EE:_J3,S2\",\"EE:_J3,Xo\",\"EE:_J3,X1\",\"EE:_J3,k1\",\"EE:_J3,k2\",\"EE:_J3,k3\",\"EE:_J3,k4\"}},{\"_J4\",{\"EE:_J4,S1\",\"EE:_J4,S3\",\"EE:_J4,S2\",\"EE:_J4,Xo\",\"EE:_J4,X1\",\"EE:_J4,k1\",\"EE:_J4,k2\",\"EE:_J4,k3\",\"EE:_J4,k4\"}}}},{\"Unscaled Elasticity Coefficients\",{{\"_J1\",{\"uEE:_J1,S1\",\"uEE:_J1,S3\",\"uEE:_J1,S2\",\"uEE:_J1,Xo\",\"uEE:_J1,X1\",\"uEE:_J1,k1\",\"uEE:_J1,k2\",\"uEE:_J1,k3\",\"uEE:_J1,k4\"}},{\"_J2\",{\"uEE:_J2,S1\",\"uEE:_J2,S3\",\"uEE:_J2,S2\",\"uEE:_J2,Xo\",\"uEE:_J2,X1\",\"uEE:_J2,k1\",\"uEE:_J2,k2\",\"uEE:_J2,k3\",\"uEE:_J2,k4\"}},{\"_J3\",{\"uEE:_J3,S1\",\"uEE:_J3,S3\",\"uEE:_J3,S2\",\"uEE:_J3,Xo\",\"uEE:_J3,X1\",\"uEE:_J3,k1\",\"uEE:_J3,k2\",\"uEE:_J3,k3\",\"uEE:_J3,k4\"}},{\"_J4\",{\"uEE:_J4,S1\",\"uEE:_J4,S3\",\"uEE:_J4,S2\",\"uEE:_J4,Xo\",\"uEE:_J4,X1\",\"uEE:_J4,k1\",\"uEE:_J4,k2\",\"uEE:_J4,k3\",\"uEE:_J4,k4\"}}}},{\"Eigenvalues\",{\"eigen_S1\",\"eigen_S3\",\"eigen_S2\"}}}";
        CHECK_EQUAL(expectedSymbols, symbols);
    }

//	TEST(FULL_JACOBIAN)
//	{
//		if(!gRR)
//		{
////			gRR = new RoadRunner(gSupportCodeFolder, gCompiler, gTempFolder);
//		}
//
//		string fName =  JoinPath(gSBMLModelsPath, "ss_threeSpecies.xml");
//		CHECK(gRR->loadSBMLFromFile(fName));
//
//		DoubleMatrix jaco = gRR->getFullJacobian();
//		const double *jacoMat = jaco.getArray();
//
//		const double jacoExpected[9] = { -0.15, 0, 0, 0, -0.55, 0.4, 0.15, 0, -0.4};
//
//		CHECK_ARRAY_CLOSE(jacoExpected, jacoMat, 9, 0.0000001);
//		//Expected result
//	//          S1       S2       S3
//	//S1{{   -0.15        0        0}
//	//S2 {    0.15     -0.4        0}
//	//S3 {       0      0.4    -0.55}}
//	}
}


