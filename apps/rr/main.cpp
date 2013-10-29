#ifdef USE_PCH
#include "rr_pch.h"
#endif
#pragma hdrstop
#if defined(WIN32)
#include <windows.h>
#endif
#include <iostream>
#include <fstream>
#include <string>

#if defined(__CODEGEARC__)
#include <dir.h>
#endif

#if defined(_MSVER)
#include <direct.h>
#endif

#include <iomanip>
#include "rrLogger.h"
#include "rrRoadRunner.h"
#include "rrException.h"
#include "rrUtils.h"
#include "rrSBMLModelSimulation.h"
#include "rrGetOptions.h"
#include "Args.h"
//---------------------------------------------------------------------------
using namespace std;
using namespace rr;

void ProcessCommandLineArguments(int argc, char* argv[], Args& args);
int main(int argc, char * argv[])
{
    string settingsFile;
    bool doContinue = true;
    Args args;
    try
    {
        Logger::enableConsoleLogging();

        if(argc < 2)
        {
            cout<<Usage(argv[0])<<endl;
            exit(0);
        }

        ProcessCommandLineArguments(argc, argv, args);

        gLog.setLevel(args.CurrentLogLevel);

        string thisExeFolder = getCurrentExeFolder();
        Log(lDebug)<<"RoadRunner bin location is: "<<thisExeFolder;

        //Assume(!) rr.exe is in bin folder of roadrunner install
        string RRInstallFolder = getParentFolder(thisExeFolder);    //Go up one folder
        Log(lDebug)<<"Assuming RoadRunner is installed in: "<<RRInstallFolder;

        if(args.UseOSTempFolder)
        {
            args.TempDataFolder = getUsersTempDataFolder();
        }

        if(args.TempDataFolder == ".")
        {
            args.TempDataFolder = getCWD();
        }

        Log(lDebug)<<"Temp data folder: "<<args.TempDataFolder<<endl;

        if(args.ModelFileName.size())
        {
            string logName = getFileName(args.ModelFileName);
            logName = changeFileExtensionTo(logName, ".log");
            //gLog.init("", gLog.getLevel());
        }
        else
        {
            Logger::enableConsoleLogging(Logger::getLevel());
        }

        Log(lInfo) << "Logs are going to " << Logger::getFileName();
        Log(lInfo) << "Log level is:" << Logger::getCurrentLevelAsString();
        SBMLModelSimulation simulation(args.DataOutputFolder, args.TempDataFolder);

        Log(lDebug) << "Working Directory: "<<getCWD()<<endl;


        //Creating roadrunner
        Log(lDebug)<<"Creating RoadRunner..."<<endl;
        RoadRunner *rr  = new RoadRunner("", args.TempDataFolder);
        rr->reset();
        Log(lDebug)<<"....."<<endl;
        simulation.UseEngine(rr);

        //The following will load and compile and simulate the sbml model in the file
        if(!args.ModelFileName.size())
        {
            Log(lInfo)<<"Please supply a sbml model file name, using option -m<modelfilename>";
            doContinue = false;
        }

        if(doContinue && !simulation.SetModelFileName(args.ModelFileName))
        {
            Log(lInfo)<<"Bad model file";
            doContinue = false;
        }

        simulation.ReCompileIfDllExists(true);
        if(doContinue && !simulation.LoadSBMLFromFile())
        {
            Log(Logger::LOG_ERROR)<<"Failed loading SBML model";
            doContinue = false;
        }

        if(doContinue)
        {
            Log(lInfo)<<"SBML semantics was loaded from file: "<<simulation.GetModelsFullFilePath();
        }

        //Then read settings file if it exists..
        if(doContinue)
        {
            if(settingsFile.size())
            {
                if(!simulation.LoadSettings(settingsFile))    //set selection list here!
                {
                    Log(Logger::LOG_ERROR)<<"Failed loading SBML model settings";
                    doContinue = false;
                }
            }
            else //Read from command line
            {
                simulation.SetTimeStart(args.StartTime);
                simulation.SetTimeEnd(args.EndTime);
                simulation.SetNumberOfPoints(args.Steps);
                simulation.SetSelectionList(args.SelectionList);
            }

            rr->setConservationAnalysis(false);
        }

        //Then Simulate model
        if(doContinue && !simulation.Simulate())
        {
            Log(Logger::LOG_ERROR)<<"Failed running simulation";
            throw("Failed running simulation");
        }

        if(doContinue)
        {
            if(args.SaveResultToFile)
            {
                //Write result
                if(!simulation.SaveResult())
                {
                    //Failed to save data
                }
            }
            else
            {
                //Write to std out
                RoadRunnerData result = simulation.GetResult();
                Log(Logger::LOG_FATAL)<<result;
            }
        }

        delete rr;
    }
    catch(rr::Exception& ex)
    {
        Log(Logger::LOG_ERROR)<<"RoadRunner exception occurred: "<<ex.what()<<endl;
    }

    Log(lInfo)<<"RoadRunner is exiting...";
    if(args.Pause)
    {
        rr::pause();
    }
    return 0;
}

void ProcessCommandLineArguments(int argc, char* argv[], Args& args)
{
    char c;

    while ((c = GetOptions(argc, argv, (const char*) ("cpufv:n:d:t:l:m:s:e:z:"))) != -1)
    {
        switch (c)
        {
            case ('v'): args.CurrentLogLevel                = Logger::stringToLevel(rrOptArg);     break;
            case ('c'): args.OnlyCompile                    = true;                                break;
            case ('p'): args.Pause                          = true;                                break;
            case ('t'): args.TempDataFolder                 = rrOptArg;                            break;
            case ('d'): args.DataOutputFolder               = rrOptArg;                            break;
            case ('m'): args.ModelFileName                  = rrOptArg;                            break;
            case ('u'): args.UseOSTempFolder                = true;                                break;
            case ('l'): args.SelectionList                  = rrOptArg;                            break;
            case ('s'): args.StartTime                      = toDouble(rrOptArg);                  break;
            case ('e'): args.EndTime                        = toDouble(rrOptArg);                  break;
            case ('z'): args.Steps                          = toInt(rrOptArg);                     break;
            case ('f'): args.SaveResultToFile               = true;                                break;
            case ('?'):
            {
                    cout<<Usage(argv[0])<<endl;
                    break;
            }
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
}

#if defined(CG_IDE)
#pragma comment(lib, "roadrunner.lib")
//#pragma comment(lib, "roadrunner-static.lib")
#pragma comment(lib, "sundials_cvode.lib")
#pragma comment(lib, "sundials_nvecserial.lib")
#pragma comment(lib, "nleq-static.lib")
#pragma comment(lib, "rr-libstruct-static.lib")
#pragma comment(lib, "libsbml-static.lib")
#pragma comment(lib, "libxml2_xe.lib")
#pragma comment(lib, "blas.lib")
#pragma comment(lib, "lapack.lib")
#pragma comment(lib, "libf2c.lib")
#pragma comment(lib, "poco_foundation-static.lib")
#endif

