#include <iostream>
#include <fstream>
#include "rrLogger.h"
#include "rrUtils.h"
#include "unit_test/UnitTest++.h"
#include "unit_test/XmlTestReporter.h"
#include "unit_test/TestReporterStdout.h"
#include "rrc_api.h"
#include "rrGetOptions.h"
#include "src/Args.h"
#include "rrRoadRunner.h"
#include "rrConfig.h"

#include "Suite_TestModel.h"

using namespace std;
using namespace rr;
using namespace rrc;
using namespace UnitTest;
using std::string;

string     gTempFolder              = "";
string     gRRInstallFolder         = "";
string     gTestDataFolder          = "";
bool       gDebug                   = false;
string     gTSModelsPath;
string     gCompiler                = "";

void ProcessCommandLineArguments(int argc, char* argv[], Args& args);
bool setup(Args& args);


//call with arguments, -m"modelFilePath" -r"resultFileFolder" -t"TempFolder" -s"Suites"
int main(int argc, char* argv[])
{
    cout << "RoadRunner C API Test" << endl;

	//string sbmlFilepath = "C:\\Users\\Wilbert\\Desktop\\model12-4.xml";
	string sbmlFilepath = "C:\\vs\\src\\roadrunner\\models\\feedback.xml";

	RRHandle _handle = createRRInstance();
	char*_s;
	
	loadSBMLFromFile(_handle, sbmlFilepath.c_str());
	//_s = getSBML(_handle);

	RRCDataPtr result;

	result = simulateEx(_handle, 0, 10, 100);
	printf(rrCDataToString(result));
	freeRRCData(result);

	
	char a;
	cin >> a;
	
    return 0;
}



