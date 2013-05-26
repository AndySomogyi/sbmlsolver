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
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "mtkFloatLabeledEdit"
#pragma link "mtkIntLabeledEdit"
#pragma resource "*.dfm"
TLMFittingFrame *LMFittingFrame;

using namespace rr;
typedef TLMFittingFrame fsff;
//---------------------------------------------------------------------------
__fastcall TLMFittingFrame::TLMFittingFrame(TComponent* Owner)
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

void TLMFittingFrame::loadParameterList()
{
    if(mRRI)
    {
        paraList->Clear();
        RRStringArrayHandle cSymbolsIDs = getGlobalParameterIds(mRRI);
		StringList symbols = convertCStringArray(cSymbolsIDs);
        AddItemsToListBox(symbols, paraList);
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

    string para = stdstr(paraList->Items->Strings[paraList->ItemIndex]);
    double val = 0;
    if(getGlobalParameterByIndex(mRRI, index, &val))
    {
    	Log()<<"Parameter "<<para<<" = "<<val;
    }
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

	assignCallbacks(mPlugin, fsfStartedCB, fsfFinishedCB, this);
    TMainF *mainForm = (TMainF*) this->Owner;

    //This call returns before the result has been created, non blocking.
    //The result is returned in the callback assigned in the configurePlugin function
    executePluginEx(mPlugin, (void*) &(mainForm->mCurrentData));
}

void __stdcall TLMFittingFrame::fsfStartedCB(void *UserData)
{
    TLMFittingFrame *pThis = (TLMFittingFrame*) UserData;
    TThread::Synchronize(NULL, pThis->fittingStarted);
}

void __stdcall TLMFittingFrame::fsfFinishedCB(void *UserData)
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
}

//---------------------------------------------------------------------------
void __fastcall TLMFittingFrame::logResultAExecute(TObject *Sender)
{
	Log()<< getPluginResult(mPlugin);
}



