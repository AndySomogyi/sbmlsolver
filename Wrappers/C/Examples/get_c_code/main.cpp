#pragma hdrstop
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "rr_c_api.h"
#include "rrUtils.h"
#include "rrStringUtils.h"
#include "rrLogger.h"

/*--------------------------------------------------------------------------
Example showing how to obtain model generated C code

---------------------------------------------------------------------------*/
using namespace std;
using namespace rr;

int main()
{
    RRHandle rrHandle = NULL;
    rrHandle =  getRRInstance();

    if(!rrHandle)
    {
        cout<<"No handle...";
    }

	char* text;
	text = getBuildDate();

	if(text)
	{
		cout<<"Build date: "<<text<<endl;
		freeText(text);
	}
	
	gLog.SetCutOffLogLevel(lDebug5);
    gLog.Init("", gLog.GetLogLevel(), unique_ptr<LogFile>(new LogFile(JoinPath("/tmp", "RoadRunner.log") ))); // WARNING: hard-coded /tmp
    Log(lInfo)<<"Logs are going to "<<gLog.GetLogFileName();
    Log(lInfo)<<"Log level is:" <<GetLogLevelAsString(gLog.GetLogLevel());

	string fileName = "/home/sagrada/myhome/develop/rr/install/all/models/test_1.xml";
	string sbml = GetFileContent(fileName.c_str());
    
    cout << "C code: " << sbml << endl;

    //To get the C Code, the code needs to be generated
    if(!loadSBML(sbml.c_str()))
    {
    	cerr<<"Failed loading SBML.\n";
        cerr<<"Last error: "<<getLastError()<<endl;
        return -1;
    }

	RRCCode* code = getCCode();
    if(!code)
    {
	  	cerr<<"Failed to get CCode from RoadRunner";
        return -1;
    }

    cout<<"START OF CODE ==========\n";
	if(code->Header)
	{
		cout<<"C Header =========== \n"<<code->Header<<"\n";
	}
	else
	{
		cout<<"C Header =========== \n"<<" is NULL"<<"\n";
	}

	if(code->Source)
	{
		cout<<"C Source =========== \n"<<code->Source<<"\n";
	}
	else
	{
		cout<<"C Source =========== \n"<<" is NULL"<<"\n";
	}

    cout<<"END OF CODE ==========\n";

	///// Cleanup
    freeCCode(code);
    text = getCopyright();
    if(hasError())
    {
        char* error = getLastError();
        cout<<error<<endl;
    }
    cout<<text<<endl;
    freeText(text);
    freeRRInstance(rrHandle);
    return 0;
}

