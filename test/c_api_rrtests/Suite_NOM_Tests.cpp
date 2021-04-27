#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "gtest/gtest.h"
#include "C/rrc_api.h"
#include "rrUtils.h"
#include "rrIniFile.h"
#include "rrException.h"
#include <filesystem>

using namespace std;
using namespace rr;
using namespace rrc;
using std::filesystem::path;

extern path gRRTestDir;
extern path gRROutputDir;
extern RRHandle gRR;
extern IniFile iniFile;

TEST(NOM_TESTS, NOM_TEST_DATA_FILES)
{
    string sec("NOM_TESTS");
    string key("InputFile");

    string NOMFileName = "NOM_Test.dat";

    gRR = createRRInstance();
    NOMFileName = (gRRTestDir /= path("rrtest_files") /= path(NOMFileName)).string();
    ASSERT_TRUE(std::filesystem::exists(NOMFileName));
    ASSERT_TRUE(iniFile.Load(NOMFileName));

    string TestModelFileName;
    //clog << "Loaded test data from file: " << NOMFileName;
    if (iniFile.GetSection(sec))
    {
        IniSection* sbml = iniFile.GetSection(sec);
        IniKey* fNameKey = sbml->GetKey(key);
        if (fNameKey)
        {
            TestModelFileName = (gRRTestDir /= path("rrtest_files") /= fNameKey->mValue).string();
            EXPECT_TRUE(std::filesystem::exists(TestModelFileName));
        }
    }
    EXPECT_TRUE(loadSBMLFromFileE(gRR, TestModelFileName.c_str(), true));
}

TEST(NOM_TESTS, NOM_GET_NAME)
{
    string section("NOM_TESTS");
    string key("ModelName");

    if (iniFile.GetSection(section))
    {
        IniSection* isec = iniFile.GetSection(section);
        IniKey* ikey = isec->GetKey(key);
        if (ikey)
        {
            //======= Calling the function to be tested =============
            string name = getModelName(gRR);

            //======= Check that it worked out
            EXPECT_EQ(ikey->mValue, name);
        }
    }
}

TEST(NOM_TESTS, NOM_GET_NUMBER_OF_RULES)
{
    string section("NOM_TESTS");
    string key("NumberOfRules");
    if (iniFile.GetSection(section))
    {
        IniSection* iSec = iniFile.GetSection(section);
        IniKey* iKey = iSec->GetKey(key);
        if (iKey)
        {
            //======= Calling the function to be tested =============
            int nr = getNumberOfRules(gRR);

            //======= Check that it worked out
            EXPECT_EQ(iKey->AsInt(), nr);
        }
    }
}

TEST(NOM_TESTS, FREE_RR_INSTANCE)
{
    EXPECT_TRUE(freeRRInstance(gRR));
    gRR = NULL;
}
