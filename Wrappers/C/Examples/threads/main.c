#pragma hdrstop
#include <stdio.h>
#include "rr_c_api.h"

int main()
{
    //Some Declarations (has to be here because this is C)
	RRInstanceListHandle 	rrs;
    TPHandle 			    tpHandle;		//ThreadPool handle.. use to check when a pool of threads has finished..

	char* modelFileName = "r://models//test_1.xml";
    int   handleCount = 50;
    int   threadCount = 8;
    int   i;
   	char  buf[2048];
	printf("Starting C program...\n");

    rrs = createRRHandles(handleCount);

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
	//Instead, one can could check for activeJobs, i.e. non blocking (see below)
    waitForJobs(tpHandle);

    //Set parameters
    logMsg(clInfo, " ---------- SETTING PARAMETERS -------------");

    //Setup instances with different variables
    for(i = 0; i < handleCount; i++)
    {
        double val = 0;
        getValue(rrs->Handle[i], "k1", &val);
        setValue(rrs->Handle[i], "k1", val/(2.5*(i + 1)));
        setNumPoints(rrs->Handle[i], 500);
        setTimeEnd(rrs->Handle[i], 150);
        setTimeCourseSelectionList(rrs->Handle[i], "S1");
    }

    //Simulate
    logMsg(clInfo, " ---------- SIMULATING ---------------------");

    //Simulate them using a pool of threads..
    tpHandle = simulateTP(rrs, 8);
    waitForJobs(tpHandle);


  	//Write data to a file
	writeMultipleRRData(rrs, "r:\\allData.dat");

	// Cleanup
    freeRRHandles(rrs);

	if(hasError())
    {
        char* error = getLastError();
        sprintf(buf, "Last error %s \n", error);
    }
	return 0;
}

#pragma link "rr_c_api.lib"

//Non blocking code waiting for threadpool to finish
//    while(true)
//    {
//		int nrOfRemainingJobs = getNumberOfRemainingJobs(tpHandle);
//        if (nrOfRemainingJobs == 0)
//        {
//           	logMsg(lInfo, "All jobs are done!!!\n");
//        	break;
//        }
//        else
//        {
//        	sprintf(buf, "There are %d remaining jobs\n", nrOfRemainingJobs);
//        	logMsg(lInfo, buf);
//            sleep(0.1);
//        }
//	}

