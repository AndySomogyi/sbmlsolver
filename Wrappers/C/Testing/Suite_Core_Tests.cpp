#include <iostream>
#include <string>
#include <vector>
#include "Poco/File.h"
#include "UnitTest++.h"
#include "rr_c_api.h"
#include "rrUtils.h"
#include "rrException.h"

using namespace UnitTest;
using namespace std;
using namespace rr;
using namespace rr_c_api;
using namespace Poco;

extern string   gTempFolder;
extern string   gTestDataFolder;
extern bool		gDebug;

SUITE(CORE_TESTS)
{
	TEST(COMPILER)
	{
    	RRHandle aRR 		  		= createRRInstanceE(gTempFolder.c_str());
        //Copy test model sources to temp folder, and compile there

		Poco::File headerFile(JoinPath(gTestDataFolder, "ModelSourceTest.h"));
		Poco::File sourceFile(JoinPath(gTestDataFolder, "ModelSourceTest.c"));
        headerFile.copyTo(gTempFolder);
        sourceFile.copyTo(gTempFolder);

        string testSource = JoinPath(gTempFolder, "ModelSourceTest.c");
		compileSource(aRR, testSource.c_str());
        freeRRInstance(aRR);
	}

    TEST(RELOADING_MODEL_MODEL_RECOMPILIATION)
    {
    	RRHandle aRR 		  		= createRRInstanceE(gTempFolder.c_str());
		string TestModelFileName 	= JoinPath(gTestDataFolder, "Test_1.xml");
		CHECK(FileExists(TestModelFileName));

		CHECK(loadSBMLFromFileE(aRR, TestModelFileName.c_str(), true));

        //Load the same model again, but do not recompile the model DLL..
		CHECK(loadSBMLFromFileE(aRR, TestModelFileName.c_str(), true));
        freeRRInstance(aRR);
    }

    TEST(RELOADING_MODEL_NO_MODEL_RECOMPILIATION)
    {
    	RRHandle aRR 		  		= createRRInstanceE(gTempFolder.c_str());
		string TestModelFileName 	= JoinPath(gTestDataFolder, "Test_1.xml");
		CHECK(FileExists(TestModelFileName));

		CHECK(loadSBMLFromFileE(aRR, TestModelFileName.c_str(), true));

        //Load the same model again, but do not recompile the model DLL..
		CHECK(loadSBMLFromFileE(aRR, TestModelFileName.c_str(), false));
        freeRRInstance(aRR);
    }

    TEST(LOADING_MODEL_MULTIPLE_INSTANCES)
    {
    	RRHandle aRR1 		  		= createRRInstanceE(gTempFolder.c_str());
    	RRHandle aRR2 		  		= createRRInstanceE(gTempFolder.c_str());
		string TestModelFileName 	= JoinPath(gTestDataFolder, "Test_1.xml");

		CHECK(FileExists(TestModelFileName));

		CHECK(loadSBMLFromFileE(aRR1, TestModelFileName.c_str(), true));
		CHECK(loadSBMLFromFileE(aRR2, TestModelFileName.c_str(), true));

        //Load the same model again, but do not recompile the model DLL..
        CHECK(loadSBMLFromFileE(aRR1, TestModelFileName.c_str(), false));
        CHECK(loadSBMLFromFileE(aRR2, TestModelFileName.c_str(), false));


        freeRRInstance(aRR1);
        freeRRInstance(aRR2);
    }

}



