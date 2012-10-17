//#pragma hdrstop
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <tchar.h>
#include <iomanip>
#if defined(__CODEGEARC__)
#include <dir.h>
#else
#include <direct.h>
#endif

#include "rrUtils.h"
#include "rrStringUtils.h"
#include "GetOptions.h"
#include "Args.h"
#include "rr_c_api.h"

//---------------------------------------------------------------------------
using namespace std;
using namespace rr;

void ProcessCommandLineArguments(int argc, char* argv[], Args& args);

int main(int argc, char* argv[])
{
    Args args;
    ProcessCommandLineArguments(argc, argv, args);

    string settingsFile;
    stringstream ss;

	bool doMore = true;	//set to false to move to end


    cout<<"======== RoadRunner C API Client ==================\n";
    RRHandle aHandle  = getRRInstance();

	if(!aHandle)
    {
        cerr<<"Failed getting a handle to RoadRunner";
    	doMore = false;
    }

    setTempFolder(args.TempDataFolder.c_str());

    if(!setLogLevel(GetLogLevelAsString(args.CurrentLogLevel).c_str()) )
    {
        cerr<<"Failed setting log RoadRunner Log level";
    	doMore = false;
    }

    cout<<"Currrent Log Level: "<<getLogLevel();

    if(!enableLogging())
    {
        cerr<<"Failed setting log RoadRunner Log level";
    	doMore = false;
    }

	char* text = getBuildDate();
	if(text)
	{
		cout<<"Build date: "<<text<<endl;
		freeText(text);
	}

    if(!FileExists(args.ModelFileName))
    {
        cerr<<"The file:"<<args.ModelFileName<<" don't exist. Please supply a sbml model file name, using option -m<modelfilename>";
        doMore = false;
    }

    //RoadRunner Flags and options
    if(doMore)
    {
	    setComputeAndAssignConservationLaws(args.ComputeAndAssignConservationLaws);
    }

    if(doMore)
    {
    	if(!loadSBML(GetFileContent(args.ModelFileName).c_str()))
	    {
    	    char* error = getLastError();
        	cerr<<"\n"<<error<<endl;
	        doMore = false;;
    	}
    }

    if(doMore && args.CalculateSteadyState)
    {
       	cout<<"Calculating steady state: "<<endl;
     	double ss;
        bool success = steadyState(ss);
        if(!success)
        {
            cerr<<"steadyState API function failed\n";
            cerr<<"API error was: "<<getLastError()<<endl;
            doMore = false;
        }
        else
        {
            //Get value for each specie?
            RRStringArray* list = getTimeCourseSelectionList();
            if(list == NULL)
            {
		        cerr<<"SelectionList is empty. Exiting\n";
            }
            for(int i = 1; i < list->Count; i++)   	//at index 0 is 'time'
            {
            	double value;
                bool isSuccess = getValue(list->String[i], value);
                if(!isSuccess)
                {
                    cerr<<"getValue API function failed\n";
                    cerr<<"API error was: "<<getLastError()<<endl;
                    return -1;
                }
                cout<<list->String[i]<<" steady state at "<<value<<endl;
            }
        }
    }

	RRResultHandle result;
	if(doMore)
    {
	    setTimeStart(args.StartTime);
    	setTimeEnd(args.EndTime);
	    setNumPoints(args.Steps);
    	setTimeCourseSelectionList(args.SelectionList.c_str());
		cout<<"Roadrunner is about to simulate model\n";
        RRStringArrayHandle list =  getTimeCourseSelectionList();

        if(list)
        {
	        cout<<"\nThe following is selected: "<<stringArrayToString(list)<<endl;
        }
        else
        {
			cout<<"SelectionList list is empty. Default list will be selected during simulation\n";
        }

        result = simulate();
    }

	if(doMore && result)
	{
		if(!args.SaveResultToFile)	
		{
			cout<<resultToString(result);	
		}
		else
		{
			
			string outPutFName = JoinPath(args.DataOutputFolder, ExtractFileName(args.ModelFileName));
			outPutFName = ChangeFileExtensionTo(outPutFName, ".csv");
			ofstream fOut(outPutFName.c_str());
			if(!fOut)
			{
				cerr<<"Failed opening file: "<<outPutFName<<" for writing.";
			}
			else
			{
				cout<<"Saving data to file: "<<outPutFName<<"\n";
				fOut<<resultToString(result);
			}
		}				
	}


    text = getCopyright();
    if(hasError())
    {
        char* error = getLastError();
        cout<<"\nThe latest error in RoadRunner was: \n"<<error<<endl;
    }

    cout<<text<<endl;

    freeRRInstance(aHandle);
    cout<<"RoadRunner is exiting...\n";

    if(args.Pause)
    {
        rr::Pause();
    }
    return 0;
}

void ProcessCommandLineArguments(int argc, char* argv[], Args& args)
{
    char c;
    while ((c = GetOptions(argc, argv, ("cfpxyv:n:d:t:l:m:s:e:z:"))) != -1)
    {
        switch (c)
        {
            case ('v'): args.CurrentLogLevel                        = GetLogLevel(optarg);    	 	break;
            case ('p'): args.Pause                                  = true;                         break;
            case ('t'): args.TempDataFolder                         = optarg;                       break;
            case ('d'): args.DataOutputFolder                       = optarg;                       break;
			case ('f'): args.SaveResultToFile                       = true;                         break;
            case ('m'): args.ModelFileName                          = optarg;                       break;
            case ('l'): args.SelectionList                          = optarg;                       break;
            case ('s'): args.StartTime                              = ToDouble(optarg);             break;
            case ('e'): args.EndTime                                = ToDouble(optarg);             break;
            case ('z'): args.Steps                                  = ToInt(optarg);                break;
            case ('x'): args.CalculateSteadyState                   = true;                			break;
            case ('y'): args.ComputeAndAssignConservationLaws  		= false;                  		break;
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
    if(argc < 2)
    {
        cout<<Usage(argv[0])<<endl;
        exit(0);
    }
}

#if defined(CG_IDE)
#pragma comment(lib, "roadrunner-static.lib")
#pragma comment(lib, "rr_c_api.lib")
#endif

