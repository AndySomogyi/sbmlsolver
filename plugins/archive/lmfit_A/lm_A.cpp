#pragma hdrstop
#include "rrLogger.h"
#include "lm_A.h"
#include "../../Wrappers/C/rrc_api.h"
#include "../../Wrappers/C/rrc_utilities.h"
#include "rrRoadRunner.h"
#include "rrRoadRunnerData.h"
#include "rrUtils.h"
//---------------------------------------------------------------------------

using namespace rr;
using namespace rrc;

LM_A::LM_A(rr::RoadRunner* aRR)
:
Plugin(				    "LM_A",   					"No Category", 		aRR),
mLM_AFit(	 	  	 	"LM_AFit", 					"", 				"Run a one species fit"),	//The 'capability'
mTempFolder(			"TempFolder", 	  			"",  				"Tempfolder used in the fitting"),
mSBML(	    			"SBML", 					"<none>",			"SBML, i.e. the model to be used in the fitting"),
mMinimizationData(		"MinData", 					MinimizationData(),	"Data structure holding minimization data"),
mLM_AFitThread(*this)
{
	//Setup the plugins capabilities
    mLM_AFit.addParameter(&mTempFolder);
    mLM_AFit.addParameter(&mSBML);
    mLM_AFit.addParameter(&mMinimizationData);
    mCapabilities.add(mLM_AFit);
}

LM_A::~LM_A()
{}

bool LM_A::resetPlugin()
{
	if(mLM_AFitThread.isRuning())
    {
    	return false;
    }

	MinimizationData& data = getMinimizationData();
    data.reset();
	return true;
}

MinimizationData& LM_A::getMinimizationData()
{
	return *(mMinimizationData.getValuePointer());
}

string LM_A::getTempFolder()
{
	return mTempFolder.getValue();
}

string LM_A::getSBML()
{
	return mSBML.getValue();
}

string LM_A::getResult()
{
	return "";
}

bool LM_A::setInputData(void* inputData)
{
	//Cast data to RRData structire
    RRDataHandle data = (RRData*) inputData;

    if(!data)
    {
    	return false;
	}

    RoadRunnerData rrData;
    rrData.reSize(data->RSize, data->CSize);

    for(int r = 0; r < data->RSize; r++)
    {
    	for(int c = 0; c < data->CSize; c++)
    	{
			rrData(r,c) = data->Data[r*data->CSize + c];
        }
    }

    StringList colNames;
    for(int c = 0; c < data->CSize; c++)
    {
		colNames.Add(data->ColumnHeaders[c]);
    }

    rrData.setColumnNames(colNames);

	MinimizationData& minData = getMinimizationData();

    minData.setInputData(rrData);
    return true;
}

bool LM_A::execute(void* inputData)
{
   	Log(lInfo)<<"Executing the LM_A plugin";
	//	mResult <<"LM_A was started on: "<<getDateTime() <<"\n";

    //go away and carry out the work in a thread
    //Assign callback functions to communicate the progress of the thread
	mLM_AFitThread.assignCallBacks(mWorkStartedCB, mWorkFinishedCB, mUserData);

    mLM_AFitThread.start();
	return true;
}

// Plugin factory function
rr::Plugin* __stdcall createPlugin(rr::RoadRunner* aRR)
{
    //allocate a new object and return it
	return new LM_A(aRR);
}


#if defined(CG_UI)
    #if defined(STATIC_BUILD)
//    	#pragma comment(lib, "roadrunner-static.lib")
    #else
    	#pragma comment(lib, "rrc_api.lib")
    	#pragma comment(lib, "roadrunner.lib")
    	#pragma comment(lib, "poco_foundation-static.lib")
    	#pragma comment(lib, "rr-libstruct-static.lib")
    #endif
#endif


