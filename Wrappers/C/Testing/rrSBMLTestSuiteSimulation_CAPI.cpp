//---------------------------------------------------------------------------

#pragma hdrstop
#include "rrUtils.h"
#include "rrSimulationData.h"
#include "rrSBMLTestSuiteSimulation_CAPI.h"

using namespace rr;
SimulationData convertCAPIResultData(RRResultHandle		resultsHandle);

SBMLTestSuiteSimulation_CAPI::SBMLTestSuiteSimulation_CAPI(const string& dataOutputFolder, const string& modelFilePath, const string& modelFileName)
:
rr::TestSuiteModelSimulation(dataOutputFolder, modelFilePath, modelFileName)
{
}

SBMLTestSuiteSimulation_CAPI::~SBMLTestSuiteSimulation_CAPI()
{

}

void SBMLTestSuiteSimulation_CAPI::UseHandle(RRHandle handle)
{
	mRRHandle = handle;
}


bool SBMLTestSuiteSimulation_CAPI::LoadSBMLFromFile()
{
	if(!mRRHandle)
    {
    	return false;
    }

    return loadSBMLFromFile(GetModelsFullFilePath().c_str());
}

bool SBMLTestSuiteSimulation_CAPI::LoadSettings(const string& settingsFName)
{
    mModelSettingsFileName = (settingsFName);

    if(!mModelSettingsFileName.size())
    {
        mModelSettingsFileName = JoinPath(mModelFilePath, GetSettingsFileNameForCase(mCurrentCaseNumber));
    }
	SBMLModelSimulation::LoadSettings(mModelSettingsFileName);

	return loadSimulationSettings(mModelSettingsFileName.c_str());
}

bool SBMLTestSuiteSimulation_CAPI::Simulate()
{
    if(!mRRHandle)
    {
        return false;
    }
	mResultHandle = simulate();

    if(mResultHandle)
    {
		mResultData = convertCAPIResultData(mResultHandle);
    }
    return mResultHandle ? true : false;
}

bool SBMLTestSuiteSimulation_CAPI::SaveResult()
{
    string resultFileName(JoinPath(mDataOutputFolder, "rrCAPI_" + mModelFileName));
    resultFileName = ChangeFileExtensionTo(resultFileName, ".csv");

    if(!mResultHandle)
    {
    	return false;
    }

    ofstream fs(resultFileName.c_str());
    fs << mResultData;
    fs.close();
    return true;
}

SimulationData convertCAPIResultData(RRResultHandle	result)
{
	SimulationData resultData;

	StringList colNames;
	//Copy column names
    for(int i = 0; i < result->CSize; i++)
    {
    	colNames.Add(result->ColumnHeaders[i]);
    }

	resultData.SetColumnNames(colNames);

    //Then the data
    int index = 0;
    resultData.Allocate(result->RSize, result->CSize);

    for(int j = 0; j < result->RSize; j++)
    {
        for(int i = 0; i < result->CSize; i++)
        {
            resultData(j,i) = result->Data[index++];
        }
    }

	return resultData;

}
