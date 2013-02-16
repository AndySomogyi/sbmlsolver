#pragma hdrstop
#pragma argsused
#include <stdio.h>
#include "rr_cm.h"

#pragma option -dw-
#pragma option -d
int main()
{
//    //Declarations..
	RRHandle rrHandle;
	char* text;
	char* modelFileName = "r://models//test_1.xml";
    char* sbml;
    RRCCodeHandle codeHandle;
//	//.....

	printf("Starting C program...");
    text = malloc(512);


    rrHandle =  getRRHandle();
//
//    if(!rrHandle)
//    {
//        printf("No handle...");
//    }
//    else
//    {
//	    printf("Handle allocated succesfully..");
//    }
//
//
//	text = getBuildDate();
//
//	if(text)
//	{
//		printf("\nBuild date: %s", text);
//		freeText(text);
//	}
//
//    setTempFolderH(rrHandle, "r:\\rrTemp");
//	enableLoggingH(rrHandle);
//    setLogLevel("Info");
//
//
//    //To get the C Code, the code needs to be generated
//    if(!loadSBMLFromFileH(rrHandle, modelFileName))
//    {
//    	printf("Failed loading SBML.\n");
//        printf("Last error: %s", getLastError());
//        return -1;
//    }
//
//	codeHandle = getCCode(rrHandle);
//    if(!codeHandle)
//    {
//	  	printf("Failed to get CCode from RoadRunner");
//        printf("Last error: %s", getLastError());
//        return -1;
//    }
//
//    printf("START OF CODE ==========\n");
//	if(codeHandle->Header)
//	{
//		printf("C Header =========== \n %s \n", codeHandle->Header);
//	}
//	else
//	{
//		printf("C Header is NULL \n");
//	}
//
//	if(codeHandle->Source)
//	{
//		printf("C Source =========== \n %s \n", codeHandle->Source);
//	}
//	else
//	{
//		printf("C Source is NULL\n");
//	}
//
//    printf("END OF CODE ==========\n");
//
//	///// Cleanup
//
//    freeCCode(codeHandle);
//    text = getCopyright(rrHandle);
//    if(text)
//    {
//    	printf(text);
//    }
//
//    freeText(text);
//    freeRRInstance(rrHandle);
//
//    if(hasError())
//    {
//        char* error = getLastError();
//        printf("Last error %s \n", error);
//    }
	return 0;
}

#pragma comment(lib, "rr_cm.lib")
