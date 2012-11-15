//#pragma hdrstop
#if defined(_WIN32) || defined(WIN32)
#include <windows.h>
#endif
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

#if defined(__CODEGEARC__)
#include <dir.h>
#elif defined(_MS_VER)
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

    cout<<"======== RoadRunner C API Client ==================\n\n";
    RRHandle aHandle  = getRRInstance();

	if(!aHandle)
    {
        cerr<<"Failed getting a handle to RoadRunner";
    	doMore = false;
    }

    if(!setLogLevel(GetLogLevelAsString(args.CurrentLogLevel).c_str()) )
    {
        cerr<<"Failed setting log RoadRunner Log level";
    	doMore = false;
    }
	
	if(args.TempDataFolder.size() < 2)
	{

		char* buffer = new char[1024];
        // Get the current working directory:
        if( (buffer = getcwd( buffer, MAXPATH )) == NULL )
        {
            perror( "getcwd error" );
        }
        else
        {
			args.TempDataFolder	= buffer;
        }
        delete [] buffer;
	}
  	
	setTempFolder(args.TempDataFolder.c_str());

    if(!enableLogging())
    {
        cerr<<"Failed setting log RoadRunner Log level";
    	doMore = false;
    }

    cout<<"Currrent Log Level: "<<getLogLevel()<<endl;
    setTempFolder(args.TempDataFolder.c_str());
	char* text = getBuildDate();
	if(text)
	{
		cout<<"Build date: "<<text<<endl;
		freeText(text);
	}

    if(args.ModelFileName.size() > 1 &&  !FileExists(args.ModelFileName))
    {
        cerr<<"The xml model file:"<<args.ModelFileName<<" don't exist. Please supply a sbml model file name, using option -m<modelfilename>";
        doMore = false;
    }
	else if(args.ModelFileName.size() == 0)
	{
        cerr<<"Please supply a sbml model file name, using option -m<modelfilename>";
        doMore = false;
	}

    //RoadRunner Flags and options
    if(doMore)
    {
	    setComputeAndAssignConservationLaws(args.ComputeAndAssignConservationLaws);
    }

    if(doMore)
    {
//    	if(!loadSBML(GetFileContent(args.ModelFileName).c_str()))
    	if(!loadSBMLFromFile(args.ModelFileName.c_str()))
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

    cout<<"\n"<<text<<endl;

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
		cout<<"Character is: "<<c<<" and optarg is:"<<optArg<<endl;	
        switch (c)
        {
            case ('v'): args.CurrentLogLevel                        = GetLogLevel(optArg);   
						cout<<"Loglevel is set to :"<<args.CurrentLogLevel<<endl; break;
            case ('p'): args.Pause                                  = true;                         break;
            case ('t'): args.TempDataFolder                         = optArg;                       break;
            case ('d'): args.DataOutputFolder                       = optArg;                       break;
			case ('f'): args.SaveResultToFile                       = true;                         break;
            case ('m'): args.ModelFileName                          = optArg;                       break;
            case ('l'): args.SelectionList                          = optArg;                       break;
            case ('s'): args.StartTime                              = ToDouble(optArg);             break;
            case ('e'): args.EndTime                                = ToDouble(optArg);             break;
            case ('z'): args.Steps                                  = ToInt(optArg);                break;
            case ('x'): args.CalculateSteadyState                   = true;                			break;
            case ('y'): args.ComputeAndAssignConservationLaws  		= false;                  		break;
            case ('?'):
            {
                    cout<<Usage(argv[0])<<endl;
            }
            default:
            {
                string str = argv[optInd-1];
                if(str != "-?")
                {
                    cout<<"*** Illegal option:\t"<<argv[optInd-1]<<" ***\n"<<endl;
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

