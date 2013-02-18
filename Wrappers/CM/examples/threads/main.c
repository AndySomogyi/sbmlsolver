#pragma hdrstop
#include <stdio.h>
#include "rr_cm.h"

int main()
{
    //Declarations..
	RRHandle rrHandle;
	char* text;
	char* modelFileName = "r://models//test_1.xml";
    char* sbml;
    RRCCodeHandle codeHandle;
	//.....

	printf("Starting C program...");

    rrHandle =  getRRHandle();

    if(!rrHandle)
    {
        printf("No handle...");
    }
    else
    {
	    printf("Handle allocated succesfully..");
    }


	text = getBuildDate();

	if(text)
	{
		printf("\nBuild date: %s", text);
		freeText(text);
	}

    setTempFolderH(rrHandle, "r:\\rrTemp");
	enableLoggingH(rrHandle);
    setLogLevel("Info");


    //To get the C Code, the code needs to be generated
    if(!loadSBMLFromFileH(rrHandle, modelFileName))
    {
    	printf("Failed loading SBML.\n");
        printf("Last error: %s", getLastError());
        return -1;
    }

	// Cleanup
    freeRRInstance(rrHandle);


	//Finish off..
    text = getCopyright(rrHandle);
    if(text)
    {
    	printf(text);
    }

    freeText(text);

    if(hasError())
    {
        char* error = getLastError();
        printf("Last error %s \n", error);
    }
	return 0;
}

#pragma link "rr_cm.lib"
