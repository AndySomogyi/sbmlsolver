#include <iostream>
#include <fstream>
#include "rrLogger.h"
#include "rrUtils.h"
#include "unit_test/UnitTest++.h"
#include "unit_test/XmlTestReporter.h"
#include "unit_test/TestReporterStdout.h"
#include "Args.h"
#include "rr_c_api.h"
#include "rrGetOptions.h"

using namespace std;
using namespace rr;
using namespace UnitTest;


string 	gSBMLModelsPath 		= "";
string 	gCompiler 				= "";
string 	gSupportCodeFolder 		= "";
string 	gTempFolder		   		= "";
string 	gRRInstallFolder 		= "";
string  gTestDataFolder			= "";
bool	gDebug			    	= false;

// initialized based on gSBMLModelsPath
string gTSModelsPath;

vector<string> gModels;
void ProcessCommandLineArguments(int argc, char* argv[], Args& args);

RRHandle gRR = NULL;

//call with arguments, -m"modelFilePath" -r"resultFileFolder" -t"TempFolder"
int main(int argc, char* argv[])
{
    Args args;
    ProcessCommandLineArguments(argc, argv, args);

	string reportFile(args.ResultOutputFile);

    string thisExeFolder = getCurrentExeFolder();
    clog<<"RoadRunner bin location is: "<<thisExeFolder;

#if defined(_MSC_VER) 
	{
		thisExeFolder = "r:\\installs\\vs\\2010\\debug\\bin";
	}
#endif
    //Assume(!) this is the bin folder of roadrunner install
	gRRInstallFolder = getParentFolder(thisExeFolder);	//Go up one folder
    gDebug				= args.EnableLogging;
    gSBMLModelsPath 	= args.SBMLModelsFilePath;
    gTempFolder			= args.TempDataFolder;
    gCompiler	 		= JoinPath(gRRInstallFolder, gCompiler);
	gSupportCodeFolder 	= JoinPath(gRRInstallFolder, "rr_support");
	gTestDataFolder     = JoinPath(gRRInstallFolder, "tests");

    //We need a rr handle to enable initial logging...
    gRR = getRRInstance();

    if(gDebug && gRR)
    {
	    enableLogging();
        setLogLevel("Debug5");
    }
    else
    {
      setLogLevel("Error");
    }
    // set model path (read from cmd line)
    gTSModelsPath = JoinPath(JoinPath(gSBMLModelsPath, "cases"), "semantic");

 	fstream aFile(reportFile.c_str(), ios::out);
    if(!aFile)
    {
    	cerr<<"Failed opening report file: "<<reportFile<<" in rr_c_api testing executable.\n";
    	return -1;
    }

	XmlTestReporter reporter1(aFile);
	TestRunner runner1(reporter1);


	clog<<"Running Suite 1\n";
	runner1.RunTestsIf(Test::GetTestList(), "TEST_MODEL_1", 			True(), 0);
//	runner1.RunTestsIf(Test::GetTestList(), "CORE_EXCEPTIONS", 		True(), 0);

//    runner1.RunTestsIf(Test::GetTestList(), "Base", 		True(), 0);
//
//     clog<<"Running SteadyState\n";
//     runner1.RunTestsIf(Test::GetTestList(), "SteadyState", 	True(), 0);

    clog<<"Running TestSuite Tests\n";
    clog<<"ModelPath "<<gTSModelsPath;
    runner1.RunTestsIf(Test::GetTestList(), "SBML_l2v4", 	True(), 0);

    //Finish outputs result to xml file
    runner1.Finish();
    return 0;
}

void ProcessCommandLineArguments(int argc, char* argv[], Args& args)
{
    char c;
    while ((c = GetOptions(argc, argv, ("vi:d:m:l:r:s:t:"))) != -1)
    {
        switch (c)
        {
            case ('m'): args.SBMLModelsFilePath                     = rrOptArg;                       break;
            case ('r'): args.ResultOutputFile                       = rrOptArg;                       break;
			case ('t'): args.TempDataFolder        		            = rrOptArg;                       break;
			case ('v'): args.EnableLogging        		            = true;                       break;
            case ('?'): cout<<Usage(argv[0])<<endl;
            default:
            {
                string str = argv[rrOptInd-1];
                if(str != "-?")
                {
                    cout<<"*** Illegal option:\t"<<argv[rrOptInd-1]<<" ***\n\n";
                }
                exit(0);
            }
        }
    }

    //Check arguments, and choose to bail here if something is not right...
    if(argc < 2)
    {
        cout<<Usage(argv[0])<<endl;
       	rr::Pause();
        cout<<"\n";
        exit(0);
    }
}

#if defined(CG_IDE)
#pragma comment(lib, "rr_c_api.lib")
#pragma comment(lib, "roadrunner-static.lib")
#pragma comment(lib, "rr-libstruct-static.lib")
//#pragma comment(lib, "roadrunner.lib")

#pragma comment(lib, "unit_test-static.lib")
#endif


