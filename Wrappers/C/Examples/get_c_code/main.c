#pragma hdrstop
#include <stdio.h>
#include "../../rr_c_api.h"

/*--------------------------------------------------------------------------
Example showing how to obtain model generated C code

---------------------------------------------------------------------------*/

int main()
{
    RRHandle rrHandle;
	char* text;

    char modelFileName[2048];

	//-------------------------------
    rrHandle =  createRRInstance();
    if(!rrHandle)
    {
        printf("No handle...");
    }

	text = getBuildDate();

	if(text)
	{
		printf("Build date: %s", text);
		freeText(text);
	}

    //Setup tempfolder
    strcpy(text, "../temp");
    if(!setTempFolder(rrHandle, text))
    {
    	printf("The temp file folder \'%s\' do not exist. Exiting...\n", text);
        exit(0);
    }

	//Setup logging
   	setLogLevel("Info");
	enableLoggingToConsole();
   	enableLoggingToFile(rrHandle);


	strcpy(modelFileName, "../models/test_1.xml");

	text = getFileContent(modelFileName);

//    cout << "C code: " << sbml << endl;
//
//    //To get the C Code, the code needs to be generated
//    if(!loadSBML(sbml.c_str()))
//    {
//    	cerr<<"Failed loading SBML.\n";
//        cerr<<"Last error: "<<getLastError()<<endl;
//        return -1;
//    }
//
//	RRCCode* code = getCCode();
//    if(!code)
//    {
//	  	cerr<<"Failed to get CCode from RoadRunner";
//        return -1;
//    }
//
//    cout<<"START OF CODE ==========\n";
//	if(code->Header)
//	{
//		cout<<"C Header =========== \n"<<code->Header<<"\n";
//	}
//	else
//	{
//		cout<<"C Header =========== \n"<<" is NULL"<<"\n";
//	}
//
//	if(code->Source)
//	{
//		cout<<"C Source =========== \n"<<code->Source<<"\n";
//	}
//	else
//	{
//		cout<<"C Source =========== \n"<<" is NULL"<<"\n";
//	}
//
//    cout<<"END OF CODE ==========\n";
//
//	///// Cleanup
//    freeCCode(code);
//    text = getCopyright();
//    if(hasError())
//    {
//        char* error = getLastError();
//        cout<<error<<endl;
//    }
//    cout<<text<<endl;
    freeText(text);
    freeRRInstance(rrHandle);
    return 0;
}

#pragma link "rr_c_api.lib"

