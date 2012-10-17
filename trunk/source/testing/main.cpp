#include <iostream>
#include <fstream>
#include "rrLogger.h"
#include "rrUtils.h"
#include "UnitTest++.h"
#include "XmlTestReporter.h"
#include "TestReporterStdOut.h"
#include "Args.h"
#include "rrGetOptions.h"

using namespace std;
using namespace rr;
using namespace UnitTest;
string gSBMLModelsPath = "";

void ProcessCommandLineArguments(int argc, char* argv[], Args& args);

//call with arguments, -m"modelFilePath" -r"resultFileFolder" -t"TempFolder"
int main(int argc, char* argv[])
{
    Args args;
    ProcessCommandLineArguments(argc, argv, args);

	string outFolder;
    string reportFile("cxx_api_tests.xml");
    reportFile = JoinPath(args.ResultOutputFolder, reportFile);

    gSBMLModelsPath = args.SBMLModelsFilePath;

	fstream aFile;
    aFile.open(reportFile.c_str(), ios::out);
    if(!aFile)
    {
    	cerr<<"Failed opening report file in rr_c_api testing executable.\n";
    	return -1;
    }


	XmlTestReporter reporter1(aFile);
	TestRunner runner1(reporter1);

    runner1.RunTestsIf(Test::GetTestList(), "Base", 		True(), 0);
    runner1.RunTestsIf(Test::GetTestList(), "SteadyState", 	True(), 0);

    runner1.Finish();
    //Made finish public in order to merge result from different test suites.
    //Finish outputs result to xml file

    return 0;
}

void ProcessCommandLineArguments(int argc, char* argv[], Args& args)
{
    char c;
    while ((c = GetOptions(argc, argv, ("m:r:t:"))) != -1)
    {
        switch (c)
        {
            case ('m'): args.SBMLModelsFilePath                       = optarg;                       break;
            case ('r'): args.ResultOutputFolder                       = optarg;                       break;
            case ('t'): args.TempDataFolder        		              = optarg;                       break;
            case ('?'): cout<<Usage(argv[0])<<endl;
            default:
            {
                string str = argv[optind-1];
                if(str != "-?")
                {
                    cout<<"*** Illegal option:\t"<<argv[optind-1]<<" ***\n"<<endl;
                }
                exit(-1);
            }
        }
    }

    //Check arguments, and choose to bail here if something is not right...
    if(argc < 2)
    {
        cout<<Usage(argv[0])<<endl;
        exit(0);
    }
}

#if defined(CG_IDE)
#pragma comment(lib, "roadrunner-static.lib")
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

