#include "unit_test/UnitTest++.h"
#include "rrLogger.h"
#include "SBMLSolver.h"
#include "rrException.h"
#include "rrStringUtils.h"
using namespace UnitTest;
using namespace rr;

extern SBMLSolver*         gRR;
extern string             gSBMLModelsPath;
extern string             gCompiler;
extern string             gSupportCodeFolder;
extern string             gTempFolder;
extern vector<string>     gModels;
SUITE(Stoichiometric)
{
    TEST(AllocateRR)
    {
        if(!gRR)
        {
            gRR = new SBMLSolver(gCompiler, gTempFolder, gSupportCodeFolder);
        }

        CHECK(gRR!=NULL);
    }

    TEST(MODEL_FILES)    //Test that model files for the tests are present
    {
    }
}


