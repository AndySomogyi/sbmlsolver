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
        LogOutput::mLogToConsole = true;

        if(argc < 2)
        {
            cout<<Usage(argv[0])<<endl;
            exit(0);
        }

     
        ProcessCommandLineArguments(argc, argv, args);

        gLog.SetCutOffLogLevel(args.CurrentLogLevel);
        string logFileName;

        RoadRunner *rr = NULL;

        if(args.UseOSTempFolder)
        {
            args.TempDataFolder = GetUsersTempDataFolder();
        }

        if(args.ModelFileName.size())
        {
            string logName = ExtractFileName(args.ModelFileName);
            logName = ChangeFileExtensionTo(logName, ".log");
            gLog.Init("", gLog.GetLogLevel(), unique_ptr<LogFile>(new LogFile(JoinPath(args.TempDataFolder, logName) )));
        }
        else
        {
            gLog.Init("", gLog.GetLogLevel(), unique_ptr<LogFile>(new LogFile(JoinPath(args.TempDataFolder, "RoadRunner.log") )));
        }

        Log(lInfo)<<"Logs are going to "<<gLog.GetLogFileName();
        Log(lInfo)<<"Log level is:" <<GetLogLevelAsString(gLog.GetLogLevel());
        SBMLModelSimulation simulation(args.DataOutputFolder, args.TempDataFolder);

        rr = new RoadRunner();
        rr->Reset();
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

        simulation.CompileIfDllExists(true);
        if(doContinue && !simulation.LoadSBMLFromFile())
        {
            Log(lError)<<"Failed loading SBML model";
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
					Log(lError)<<"Failed loading SBML model settings";
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

			rr->ComputeAndAssignConservationLaws(false);
		}

        //Then Simulate model
        if(doContinue && !simulation.Simulate())
        {
            Log(lError)<<"Failed running simulation";
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
				SimulationData result = simulation.GetResult();
				Log(lShowAlways)<<result;
			}
		}       

        delete rr;
    }
    catch(rr::Exception& ex)
    {
        Log(lError)<<"RoadRunner exception occurred: "<<ex.what()<<endl;
    }
	
	Log(lInfo)<<"RoadRunner is exiting...";
	if(args.Pause)
	{
		Pause();
	}
    return 0;
}

void ProcessCommandLineArguments(int argc, char* argv[], Args& args)
{
    char c;
    while ((c = GetOptions(argc, argv, ("cpufv:n:d:t:l:m:s:e:z:"))) != -1)
    {
        switch (c)
        {
            case ('v'): args.CurrentLogLevel                = GetLogLevel(optarg);     break;
            case ('c'): args.OnlyCompile                    = true;                         break;
            case ('p'): args.Pause                          = true;                         break;
            case ('t'): args.TempDataFolder                 = optarg;                       break;
            case ('d'): args.DataOutputFolder               = optarg;                       break;
            case ('m'): args.ModelFileName                  = optarg;                       break;
            case ('u'): args.UseOSTempFolder                = true;                         break;
            case ('l'): args.SelectionList                  = optarg;                       break;
            case ('s'): args.StartTime                      = ToDouble(optarg);             break;
            case ('e'): args.EndTime                        = ToDouble(optarg);             break;
            case ('z'): args.Steps                          = ToInt(optarg);                break;
            case ('f'): args.SaveResultToFile               = true;                         break;
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

    //Check arguments, and choose to bail here if something is not right...

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

