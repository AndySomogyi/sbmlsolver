#ifdef USE_PCH
#include "rr_pch.h"
#endif
#pragma hdrstop
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <tchar.h>

#if defined(__CODEGEARC__)
#include <dir.h>
#else
#include <direct.h>
#endif

#include <iomanip>
#include "rrLogger.h"
#include "rrRoadRunner.h"
#include "rrCGenerator.h"
#include "rrException.h"
#include "rrUtils.h"
#include "rrStringUtils.h"
#include "rrTestSuiteModelSimulation.h"
#include "rrGetOptions.h"
#include "Args.h"
#include "rrStopWatch.h"
//---------------------------------------------------------------------------
using namespace std;
using namespace rr;
string Usage(const string& prg);

int main(int argc, char * argv[])
{

    LogOutput::mLogToConsole = true;

    if(argc < 2)
    {
        cout<<Usage(argv[0])<<endl;
        exit(0);
    }

    Args paras;
    char c;
    while ((c = GetOptions(argc, argv, ("cpv:n:d:t:m:"))) != -1)
    {
        switch (c)
        {
            case ('n'): paras.CaseNumber                    = ToInt(optarg);                break;
            case ('v'): paras.CurrentLogLevel               = GetLogLevel(optarg);          break;
            case ('p'): paras.Pause                         = true;                         break;
            case ('t'): paras.TempDataFolder                = optarg;                       break;
            case ('d'): paras.DataOutputFolder              = optarg;                       break;
            case ('m'): paras.TestSuiteModelsPath           = optarg;                       break;
            case ('?'):
            {
                    cout<<Usage(argv[0])<<endl;
            }
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

    string excludedCases("rrExcludedTestCases.txt");

    vector<string> exclude;
    exclude = GetLinesInFile(excludedCases);

    vector<int> cant_simulate; //Theese are cases rr don't support (yet)

    for(int i = 0; i< exclude.size(); i++)
    {
        string nr = exclude[i];
        int j = ToInt(nr);
        if(j > 0)
        {
            cant_simulate.push_back(j);
        }
        else
        {
            Log(lError)<<"Bad line in file "<<excludedCases;
        }
    }
    //The followoing are cases currently having problems. Some do in fact pass the test, but takes very long to run
    vector<int> exceptions;
//    exceptions.push_back(958);  //?
//    exceptions.push_back(961);    //Weird assignments...
//    exceptions.push_back(966); //This one takes really long tim..
    RoadRunner *rrI = NULL;     //The roadrunner instance
    StopWatch sWatch;
    try
    {
        if(std::find(exceptions.begin(), exceptions.end(), paras.CaseNumber) != exceptions.end())
        {
            //Don't simulate this one..
            throw(rr::Exception("KNOWN_PROBLEM"));
        }

         if(std::find(cant_simulate.begin(), cant_simulate.end(), paras.CaseNumber) != cant_simulate.end())
        {
            //Don't simulate this one..
            throw(rr::Exception("NOT_SUPPORTED"));
        }

        gLog.SetCutOffLogLevel(paras.CurrentLogLevel);
        string dataOutputFolder(paras.DataOutputFolder);
        string dummy;
        string logFileName;

        CreateTestSuiteFileNameParts(paras.CaseNumber, ".log", dummy, logFileName);

        //Create subfolder for data output
        dataOutputFolder = JoinPath(dataOutputFolder, GetTestSuiteSubFolderName(paras.CaseNumber));

        if(!CreateFolder(dataOutputFolder))
        {
            throw(rr::Exception("Failed creating output folder for data output"));
        }

        gLog.Init("", gLog.GetLogLevel(), unique_ptr<LogFile>(new LogFile(JoinPath(dataOutputFolder, logFileName))));
        Log(lDebug)<<"Logs are going to "<<gLog.GetLogFileName();

        Log(lDebug)<<"Current Log level is:" <<gLog.GetCurrentLogLevel();
        TestSuiteModelSimulation simulation(dataOutputFolder);


        sWatch.Start();
        //dataOutputFolder += dummy;
        rrI = new RoadRunner();
        rrI->Reset();
        rrI->setCompiler("tcc");
        simulation.UseEngine(rrI);

        //Read SBML models.....
        string modelFilePath(paras.TestSuiteModelsPath);
        string modelFileName;

        simulation.SetCaseNumber(paras.CaseNumber);
        CreateTestSuiteFileNameParts(paras.CaseNumber, "-sbml-l2v4.xml", modelFilePath, modelFileName);

        //The following will load and compile and simulate the sbml model in the file
        simulation.SetModelFilePath(modelFilePath);
        simulation.SetModelFileName(modelFileName);
        simulation.CompileIfDllExists(true);
        simulation.CopyFilesToOutputFolder();

        if(!simulation.LoadSBMLFromFile())
        {
            Log(lError)<<"Failed loading SBML model";
            goto end;
        }
        //Then read settings file if it exists..
        string settingsOveride("");//C:\\rrw\\Models\\settings_override.txt");
        if(!simulation.LoadSettings(settingsOveride))    //set selection list here!
        {
            Log(lError)<<"Failed loading SBML model settings";
        }

        rrI->ComputeAndAssignConservationLaws(false);

        //Then Simulate model
         if(!simulation.Simulate())
        {
            Log(lError)<<"Failed running simulation";
            throw("Failed running simulation");
        }

        //Write result
         if(!simulation.SaveResult())
        {
            //Failed to save data
        }

        if(!simulation.LoadReferenceData())
        {
            Log(lError)<<"Failed loading reference data";
        }

        simulation.CreateErrorData();

        //Check error data.. if an error in the set is larger than threshold, signal an error
        if(!simulation.Pass())
        {
            Log(lError)<<"FAIL "<<"FAILING_POINTS "<<simulation.NrOfFailingPoints()<<" LARGEST_ERROR "<<simulation.LargestError()<<" SUPPORTED TRUE"<<" EXCEPTION_MSG NONE"<<" Time "<<sWatch.GetTime();;
        }
        else
        {
            Log(lError)<<"PASS "<<"FAILING_POINTS "<<simulation.NrOfFailingPoints()<<" LARGEST_ERROR "<<simulation.LargestError()<<" SUPPORTED TRUE"<<" EXCEPTION_MSG NONE"<<" Time "<<sWatch.GetTime();;
        }

        simulation.SaveAllData();
        simulation.SaveModelAsXML(dataOutputFolder);
        sWatch.Stop();
     }
    catch(rr::Exception& ex)
    {
        string error = ex.what();

        if(error.find("NOT_SUPPORTED") != string::npos)
        {
            Log(lError)<<"FAIL "<<"FAILING_POINTS "<<0<<" LARGEST_ERROR "<<1<<" SUPPORTED FALSE"<<" EXCEPTION_MSG "<<ex.what()<<" Time "<<sWatch.GetTime();
        }

        if(error.find("KNOWN_PROBLEM") != string::npos)
        {
            Log(lError)<<"FAIL "<<"FAILING_POINTS "<<0<<" LARGEST_ERROR "<<1<<" SUPPORTED TRUE"<<" EXCEPTION_MSG "<<ex.what()<<" Time "<<sWatch.GetTime();
        }

    }

    end:
    delete rrI;

    Log(lInfo)<<"Done";
    if(paras.Pause)
    {
        Pause();
    }

    return 0;
}

string Usage(const string& prg)
{
    stringstream usage;
    usage << "\nUSAGE for "<<prg<<": (options and parameters)\n\n";
    usage<<left;
    usage<<setfill('.');
    usage<<setw(25)<<"-n\"number\" "    <<" TestSuite number\n";
    usage<<setw(25)<<"-m\"path\" "    	<<" Path to TestSuite models\n";
    usage<<setw(25)<<"-v"                <<" Verbose mode. Ouputs information during program execution\n";
    usage<<setw(25)<<"-? "               <<" Shows the help screen.\n\n";

    usage<<"\nSystems Biology, UW 2012\n";
    return usage.str();
}

#if defined(CG_IDE)
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
#endif

