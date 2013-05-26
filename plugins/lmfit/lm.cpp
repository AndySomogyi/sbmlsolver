#pragma hdrstop
#include "rrLogger.h"
#include "lm.h"
#include "../../Wrappers/C/rrc_api.h"
#include "../../Wrappers/C/rrc_utilities.h"
#include "rrRoadRunner.h"
#include "rrRoadRunnerData.h"
#include "rrUtils.h"
//---------------------------------------------------------------------------

using namespace rr;
using namespace rrc;

LM::LM(rr::RoadRunner* aRR)
:
Plugin(				    "Levenberg-Marquardt Minimization",   			"No Category", 		aRR),
mLMFit(	 	  	 		"LMFit", 					"", 				"Run a one species fit"),	//The 'capability'
mTempFolder(			"TempFolder", 	  			"",  				"Tempfolder used in the fitting"),
mSBML(	    			"SBML", 					"<none>",			"SBML, i.e. the model to be used in the fitting"),
mMinimizationData(		"MinData", 					MinimizationData(),	"Data structure holding minimization data"),
mLMFitThread(*this)
{
	//Setup the plugins capabilities
    mLMFit.addParameter(&mTempFolder);
    mLMFit.addParameter(&mSBML);
    mLMFit.addParameter(&mMinimizationData);
    mCapabilities.add(mLMFit);
}

LM::~LM()
{}

bool LM::resetPlugin()
{
	if(mLMFitThread.isRuning())
    {
    	return false;
    }

	MinimizationData& data = getMinimizationData();
    data.reset();
	return true;
}

MinimizationData& LM::getMinimizationData()
{
	return *(mMinimizationData.getValuePointer());
}

string LM::getTempFolder()
{
	return mTempFolder.getValue();
}

string LM::getSBML()
{
	return mSBML.getValue();
}

string LM::getResult()
{
	return "";
}

bool LM::setInputData(void* inputData)
{
	//Cast data to RRData structire
    RRDataHandle data = (RRData*) inputData;

    if(!data)
    {
    	return false;
	}

    RoadRunnerData rrData(data->RSize, data->CSize);

	//Column Names
    StringList colNames;
    for(int c = 0; c < data->CSize; c++)
    {
		colNames.add(data->ColumnHeaders[c]);
    }
    rrData.setColumnNames(colNames);

    //The data
    for(int r = 0; r < data->RSize; r++)
    {
    	for(int c = 0; c < data->CSize; c++)
    	{
			rrData(r,c) = data->Data[r*data->CSize + c];
        }
    }

	//Weights ?
    if(data->Weights != NULL)
    {
    	rrData.allocateWeights();
        for(int r = 0; r < data->RSize; r++)
        {
            for(int c = 0; c < data->CSize; c++)
            {
                rrData.setWeight(r,c) = data->Weights[r*data->CSize + c];
            }
        }
    }



	MinimizationData& minData = getMinimizationData();
    minData.setInputData(rrData);
    return true;
}

bool LM::execute(void* inputData)
{
   	Log(lInfo)<<"Executing the LM plugin";
	//	mResult <<"LM was started on: "<<getDateTime() <<"\n";

    //go away and carry out the work in a thread
    //Assign callback functions to communicate the progress of the thread
	mLMFitThread.assignCallBacks(mWorkStartedCB, mWorkFinishedCB, mUserData);

    mLMFitThread.start();
	return true;
}

// Plugin factory function
rr::Plugin* __stdcall createPlugin(rr::RoadRunner* aRR)
{
    //allocate a new object and return it
	return new LM(aRR);
}


#if defined(CG_UI)
    #if defined(STATIC_RR)
    	#pragma comment(lib, "roadrunner-static.lib")
    #else
    	#pragma comment(lib, "rrc_api.lib")
    	#pragma comment(lib, "roadrunner.lib")
    	#pragma comment(lib, "poco_foundation-static.lib")
    	#pragma comment(lib, "rr-libstruct-static.lib")
		#pragma comment(lib, "lmfit-static.lib")
    #endif
#endif


