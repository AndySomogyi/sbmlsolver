#include "unit_test/UnitTest++.h"
#include "rrLogger.h"
#include "rrRoadRunner.h"
#include "rrException.h"
#include "rrStringUtils.h"
#include "rrUtils.h"
#include "rrVersionInfo.h"

using namespace UnitTest;
using namespace rr;

extern RoadRunner*        gRR;
extern string             gSBMLModelsPath;
extern string             gCompiler;
extern string             gSupportCodeFolder;
extern string             gRRInstallFolder;
extern string             gTempFolder;
extern vector<string>     gModels;
SUITE(Base)
{
    TEST(VERSIONS)
    {
        //Static functions, don't need a handle, (gRR) ?
        CHECK_EQUAL(rr::getVersionStr(VERSIONSTR_BASIC), "1.0.0");
        CHECK_EQUAL(rr::getVersionStr(VERSIONSTR_LIBSBML), "LibSBML Version: 5.6.0");
    }

    TEST(AllocateRR)
    {


        if(!gRR)
        {
            gRR = new RoadRunner(gCompiler, gTempFolder, gSupportCodeFolder);
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
            gRR = new RoadRunner(gCompiler, gTempFolder, gSupportCodeFolder);
        }

        //To check this properly, we will need to measure memory before and after somehow..
        CHECK_CLOSE(memoryBefore, memoryAfter, 10);
    }

    TEST(MODEL_FILES)    //Test that model files for the tests are present
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
            CHECK(fileExists(joinPath(gSBMLModelsPath, gModels[i])));
        }
    }

    TEST(LOAD_SBML)
    {
        for(int i = 0 ; i < gModels.size(); i++)
        {
            string model =  joinPath(gSBMLModelsPath, gModels[i]);
            gRR->load(model);
        }
    }
}


