#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "UnitTest++.h"
#include "rrc_core_api.h"
#include "rrUtils.h"
#include "rrIniFile.h"
#include "rrException.h"

using namespace UnitTest;
using namespace std;
using namespace rr;
using namespace rrc;

extern string   gTempFolder;
extern string   gTestDataFolder;
extern string   gRRInstallFolder;
extern bool     gDebug;

SUITE(NOM_TESTS)
{
string TestDataFileName     = "NOM_Test.dat";
IniFile iniFile;
string TestModelFileName;
RRHandle gRR = NULL;

    TEST(NOM_TEST_DATA_FILES)
    {
    	string sec("NOM_TESTS");
        string key("InputFile");

        gRR                         = createRRInstanceEx(gTempFolder.c_str());
        string testDataFileName     = joinPath(gTestDataFolder, TestDataFileName);
        CHECK(fileExists(testDataFileName));
        CHECK(iniFile.Load(testDataFileName));

        clog<<"Loaded test data from file: "<< testDataFileName;
        if(iniFile.GetSection(sec))
        {
            IniSection* sbml = iniFile.GetSection(sec);
            IniKey* fNameKey = sbml->GetKey(key);
            if(fNameKey)
            {
                TestModelFileName  = joinPath(gTestDataFolder, fNameKey->mValue);
                CHECK(fileExists(TestModelFileName));
            }
        }
        CHECK(loadSBMLFromFileE(gRR, TestModelFileName.c_str(), true));
    }

    TEST(NOM_GET_NAME)
    {
    	string section("NOM_TESTS");
        string key("ModelName");

        if(iniFile.GetSection(section))
        {
            IniSection* isec = iniFile.GetSection(section);
            IniKey* 	ikey = isec->GetKey(key);
            if(ikey)
            {
            	//======= Calling the function to be tested =============
                string name = getModelName(gRR);

                //======= Check that it worked out
                CHECK_EQUAL(ikey->mValue, name);
            }
        }
    }

    TEST(NOM_GET_NUMBER_OF_RULES)
    {
    	string section("NOM_TESTS");
        string key("NumberOfRules");
        if(iniFile.GetSection(section))
        {
            IniSection* iSec = iniFile.GetSection(section);
            IniKey* 	iKey = iSec->GetKey(key);
            if(iKey)
            {
            	//======= Calling the function to be tested =============
                int nr = getNumberOfRules(gRR);

                //======= Check that it worked out
                CHECK_EQUAL(iKey->AsInt() , nr);
            }
        }
    }

}





