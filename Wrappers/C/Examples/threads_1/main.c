#pragma hdrstop
#if defined(linux)
#include <stdlib.h>
#include <string.h>
#endif
#include <stdio.h>
#include "rr_c_api.h"

int main()
{
    //Some Declarations (has to be here because this is C)
	RRHandle 				rrHandle;
    RRThreadHandle			threadHandle;
    char tempFolder[1024];
	int i;
    double val;
	char* modelFileName = "../models/test_1.xml";
   	char  buf[2048];
	// -------------------------------------------------------------

	printf("Starting C program...\n");

    rrHandle = createRRInstance();

    if(!rrHandle)
    {
        printf("No handles...\n");
    }
    else
    {
	    printf("Handles allocated succesfully..\n");
    }

   	setLogLevel("Info");
    strcpy(tempFolder, "../temp");
    if(!setTempFolder(rrHandle, tempFolder))
    {
    	printf("The temp file folder \'%s\' do not exist. Exiting...\n", tempFolder);
        exit(0);
    }
	enableLoggingToConsole();
   	enableLoggingToFile(rrHandle);

	//loadSBML models in threads instead
    threadHandle = loadSBMLFromFileThread(rrHandle, modelFileName);

    //waitForJob will block until the thread haa finished
	//Instead, one can could check for activeJob, i.e. non blocking (see below)
    waitForJob(threadHandle);

    //Set parameters
    logMsg(clInfo, " ---------- SETTING PARAMETERS -------------");

    //Setup instances with different variables
    val = 0;
    getValue(rrHandle, "k1", &val);
    setValue(rrHandle, "k1", val/(2.5*(i + 1)));
    setNumPoints(rrHandle, 500);
    setTimeEnd(rrHandle, 150);
    setTimeCourseSelectionList(rrHandle, "TIME S1");


    //Simulate
    logMsg(clInfo, " ---------- SIMULATING ---------------------");

    //Simulate them using a pool of threads..
    threadHandle = simulateThread(rrHandle);

    waitForJob(threadHandle);

  	//Write data to a file
	writeRRData(rrHandle, "oneThreadData.dat");

	// Cleanup
    freeRRInstance(rrHandle);

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

