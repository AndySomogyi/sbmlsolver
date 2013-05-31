#include <iostream>
#include <string>
#include <vector>
#include "UnitTest++.h"
#include "rrc_api.h"
#include "rrUtils.h"
#include "rrException.h"
#include "src/rrSBMLTestSuiteSimulation_CAPI.h"

using namespace UnitTest;
using namespace std;
using namespace rr;

extern string 	gTSModelsPath;
extern string   gTempFolder;
extern bool		gDebug;

bool RunTest(const string& version, int number);

SUITE(SBML_l2v4)
{
 TEST(15) { CHECK(RunTest("l2v4", 15)); }
 TEST(16) { CHECK(RunTest("l2v4", 16)); }
 TEST(17) { CHECK(RunTest("l2v4", 17)); }
 TEST(18) { CHECK(RunTest("l2v4", 18)); }
 TEST(19) { CHECK(RunTest("l2v4", 19)); }
 TEST(20) { CHECK(RunTest("l2v4", 20)); }
 TEST(21) { CHECK(RunTest("l2v4", 21)); }
 TEST(22) { CHECK(RunTest("l2v4", 22)); }
 TEST(23) { CHECK(RunTest("l2v4", 23)); }
 TEST(24) { CHECK(RunTest("l2v4", 24)); }
 TEST(25) { CHECK(RunTest("l2v4", 25)); }
 TEST(26) { CHECK(RunTest("l2v4", 26)); }
 TEST(27) { CHECK(RunTest("l2v4", 27)); }
 TEST(28) { CHECK(RunTest("l2v4", 28)); }
 TEST(29) { CHECK(RunTest("l2v4", 29)); }
 TEST(30) { CHECK(RunTest("l2v4", 30)); }
 TEST(31) { CHECK(RunTest("l2v4", 31)); }
TEST(889) { CHECK(RunTest("l2v4", 889)); }
TEST(890) { CHECK(RunTest("l2v4", 890)); }
TEST(891) { CHECK(RunTest("l2v4", 891)); }
TEST(892) { CHECK(RunTest("l2v4", 892)); }
TEST(893) { CHECK(RunTest("l2v4", 893)); }
TEST(894) { CHECK(RunTest("l2v4", 894)); }
TEST(895) { CHECK(RunTest("l2v4", 895)); }
TEST(896) { CHECK(RunTest("l2v4", 896)); }
}



