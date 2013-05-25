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

        AddItemsToListBox(symbols, paraList);
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

void __fastcall TFullSpaceFittingFrame::executeBtnClick(TObject *Sender)
{
    string strVal = getTempFolder(mRRI);
    string msg = (setPluginParameter(mPlugin, "TempFolder", (char*) &strVal)) ? "Updated tempfolder" : "Failed to set tempFolder";
    Log() << msg;

    strVal = getSBML(mRRI);
    msg = (setPluginParameter(mPlugin, "SBML", (char*) &strVal)) ? "Assigned SBML" : "Failed assigning SBML";
    Log() << msg;

	vector<string> parasToFit;
    parasToFit.push_back("k1");

    int dummy = stepsE->GetValue();
    setPluginParameter(mPlugin, "Steps per dimension", (char*) &dummy);

    double dummyDbl = sweepRangeE->GetValue();
    setPluginParameter(mPlugin, "Parameter sweep range", (char*) &dummyDbl);


	msg = setPluginParameter(mPlugin, "Parameters to fit", (char*) &parasToFit) ? "Assigned fitting parameter" : "Failed assigning fitting parameter";
    Log() << msg;

	assignCallbacks(mPlugin, fsfStartedCB, fsfFinishedCB, this);
    TMainF *mf = (TMainF*) this->Owner;

    //This call returns before the result has been created, non blocking.
    //The result is returned in the callback assigned in the configurePlugin function
    executePluginEx(mPlugin, (void*) &(mf->mCurrentData));
}

void __stdcall TFullSpaceFittingFrame::fsfStartedCB(void *UserData)
{
    TFullSpaceFittingFrame *pThis = (TFullSpaceFittingFrame*) UserData;
    TThread::Synchronize(NULL, pThis->onFSFStarted);
}

void __stdcall TFullSpaceFittingFrame::fsfFinishedCB(void *UserData)
{
    TFullSpaceFittingFrame *pThis = (TFullSpaceFittingFrame*) UserData;
    TThread::Synchronize(NULL, pThis->onFSFFinished);
}

void __fastcall TFullSpaceFittingFrame::onFSFStarted()
{
	Log()<<"Full space fitting was started";
}

void __fastcall TFullSpaceFittingFrame::onFSFFinished()
{
	Log()<<"Full space fitting finished";
}

