#ifdef USE_PCH
#include "rr_pch.h"
#endif
#pragma hdrstop
#include <iomanip>
#include <sstream>
#include "Args.h"
//---------------------------------------------------------------------------

using namespace std;

Args::Args()
:
UseOSTempFolder(false),
OnlyCompile(false),
Pause(false),
CurrentLogLevel(Logger::LOG_INFORMATION),
ModelFileName(""),
DataOutputFolder(""),
TempDataFolder("."),
StartTime(0),
Duration(5),
EndTime(StartTime + Duration),
Steps(50),
//SaveResultToFile(""),
SelectionList("")
{}

string Usage(const string& prg)
{
    stringstream usage;
    usage << "\nUSAGE for "<<prg<<"\n\n";
    usage<<left;
    usage<<setfill('.');
    usage<<setw(25)<<"-v<debug level>"              <<" Debug levels: Error, Warning, Info, Debug. Default: Info\n";
    usage<<setw(25)<<"-m<FileName>"                 <<" SBML Model File Name (with path)\n";
    usage<<setw(25)<<"-d<FilePath>"                 <<" Data output folder. If not given, data is output to current directory (implies -f is given)\n";
    usage<<setw(25)<<"-t<FilePath>"                 <<" Temporary data output folder. If not given, temp files are output to current directory\n";
    usage<<setw(25)<<"-p"                           <<" Pause before exiting.\n";
    usage<<setw(25)<<"-c"                           <<" Stop execution after compiling model\n";
    usage<<setw(25)<<"-u"                           <<" Use users OS designated temporary folder\n";
    usage<<setw(25)<<"-s<#>"                        <<" Set the start time for simulation. Default: 0\n";
    usage<<setw(25)<<"-e<#>"                        <<" Set the end time for simulation. Default: 5\n";
    usage<<setw(25)<<"-z<#>"                        <<" Set number of steps in the simulation. Default: 50\n";
    usage<<setw(25)<<"-l<List>"                     <<" Set selection list. Separate variables using ',' or space\n";
    usage<<setw(25)<<"-? "                          <<" Shows the help screen.\n\n";

    usage<<"\nSystems Biology, UW 2012\n";
    return usage.str();
}

