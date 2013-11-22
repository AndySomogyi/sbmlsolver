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
    cout << "RoadRunner SBML Test Suite" << endl;

    cout << "built on " << __DATE__ << ", " << __TIME__ << endl;

    cout << RoadRunner::getExtendedVersionInfo() << endl;

    Logger::enableConsoleLogging();

    Args args;
    ProcessCommandLineArguments(argc, argv, args);
    setup(args);

    string reportFile(args.ResultOutputFile);
     fstream aFile(reportFile.c_str(), ios::out);
    if(!aFile)
    {
        cerr<<"Failed opening report file: "<<reportFile<<" in rrc_api testing executable.\n";
        return false;
    }

    XmlTestReporter reporter1(aFile);
    TestRunner runner1(reporter1);

    if(args.Suites.find('A') != std::string::npos)
    {
        clog<<"Running Suite CORE_TESTS\n";
        runner1.RunTestsIf(Test::GetTestList(), "CORE_TESTS",             True(), 0);
    }

    if(args.Suites.find('B') != std::string::npos)
    {
        clog<<"Running Suite TEST_MODEL_1\n";
        runner1.RunTestsIf(Test::GetTestList(), "TEST_MODEL_1",          True(), 0);
    }

    if(args.Suites.find('C') != std::string::npos)
    {
        clog<<"Running Suite NOM TESTS\n";
        runner1.RunTestsIf(Test::GetTestList(), "NOM_TESTS",             True(), 0);
    }

    if(args.Suites.find('D') != std::string::npos)
    {
        clog<<"Running Suite LibStruct TESTS\n";
        runner1.RunTestsIf(Test::GetTestList(), "LIBSTRUCT_TESTS",          True(), 0);
    }

    if (args.Suites.find('E') != std::string::npos)
    {

        clog << "Running Suite SBML_l2v4\n";
        clog << "ModelPath " << gTSModelsPath;
        runner1.RunTestsIf(Test::GetTestList(), "SBML_TEST_SUITE", True(), 0);
    }
    if (args.Suites.find('F') != std::string::npos)
    {

        clog << "Running Suite Valgrind SBML_TEST_SUITE_VG1\n";
        clog << "ModelPath " << gTSModelsPath;
        runner1.RunTestsIf(Test::GetTestList(), "SBML_TEST_SUITE_VG1", True(),
                0);
    }

    if (args.Suites.find('G') != std::string::npos)
    {

        clog << "Running Suite Valgrind SBML_TEST_SUITE_VG2\n";
        clog << "ModelPath " << gTSModelsPath;
        runner1.RunTestsIf(Test::GetTestList(), "SBML_TEST_SUITE_VG2", True(),
                0);
    }

    if (args.Suites.find('H') != std::string::npos)
    {

        clog << "Running Suite Valgrind SBML_TEST_SUITE_RATE_RULES\n";
        clog << "ModelPath " << gTSModelsPath;
        runner1.RunTestsIf(Test::GetTestList(), "SBML_TEST_SUITE_RATE_RULES", True(),
                0);
    }

    if (args.Suites.find('I') != std::string::npos)
    {

        clog << "Running Suite Valgrind SBML_TEST_SUITE_EVENTS\n";
        clog << "ModelPath " << gTSModelsPath;
        runner1.RunTestsIf(Test::GetTestList(), "SBML_TEST_SUITE_EVENTS", True(),
                0);
    }

    if (args.Suites.find('J') != std::string::npos)
    {
        clog << "Running Suite SBML_TEST_SUITE_C_FAILS\n";
        clog << "ModelPath " << gTSModelsPath;
        runner1.RunTestsIf(Test::GetTestList(), "SBML_TEST_SUITE_C_FAIL", True(),
                0);
    }

    if (args.Suites.find('K') != std::string::npos)
    {
        clog << "Running Suite SBML_TEST_SUITE_LONGTIME\n";
        clog << "ModelPath " << gTSModelsPath;
        runner1.RunTestsIf(Test::GetTestList(), "SBML_TEST_SUITE_LONGTIME", True(),
                           0);
    }

    if (args.Suites.find('L') != std::string::npos)
    {
        clog << "Running Suite PLUGINMANAGER_TESTS\n";
        clog << "ModelPath " << gTSModelsPath;
        runner1.RunTestsIf(Test::GetTestList(), "PLUGINMANAGER_TESTS", True(),
                           0);
    }

    //Finish outputs result to xml file
    runner1.Finish();
    return 0;
}

bool setup(Args& args)
{
    string thisExeFolder = getCurrentExeFolder();
    clog<<"RoadRunner bin location is: "<<thisExeFolder<<endl;

    //Assume(!) this is the bin folder of roadrunner install
    gRRInstallFolder     = getParentFolder(thisExeFolder);
    gDebug               = args.EnableLogging;
    gTSModelsPath        = args.ModelsFilePath;
    gTempFolder          = args.TempDataFolder;
    gTestDataFolder      = joinPath(gRRInstallFolder, "testing");

    gCompiler = args.compiler;
    Log(Logger::LOG_NOTICE) << "Using compiler " << gCompiler;


    if(args.Suites.size() == 0)
    {
        //Run all
        args.Suites = "ABCDE";
    }

    setInstallFolder(gRRInstallFolder.c_str());

    if(gDebug)
    {
        enableLoggingToConsole();
        Logger::setLevel(Logger::LOG_DEBUG);
    }
    else
    {
        Logger::setLevel(Logger::LOG_NOTICE);
    }

    // set test suite model path (read from cmd line)
    gTSModelsPath = joinPath(joinPath(gTSModelsPath, "cases"), "semantic");
    return true;
}

void ProcessCommandLineArguments(int argc, char* argv[], Args& args)
{
    char c;
    while ((c = GetOptions(argc, argv, ("m:r:t:vs:c:"))) != -1)
    {
        switch (c)
        {
            case ('m'): args.ModelsFilePath     = rrOptArg; break;
            case ('r'): args.ResultOutputFile   = rrOptArg; break;
            case ('t'): args.TempDataFolder     = rrOptArg; break;
            case ('v'): args.EnableLogging      = true;     break;
            case ('s'): args.Suites             = rrOptArg; break;
            case ('c'): args.compiler           = rrOptArg; break;
            case ('?'): cout << Usage(argv[0]) << endl;     break;
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
           rr::pause();
        cout<<"\n";
        exit(0);
    }
}

#if defined(CG_IDE)

#if defined(STATIC_RRC)
#pragma comment(lib, "rrc_api-static.lib")
#else
#pragma comment(lib, "rrc_api.lib")
#endif

#if defined(STATIC_RR)
    #pragma comment(lib, "roadrunner-static.lib")
#else
    #pragma comment(lib, "roadrunner.lib")
#endif

#pragma comment(lib, "sundials_cvode")
#pragma comment(lib, "sundials_nvecserial")
#pragma comment(lib, "libf2c")
#pragma comment(lib, "blas")
#pragma comment(lib, "lapack")
#pragma comment(lib, "nleq-static")
#pragma comment(lib, "poco_foundation-static.lib")
#pragma comment(lib, "poco_xml-static.lib")
#pragma comment(lib, "rr-libstruct-static.lib")
#pragma comment(lib, "unit_test-static.lib")
#endif


