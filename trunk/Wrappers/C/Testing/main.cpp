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
    string reportFile("c_api_tests.xml");

	if(argc > 1)
    {
		char* path = argv[1];
        outFolder = argv[1];
        reportFile = JoinPath(outFolder, reportFile);
        if(!FolderExists(outFolder))
        {
            return -1;
        }
    }

	fstream aFile;
    aFile.open(reportFile.c_str(), ios::out);
    if(!aFile)
    {
    	return -1;
    }


	XmlTestReporter reporter1(aFile);

	TestRunner runner1(reporter1);

    runner1.RunTestsIf(Test::GetTestList(), "Base", 		True(), 0);
    runner1.RunTestsIf(Test::GetTestList(), "SteadyState", 	True(), 0);

    runner1.Finish();//Made finish public in order to merge result from different test suites

//    DeferredTestReporter::DeferredTestResultList list = reporter1.GetResults();
//	DeferredTestReporter::DeferredTestResultList::iterator iter;
//    for(iter = list.begin(); iter != list.end(); iter++)
//    {
//    	DeferredTestResult& res = (*iter);
//    	cout<<res.suiteName<<" "<<endl;
//    }

	return 0;
}

#if defined(CG_IDE)
#pragma comment(lib, "rr_c_api.lib")
#pragma comment(lib, "roadrunner-static.lib")
#pragma comment(lib, "unit_test-static.lib")
#endif

