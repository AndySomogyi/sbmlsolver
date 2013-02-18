#pragma hdrstop
#include <stdio.h>
#include "rr_cm.h"

int main()
{
    //Declarations..
	RRHandle *rrHandles;
	char* text;
	char* modelFileName = "r://models//test_1.xml";
    char* sbml;
    int   handleCount;
    int i;
	//.....

    handleCount = 100;
	printf("Starting C program...");


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
    for(i = 0; i < handleCount; i++)
    {
    	setTempFolder(rrHandles[i], "r:\\rrTemp");
		enableLoggingToConsole(rrHandles[i]);
		enableLoggingToFile(rrHandles[i]);
    }

	//loadSBML models in threads instead
    loadSBMLFromFile(rrHandles, modelFileName);


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
