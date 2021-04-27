#include "gtest/gtest.h"
#include "../test_util.h"
#include "C/rrc_api.h"
#include "rrLogger.h"
#include "rrUtils.h"
#include "rrIniFile.h"

#include <string>
#include <filesystem>

//Add using clauses..
using namespace std;
using namespace rrc;
using std::filesystem::path;

extern path gRRTestDir;
extern path gRROutputDir;
extern rr::IniFile iniFile;
extern RRHandle gRR;

//This tests is mimicking the Python tests
extern path TestModelFileName;

TEST(C_API_EXCEPTIONS, DATA_FILES)
{
    path testDataFileName = rrTestDir_ / "models/C_API_CORE/TestModel_1.dat";

    ASSERT_TRUE(std::filesystem::exists(testDataFileName));
    ASSERT_TRUE(iniFile.Load(testDataFileName.string()));

    //clog << "Loaded test data from file: " << testDataFileName;
    if (iniFile.GetSection("SBML_FILE"))
    {
        rr::IniSection* sbml = iniFile.GetSection("SBML_FILE");
        rr::IniKey* fNameKey = sbml->GetKey("sbmlFile");
        if (fNameKey)
        {
            TestModelFileName = path(rrTestDir_.string() + "models/C_API_CORE/") / fNameKey->mValue;
            EXPECT_TRUE(std::filesystem::exists(TestModelFileName));
        }
    }
    else
    {
        EXPECT_TRUE(false);
    }
}

TEST(C_API_EXCEPTIONS, LOAD_SBML)
{
    EXPECT_TRUE(loadSBMLFromFile(gRR, TestModelFileName.string().c_str()));
}

TEST(C_API_EXCEPTIONS, SET_COMPUTE_AND_ASSIGN_CONSERVATION_LAWS)
{
    ASSERT_TRUE(gRR != NULL);
    bool res = setComputeAndAssignConservationLaws(gRR, true);
    EXPECT_TRUE(res);
    clog << "\nConversation laws: " << res << endl;
}

TEST(C_API_EXCEPTIONS, GET_UNSCALED_ELASTICITY_COEFFICIENT)
{
    double test;
    bool val = getuEE(gRR, "J1", "S1", &test);

    // val = getuEE("J1", "S34", test);

     //val =  getuCC("J1", "S1", test);

}
