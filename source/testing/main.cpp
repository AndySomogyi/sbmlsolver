#include <iostream>
#include <fstream>
#include "rrLogger.h"
#include "rrUtils.h"
#include "unit_test/UnitTest++.h"
#include "unit_test/XmlTestReporter.h"
#include "unit_test/TestReporterStdout.h"
#include "Args.h"
#include "rrRoadRunner.h"
#include "rrGetOptions.h"

using namespace std;
using namespace rr;
using namespace UnitTest;

RoadRunner* gRR = NULL;

string gSBMLModelsPath 		= "";
string gCompiler 			= "";
string gSupportCodeFolder 	= "";
string gTempFolder		   	= "";

#if defined(_WIN32) || defined(WIN32)
//Test suite
string gTSModelsPath 		= "r:\\SBMLTS\\cases\\semantic";
string gTSDataOutPutFolder 	= "r:\\RRTesting\\DataOutput\\xe1";
#else
string gTSModelsPath 		= "/r/SBMLTS/cases/semantic";
string gTSDataOutPutFolder 	= "/r/RRTesting/DataOutput/gcc";
#endif

vector<string> gModels;
void ProcessCommandLineArguments(int argc, char* argv[], Args& args);

int main(int argc, char* argv[])
{
    Args args;
    ProcessCommandLineArguments(argc, argv, args);

   	bool doLogging  	= args.EnableLogging;

    if(doLogging)
    {
        gLog.Init("", lDebug1, unique_ptr<LogFile>(new LogFile("testing.log")));
        LogOutput::mLogToConsole = true;
    }
	string reportFile(args.ResultOutputFile);

    gSBMLModelsPath 	= args.SBMLModelsFilePath;
	gCompiler	 		= args.Compiler;
    gTempFolder			= args.TempDataFolder;
	gSupportCodeFolder 	= args.SupportCodeFolder;

	Log(lDebug) << "Support code folder is set to:"<<gSupportCodeFolder;
 	fstream aFile(reportFile.c_str(), ios::out);
    if(!aFile)
    {
		cerr<<"Failed opening report file: "<<reportFile<<" in rr_cpp_api testing executable.\n";
    	return -1;
    }

	XmlTestReporter reporter1(aFile);
	TestRunner runner1(reporter1);

    runner1.RunTestsIf(Test::GetTestList(), "Base", 		True(), 0);
    runner1.RunTestsIf(Test::GetTestList(), "SteadyState", 	True(), 0);
    runner1.RunTestsIf(Test::GetTestList(), "SBML_l2v4",   	True(), 0);

    //Finish outputs result to xml file
    runner1.Finish();
    return 0;
}

void ProcessCommandLineArguments(int argc, char* argv[], Args& args)
{
    char c;
    while ((c = GetOptions(argc, argv, ("vi:m:l:r:s:t:"))) != -1)
    {
        switch (c)
        {
            case ('m'): args.SBMLModelsFilePath                     = rrOptArg;                       break;
			case ('l'): args.Compiler      			                = rrOptArg;                       break;
            case ('r'): args.ResultOutputFile                       = rrOptArg;                       break;
			case ('s'): args.SupportCodeFolder     		            = rrOptArg;                       break;
			case ('t'): args.TempDataFolder        		            = rrOptArg;                       break;
			case ('v'): args.EnableLogging        		            = true;                       break;
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
#pragma comment(lib, "roadrunner-static.lib")
//#pragma comment(lib, "sundials_cvode.lib")
//#pragma comment(lib, "sundials_nvecserial.lib")
//#pragma comment(lib, "nleq-static.lib")
//#pragma comment(lib, "rr-libstruct-static.lib")
//#pragma comment(lib, "libsbml-static.lib")
//#pragma comment(lib, "libxml2_xe.lib")
//#pragma comment(lib, "blas.lib")
//#pragma comment(lib, "lapack.lib")
//#pragma comment(lib, "libf2c.lib")
#pragma comment(lib, "unit_test-static.lib")
//#pragma comment(lib, "poco_foundation-static.lib")
#endif

