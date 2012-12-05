#include <iostream>
#include <fstream>
#include "rrUtils.h"
#include "UnitTest++.h"
#include "XmlTestReporter.h"
#include "TestReporterStdOut.h"
#include "Args.h"
#include "rrGetOptions.h"
#include "rr_c_api.h"

using namespace std;
using namespace rr;
using namespace UnitTest;
string gSBMLModelsPath = "";
string gCompilerPath = "";
string gSupportCodeFolder = "";

void ProcessCommandLineArguments(int argc, char* argv[], Args& args);

RRHandle gRR = NULL;

//call with arguments, -m"modelFilePath" -r"resultFileFolder" -t"TempFolder"
int main(int argc, char* argv[])
{
    Args args;
    ProcessCommandLineArguments(argc, argv, args);

	string outFolder;
    string reportFile;
	reportFile = args.ResultOutputFile;

    gSBMLModelsPath = args.SBMLModelsFilePath;
	gCompilerPath = args.CompilerLocation;
	gSupportCodeFolder = args.SupportCodeFolder;

	fstream aFile;
    aFile.open(reportFile.c_str(), ios::out);
    if(!aFile)
    {
    	cerr<<"Failed opening report file: "<<reportFile<<" in rr_c_api testing executable.\n";
    	return -1;
    }

	XmlTestReporter reporter1(aFile);
	TestRunner runner1(reporter1);

    runner1.RunTestsIf(Test::GetTestList(), "Base", 		True(), 0);
    runner1.RunTestsIf(Test::GetTestList(), "SteadyState", 	True(), 0);

    runner1.Finish();
    return 0;
}

void ProcessCommandLineArguments(int argc, char* argv[], Args& args)
{
    char c;
    while ((c = GetOptions(argc, argv, ("m:l:r:s:t:"))) != -1)
    {
        switch (c)
        {
            case ('m'): args.SBMLModelsFilePath                     = rrOptArg;                       break;
			case ('l'): args.CompilerLocation                       = rrOptArg;                       break;
            case ('r'): args.ResultOutputFile                       = rrOptArg;                       break;
			case ('s'): args.SupportCodeFolder     		            = rrOptArg;                       break;
			case ('t'): args.TempDataFolder        		            = rrOptArg;                       break;
            case ('?'): cout<<Usage(argv[0])<<endl;
            default:
            {
                string str = argv[rrOptInd-1];
                if(str != "-?")
                {
                    cout<<"*** Illegal option:\t"<<argv[rrOptInd-1]<<" ***\n"<<endl;
                }
                exit(-1);
            }
        }
    }

    //Check arguments, and choose to bail here if something is not right...
    if(argc < 2)
    {
        cout<<Usage(argv[0])<<endl;
        exit(-1);
    }
}

#if defined(CG_IDE)
#pragma comment(lib, "rr_c_api.lib")
#pragma comment(lib, "roadrunner-static.lib")
#pragma comment(lib, "unit_test-static.lib")
#endif

