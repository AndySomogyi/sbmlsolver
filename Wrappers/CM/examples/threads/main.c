#pragma hdrstop
#include <stdio.h>
#include "rr_cm.h"

int main()
{
    //Some Declarations (has to be here because this is C)
	RRInstanceListHandle 	rrs;
    TPHandle *tpHandle;		//ThreadPool handle.. use to check when a pool of threads has finished..

	char* modelFileName = "r://models//test_1.xml";
    int   handleCount = 100;
    int   threadCount = 4;
    int   i;
   	char  buf[2048];
	printf("Starting C program...\n");

    rrs = getRRHandles(handleCount);

    if(!rrs)
    {
        printf("No handles...\n");
    }
    else
    {
	    printf("Handles allocated succesfully..\n");
    }

   	setLogLevel("INfo");
	enableLoggingToConsole();

    for(i = 0; i < handleCount; i++)
    {
    	setTempFolder(rrs->Handle[i], "r:\\rrTemp");
    }
   	enableLoggingToFile(rrs->Handle[0]);

	//loadSBML models in threads instead
    tpHandle = loadModelFromFileTP(rrs, modelFileName, threadCount);

    //waitForJobs will block until all threads have finished
//    waitForJobs(tpHandle);
	//Instead, one can check for activeJobs, i.e. non blocking
    while(true)
    {
		int nrOfRemainingJobs = getNumberOfRemainingJobs(tpHandle);
        if (nrOfRemainingJobs == 0)
        {
           	logMsg(lInfo, "All jobs are done!!!\n");
        	break;
        }
        else
        {
        	sprintf(buf, "There are %d remaining jobs\n", nrOfRemainingJobs);
        	logMsg(lInfo, buf);
            sleep(0.1);
        }
    }

	// Cleanup
    freeRRHandles(rrs);

	if(hasError())
    {
        char* error = getLastError();
        sprintf(buf, "Last error %s \n", error);
    }
	return 0;
}

#pragma link "rr_cm.lib"
