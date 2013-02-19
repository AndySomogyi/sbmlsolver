#pragma hdrstop
#include <stdio.h>
#include "rr_cm.h"

int main()
{
    //Declarations..
	RRHandle *rrHandles;
    TPHandle *tpHandle;		//ThreadPool handle.. use to check when a pool of threads has finished..

	char* text;
	char* modelFileName = "r://models//test_1.xml";
    char* sbml;
    int   handleCount;
    int i;

	//.....

    handleCount = 100;
	printf("Starting C program...\n");

    rrHandles =  getRRHandles(handleCount);

    if(!rrHandles)
    {
        printf("No handles...");
    }
    else
    {
	    printf("Handles allocated succesfully..");
    }

   	setLogLevel("Info");
	enableLoggingToConsole();
    for(i = 0; i < handleCount; i++)
    {
    	setTempFolder(rrHandles[i], "r:\\rrTemp");
		enableLoggingToFile(rrHandles[i]);
    }

	//loadSBML models in threads instead
    tpHandle = loadModelFromFileTP(rrHandles, modelFileName);


    for(i = 0; i < handleCount; i++)
    {
        if(!loadSBMLFromFile(rrHandles[i], modelFileName))
        {
        	printf("Failed loading SBML.\n");
            printf("Last error: %s", getLastError());
            return -1;
        }
    }


//	// Cleanup
//    freeRRInstance(rrHandle);
//
//
//	//Finish off..
//    text = getCopyright(rrHandle);
//    if(text)
//    {
//    	printf(text);
//    }
//
//    freeText(text);

    if(hasError())
    {
        char* error = getLastError();
        printf("Last error %s \n", error);
    }
	return 0;
}

#pragma link "rr_cm.lib"
