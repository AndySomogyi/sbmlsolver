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
}



