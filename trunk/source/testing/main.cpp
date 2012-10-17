#include <iostream>
#include <fstream>
#include "rrLogger.h"
#include "rrUtils.h"
#include "UnitTest++.h"
#include "XmlTestReporter.h"
#include "TestReporterStdOut.h"

using namespace std;
using namespace rr;
using namespace UnitTest;

int main(int argc, char* argv[])
{
	string outFolder;
    string reportFile("cxx_api_tests.xml");

    cout<<"Running tests...";
	if(argc > 1)
    {
		char* path = argv[1];
        outFolder = argv[1];
        reportFile = JoinPath(outFolder, reportFile);
        if(!FolderExists(outFolder))
        {
        	cout<<"The folder: "<<outFolder<<" do not exist.."<<endl;
            return -1;
        }
    }

	fstream aFile;
    aFile.open(reportFile.c_str(), ios::out);
    if(!aFile)
    {
    	return -1;
    }

	XmlTestReporter reporter(aFile);

	LogOutput::mLogToConsole = true;
    gLog.SetCutOffLogLevel(lDebug4);

	//TestReporterStdout stdreporter;
	//TestRunner runner(stdreporter);
	//runner.RunTestsIf(UnitTest::Test::GetTestList(), NULL, UnitTest::True(),0);

    TestRunner xml_runner(reporter);
	xml_runner.RunTestsIf(UnitTest::Test::GetTestList(), NULL, UnitTest::True(),0);
	
	return 0;
}

#if defined(CG_IDE)
#pragma comment(lib, "rr_c_api.lib")
#pragma comment(lib, "roadrunner.lib")
#pragma comment(lib, "sundials_cvode.lib")
#pragma comment(lib, "sundials_nvecserial.lib")
#pragma comment(lib, "nleq-static.lib")
#pragma comment(lib, "rr-libstruct-static.lib")
#pragma comment(lib, "libsbml-static.lib")
#pragma comment(lib, "libxml2_xe.lib")
#pragma comment(lib, "blas.lib")
#pragma comment(lib, "lapack.lib")
#pragma comment(lib, "libf2c.lib")
#pragma comment(lib, "unit_test-static.lib")
#endif

