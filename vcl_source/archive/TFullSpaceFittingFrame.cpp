#include <vcl.h>
#pragma hdrstop
#include <Classes.hpp>
#include "rrException.h"
#include "TFullSpaceFittingFrame.h"
#include "rrNewArrayList.h"
#include "rrUtils.h"
#include "rrMemoLogger.h"
#include "rrc_api.h"
#include "rrEndUserUtils.h"
#include "rrVCLUtils.h"
#include "MainForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "mtkFloatLabeledEdit"
#pragma link "mtkIntLabeledEdit"
#pragma resource "*.dfm"
TFullSpaceFittingFrame *FullSpaceFittingFrame;

using namespace rr;
typedef TFullSpaceFittingFrame fsff;
//---------------------------------------------------------------------------
__fastcall TFullSpaceFittingFrame::TFullSpaceFittingFrame(TComponent* Owner)
	:
TFrame(Owner),
infoMemo(NULL)
{}

void fsff::assignRRHandle(RRHandle aHandle)
{
	mRRI = aHandle;
}

void fsff::assignPluginHandle(RRPluginHandle aHandle)
{
	mPlugin = aHandle;
}

void TFullSpaceFittingFrame::loadParameterList()
{
    if(mRRI)
    {
        paraList->Clear();
        RRStringArrayHandle cSymbolsIDs = getGlobalParameterIds(mRRI);
		StringList symbols = convertCStringArray(cSymbolsIDs);
        addItemsToListBox(symbols, paraList);
    }
}

void __fastcall TFullSpaceFittingFrame::paraListClick(TObject *Sender)
{
	//Get the currently selected parameter
    int index = paraList->ItemIndex;
    if(index == -1)
    {
    	return;
    }

    string para = stdstr(paraList->Items->Strings[paraList->ItemIndex]);
    double val = 0;
    if(getGlobalParameterByIndex(mRRI, index, &val))
    {
    	Log()<<"Parameter "<<para<<" = "<<val;
    }
}

void __fastcall TFullSpaceFittingFrame::paraListClickCheck(TObject *Sender)
{
	//Fit only one parameter at a time
    int index = paraList->ItemIndex;
    if(index == -1)
    {
    	return;
    }

    for(int i = 0; i < paraList->Count; i++)
    {
    	if(i != index)
        {
			paraList->Checked[i] = false;
        }
    }
}

void __fastcall TFullSpaceFittingFrame::executeBtnClick(TObject *Sender)
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

    string strVal = stdstr(paraList->Items->Strings[checkedItem]);

	string msg = setPluginParameter(mPlugin, "Parameter to fit", strVal.c_str())
    	? "Assigned fitting parameter" : "Failed assigning fitting parameter";
    Log() << msg;

    strVal = getTempFolder(mRRI);

    msg = setPluginParameter(mPlugin, "TempFolder", strVal.c_str())
    ? "Updated tempfolder" : "Failed to set tempFolder";
    Log() << msg;

    strVal = getSBML(mRRI);
    msg = setPluginParameter(mPlugin, "SBML", strVal.c_str())
    ? "Assigned SBML" : "Failed assigning SBML";
    Log() << msg;

	if(stepsE->GetNumber() < 1)
    {
    	Log()<<"Error: Submit number of steps for fitting";
        return;
    }

    strVal = stepsE->GetNumberAsStdString();
    setPluginParameter(mPlugin, "Steps per dimension", strVal.c_str());

    strVal = sweepRangeE->GetNumberAsStdString();
    setPluginParameter(mPlugin, "Parameter sweep range", strVal.c_str());


	assignCallbacks(mPlugin, fsfStartedCB, fsfFinishedCB, this);
    TMainF *mainForm = (TMainF*) this->Owner;

    //This call returns before the result has been created, non blocking.
    //The result is returned in the callback assigned in the configurePlugin function
    executePluginEx(mPlugin, (void*) &(mainForm->mCurrentData));
}

void __stdcall TFullSpaceFittingFrame::fsfStartedCB(void *UserData)
{
    TFullSpaceFittingFrame *pThis = (TFullSpaceFittingFrame*) UserData;
    TThread::Synchronize(NULL, pThis->fittingStarted);
}

void __stdcall TFullSpaceFittingFrame::fsfFinishedCB(void *UserData)
{
    TFullSpaceFittingFrame *pThis = (TFullSpaceFittingFrame*) UserData;
    TThread::Synchronize(NULL, pThis->fittingFinished);
}

void __fastcall TFullSpaceFittingFrame::fittingStarted()
{
	Log()<<"Full space fitting was started";
    if(onFittingStarted)
    {
        onFittingStarted();
    }
}

string TFullSpaceFittingFrame::getResult()
{
	return getPluginResult(mPlugin);
}

void __fastcall TFullSpaceFittingFrame::fittingFinished()
{
	Log()<<"Full space fitting was finished";
    if(onFittingFinished)
    {
        onFittingFinished();
    }
}

//---------------------------------------------------------------------------
void __fastcall TFullSpaceFittingFrame::logResultAExecute(TObject *Sender)
{
	Log()<< getPluginResult(mPlugin);
}



