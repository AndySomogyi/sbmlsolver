#include "unit_test/UnitTest++.h"
#include "rrLogger.h"
#include "rrRoadRunner.h"
#include "rrException.h"
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
	TEST(AllocateRR)
	{
		if(!gRR)
		{
			gRR = new RoadRunner(gSupportCodeFolder, gCompiler, gTempFolder);
		}

        //Populate models
        gModels.clear();
        gModels.push_back("feedback.xml");
        gModels.push_back("ss_threeSpecies.xml");
        gModels.push_back("ss_TurnOnConservationAnalysis.xml");
        gModels.push_back("squareWaveModel.xml");
        gModels.push_back("test_1.xml");

		CHECK(gRR!=NULL);

		if(gRR)
		{

		}
	}

	TEST(AllocateDeAllocateRR)
	{
        for(int i = 0; i < 100; i++)
        {
            if(gRR)
            {
                delete gRR;
                gRR = NULL;
            }
			gRR = new RoadRunner(gSupportCodeFolder, gCompiler, gTempFolder);
        }

		CHECK(gRR != NULL);
	}

    TEST(VERSIONS)
    {
    	CHECK_EQUAL(getVersion(), 				"1.0.0");
		CHECK_EQUAL(gRR->getlibSBMLVersion(), 	"5.6.0");
    }

    TEST(MODEL_FILES)	//Test that model files for the tests are present
    {
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

	TEST(FULL_JACOBIAN)
	{
		string fName =  JoinPath(gSBMLModelsPath, "ss_threeSpecies.xml");
		CHECK(gRR->loadSBMLFromFile(fName));

		DoubleMatrix jaco = gRR->getFullJacobian();
		const double *jacoMat = jaco.getArray();

		const double jacoExpected[9] = { -0.15, 0, 0, 0, -0.55, 0.4, 0.15, 0, -0.4};

		CHECK_ARRAY_CLOSE(jacoExpected, jacoMat, 9, 0.0000001);
		//Expected result
	//          S1       S2       S3
	//S1{{   -0.15        0        0}
	//S2 {    0.15     -0.4        0}
	//S3 {       0      0.4    -0.55}}
	}
}


