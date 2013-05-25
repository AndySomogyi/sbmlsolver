#include <vcl.h>
#pragma hdrstop
#include <Classes.hpp>
#include "rrVCLUtils.h"
#include "rrException.h"
#include "TSimulationFrame.h"
#include "rrNewArrayList.h"
#include "rrUtils.h"
#include "rrc_api.h"
#include "rrMemoLogger.h"
#include "rrEndUserUtils.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "mtkFloatLabeledEdit"
#pragma link "mtkIntLabeledEdit"
#pragma resource "*.dfm"
TSimulateFrame *SimulateFrame;

//---------------------------------------------------------------------------
__fastcall TSimulateFrame::TSimulateFrame(TComponent* Owner)
	:
TFrame(Owner),
infoMemo(NULL),
onSimulationFinished(NULL)
{}

//---------------------------------------------------------------------------
void TSimulateFrame::assignRRHandle(RRHandle aHandle)
{
	mRRHandle = aHandle;
}

bool TSimulateFrame::loadSelectionList()
{
    if(mRRHandle)
    {
        SelList->Clear();
		paraListB->Clear();
        SelList->Items->Add("Time");
        SelList->Checked[0] = true;
        RRListHandle cSymbols = getAvailableTimeCourseSymbols(mRRHandle);
		NewArrayList symbols = convertCList(cSymbols);

        int count = symbols.Count();

        StringList fs       = symbols.GetStringList("Floating Species");
        AddItemsToListBox(fs, SelList);

        StringList bs       = symbols.GetStringList("Boundary Species");
        AddItemsToListBox(bs, SelList);

        StringList gp       = symbols.GetStringList("Global Parameters");
        AddItemsToListBox(gp, paraListB);

        EnableDisableSimulation();
    }
    return true;
}


//---------------------------------------------------------------------------
void TSimulateFrame::EnableDisableSimulation()
{
    //Check if there is at least one checked species in the list box
    bool enableDisable = false;

    for(int i = 0; i < SelList->Count; i++)
    {
        if(SelList->Checked[i])
        {
            enableDisable = true;
            break;
        }
    }

    if(enableDisable)
    {
        Log()<<"Enabling simulation..";
    }
    else
    {
        Log()<<"Disabling simulation..";
    }
    mStartTimeE->Enabled            = enableDisable;
    mEndTimeE->Enabled              = enableDisable;
    mNrOfSimulationPointsE->Enabled = enableDisable;
    simulateA->Enabled              = enableDisable;
}

void __fastcall TSimulateFrame::SelListClick(TObject *Sender)
{
    EnableDisableSimulation();
}

void __fastcall TSimulateFrame::simulateAExecute(TObject *Sender)
{
    mSimJobH = simulate();
}

RRJobHandle __fastcall TSimulateFrame::simulate()
{
    //Setup selection list
    StringList list = GetCheckedItems(SelList);
    string selected = list.AsString();
    setTimeCourseSelectionList(mRRHandle, selected.c_str());

	reset(mRRHandle);

    mSimJobH = simulateJobEx(	mRRHandle,
    							mStartTimeE->GetValue(),
                                mEndTimeE->GetValue(),
                                mNrOfSimulationPointsE->GetValue(),
    							ThreadEnterCB,		//Callback called as the thread starts its job
                                ThreadExitCB,       //Callback called as the thread finishes its job
                                this);              //void* userData, needed when inside the callback function

    return mSimJobH;
}

void __stdcall TSimulateFrame::ThreadEnterCB(void *UserData)
{
    TSimulateFrame *pThis = (TSimulateFrame*) UserData;
    TThread::Synchronize(NULL, pThis->simulationStarted);
}

void __stdcall TSimulateFrame::ThreadExitCB(void *UserData)
{
    TSimulateFrame *pThis = (TSimulateFrame*) UserData;
    TThread::Synchronize(NULL, pThis->simulationFinished);
}

void __fastcall TSimulateFrame::simulationStarted()
{
	Log()<<"Simulation was started at: " << getTime();

}

void __fastcall TSimulateFrame::simulationFinished()
{
	Log()<<"Simulation was finished at: "<< getTime();
    if(onSimulationFinished)
    {
        onSimulationFinished();
    }
}

