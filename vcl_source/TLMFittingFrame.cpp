#include <vcl.h>
#pragma hdrstop
#include <Classes.hpp>
#include "rrException.h"
#include "TLMFittingFrame.h"
#include "rrNewArrayList.h"
#include "rrUtils.h"
#include "rrMemoLogger.h"
#include "rrc_api.h"
#include "rrEndUserUtils.h"
#include "rrVCLUtils.h"
#include "MainForm.h"
#include "rrMinimizationData.h"
#include "rrc_support.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "mtkFloatLabeledEdit"
#pragma link "mtkIntLabeledEdit"
#pragma resource "*.dfm"
TLMFittingFrame *LMFittingFrame;

using namespace rr;
//typedef TLMFittingFrame fsff;
//---------------------------------------------------------------------------
__fastcall TLMFittingFrame::TLMFittingFrame(TComponent* Owner)
	:
TFrame(Owner),
infoMemo(NULL)
{}

void TLMFittingFrame::assignRRHandle(RRHandle aHandle)
{
	mRRI = aHandle;
}

void TLMFittingFrame::assignPluginHandle(RRPluginHandle aHandle)
{
	mPlugin = aHandle;
}

void TLMFittingFrame::loadParameterList()
{
    if(mRRI)
    {
        paraList->Clear();
        mParameters.clear();
        RRStringArrayHandle cSymbolsIDs = getGlobalParameterIds(mRRI);
		StringList symbols = convertCStringArray(cSymbolsIDs);

        for(int i = 0; i < symbols.Count(); i++)
        {
        	string parName = symbols[i];
        	double value;
            if(!getValue(mRRI, parName.c_str(), &value))
            {
            	Log()<<"There was a problem with parameter: "<<parName;
            }
            else
            {
        		mParameters.add(new Parameter<double>(parName, value, ""));
            }
        }
 	  	for(int i = 0; i < mParameters.count(); i++)
    	{
	       int index = paraList->Items->Add(mParameters[i]->getName().c_str());
           paraList->Items->Objects[i] = (TObject*) mParameters[i];
           paraList->Checked[index] = true;
        }
    }
}

void __fastcall TLMFittingFrame::paraListClick(TObject *Sender)
{
	//Get the currently selected parameter
    int index = paraList->ItemIndex;
    if(index == -1)
    {
    	return;
    }

    Parameter<double>* para = (Parameter<double>*) paraList->Items->Objects[paraList->ItemIndex];
   	Log()<<"Parameter "<<para->getName()<<" = "<<para->getValue();
    paraEdit->FNumber = para->getValuePointer();
	paraEdit->Update();
}

void __fastcall TLMFittingFrame::executeBtnClick(TObject *Sender)
{
	int checkedItem = -1;
	for(int i = 0; i < paraList->Count; i++)
    {
        if(paraList->Checked[i])
        {
        	checkedItem = i;
            break;
        }
    }

    if(checkedItem == -1)
    {
    	Log()<<"Error: Please select a parameter to fit";
        return;
    }

    //Reset on each run
	resetPlugin(mPlugin);

    //Get the plugins minimizationData handle
    RRParameterHandle minDataParaHandle = getPluginParameter(mPlugin, "MinData", NULL);
    if(!minDataParaHandle)
    {
    	throw("bad....");
    }

    RRMinimizationDataHandle minData = getParameterValueAsPointer(minDataParaHandle);

    for(int i = 0; i < mParameters.count(); i++)
    {
    	Parameter<double>* para = (Parameter<double>*) mParameters[i];
    	addDoubleParameter(minData, para->getName().c_str(), para->getValue());
    }

    RRStringArrayHandle selList = getTimeCourseSelectionList(mRRI);

	setMinimizationSelectionList(minData, selList);

    RRData* rrData = rrc::createRRData(MainF->mCurrentData);
	setInputData(mPlugin, rrData);

    string strVal;
	string msg;
    strVal = getTempFolder(mRRI);

    msg = setPluginParameter(mPlugin, "TempFolder", strVal.c_str())
    ? "Updated tempfolder" : "Failed to set tempFolder";
    Log() << msg;

    strVal = getSBML(mRRI);
    msg = setPluginParameter(mPlugin, "SBML", strVal.c_str())
    ? "Assigned SBML" : "Failed assigning SBML";
    Log() << msg;

	assignCallbacks(mPlugin, fittingStartedCB, fittingFinishedCB, this);
    TMainF *mainForm = (TMainF*) this->Owner;

    //This call returns before the result has been created, non blocking.
    //The result is returned in the callback assigned in the configurePlugin function
    executePluginEx(mPlugin, (void*) &(mainForm->mCurrentData));
}

void __stdcall TLMFittingFrame::fittingStartedCB(void *UserData)
{
    TLMFittingFrame *pThis = (TLMFittingFrame*) UserData;
    TThread::Synchronize(NULL, pThis->fittingStarted);
}

void __stdcall TLMFittingFrame::fittingFinishedCB(void *UserData)
{
    TLMFittingFrame *pThis = (TLMFittingFrame*) UserData;
    TThread::Synchronize(NULL, pThis->fittingFinished);
}

void __fastcall TLMFittingFrame::fittingStarted()
{
	Log()<<"Full space fitting was started";
    if(onFittingStarted)
    {
        onFittingStarted();
    }
}

string TLMFittingFrame::getResult()
{
	return getPluginResult(mPlugin);
}

void __fastcall TLMFittingFrame::fittingFinished()
{
	Log()<<"Full space fitting was finished";
    if(onFittingFinished)
    {
        onFittingFinished();
    }

    //Get the plugins minimizationData handle
    RRParameterHandle minDataParaHandle = getPluginParameter(mPlugin, "MinData", NULL);
    if(!minDataParaHandle)
    {
    	throw("bad....");
    }

    RRMinimizationDataHandle _minData = getParameterValueAsPointer(minDataParaHandle);
    MinimizationData& minData = *(MinimizationData*) _minData;
    RoadRunnerData inputData = minData.getInputData();
	RoadRunnerData modelData = minData.getModelData();
	RoadRunnerData residuals = minData.getResidualsData();

	Chart1->Series[0]->Clear();
	Chart1->Series[1]->Clear();
	Chart1->Series[2]->Clear();
    //Plot the input, model and residual data..
    for(int i = 0; i < inputData.rSize(); i++)
    {
        double xVal = inputData(i,0);
        double yData = modelData(i, 1);

        Chart1->Series[0]->AddXY(xVal, yData);

        yData = inputData(i, 1);
        Chart1->Series[1]->AddXY(xVal, yData);

        yData = residuals(i, 1);
        Chart1->Series[2]->AddXY(xVal, yData);

    }
    Chart1->Update();
}

//---------------------------------------------------------------------------
void __fastcall TLMFittingFrame::logResultAExecute(TObject *Sender)
{
	Log()<< getPluginResult(mPlugin);
}



