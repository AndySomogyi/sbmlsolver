#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include "rrLogger.h"
#include "rrStringUtils.h"
#include "rrRoadRunner.h"
#include "rrException.h"
#include "rrGetOptions.h"
#include "Args.h"
//---------------------------------------------------------------------------
using namespace std;
using namespace rr;

void ProcessCommandLineArguments(int argc, char* argv[], Args& args);
int main(int argc, char * argv[])
{
    try
    {
        LogOutput::mLogToConsole = true;

        if(argc < 2)
        {
            cout<<Usage(argv[0])<<endl;
            exit(0);
        }

        Args args;
        ProcessCommandLineArguments(argc, argv, args);

        gLog.SetCutOffLogLevel(args.CurrentLogLevel);

        RoadRunner *rr = NULL;


        gLog.Init("", gLog.GetLogLevel(), unique_ptr<LogFile>(new LogFile(JoinPath(args.TempDataFolder, "RoadRunner.log") )));

        Log(lShowAlways)<<"Logs are going to "<<gLog.GetLogFileName();
        Log(lShowAlways)<<"Log level is:" <<gLog.GetCurrentLogLevel();

        rr = new RoadRunner();
        rr->Reset();

        //The following will load and compile and simulate the sbml model in the file
        if(!args.ModelFileName.size())
        {
            Log(lInfo)<<"Please supply a sbml model file name, using option -m<modelfilename>";
        }
         
        if(rr->loadSBMLFromFile(args.ModelFileName))
        {
            Log(lInfo)<<"The model was loaded into roadrunner succesfully..";
        }

        Log(lInfo)<<"RoadRunner is exiting...";

        delete rr;
    }
    catch(rr::Exception& ex)
    {
        Log(lError)<<"RoadRunner exception occured: "<<ex.what()<<endl;
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
            case ('t'): args.TempDataFolder                 = optarg;                       break;
            case ('m'): args.ModelFileName                  = optarg;                       break;
            case ('u'): args.UseOSTempFolder                = true;                         break;
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
}
