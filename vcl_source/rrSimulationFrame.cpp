//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <Classes.hpp>
#include "rrException.h"
#include "rrSimulationFrame.h"
#include "rrNewArrayList.h"
#include "rrUtils.h"
#include "memoLogger.h"
#include "rrc_api.h"
#include "rrEndUserUtils.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "mtkFloatLabeledEdit"
#pragma link "mtkIntLabeledEdit"
#pragma resource "*.dfm"
TrrSettingFrame *rrSettingFrame;

//---------------------------------------------------------------------------
__fastcall TrrSettingFrame::TrrSettingFrame(TComponent* Owner)
	:
TFrame(Owner),
infoMemo(NULL),
onSimulationFinished(NULL)
{}

//---------------------------------------------------------------------------
void TrrSettingFrame::assignRRHandle(RRHandle aHandle)
{
	mRRHandle = aHandle;
}

bool TrrSettingFrame::loadSelectionList()
{
    if(mRRHandle)
    {
        SelList->Clear();
        SelList->Items->Add("Time");
        SelList->Checked[0] = true;
        RRListHandle cSymbols = getAvailableTimeCourseSymbols(mRRHandle);
		NewArrayList symbols = convertCList(cSymbols);

        int count = symbols.Count();

        Log() << symbols.AsString();

        StringList fs       = symbols.GetStringList("Floating Species");
        Log() << fs;
        StringList bs       = symbols.GetStringList("Boundary Species");
        StringList vols     = symbols.GetStringList("Volumes");
        StringList gp       = symbols.GetStringList("Global Parameters");
        StringList fluxes   = symbols.GetStringList("Fluxes");

        AddItemsToListBox(fs);
        AddItemsToListBox(bs);
        AddItemsToListBox(vols);
        AddItemsToListBox(gp);
        AddItemsToListBox(fluxes);
        EnableDisableSimulation();
    }
    return true;
}

void TrrSettingFrame::AddItemsToListBox(const StringList& items)
{
    Log()<<items;

    for(int i = 0; i < items.Count(); i++)
    {
        SelList->Items->Add(items[i].c_str());
        SelList->Checked[i] = true;
    }
}

//---------------------------------------------------------------------------
void TrrSettingFrame::EnableDisableSimulation()
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

void __fastcall TrrSettingFrame::SelListClick(TObject *Sender)
{
    EnableDisableSimulation();
}

void __fastcall TrrSettingFrame::simulateAExecute(TObject *Sender)
{
    mSimJobH = simulate();
}

RRJobHandle __fastcall TrrSettingFrame::simulate()
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

void TrrSettingFrame::ThreadEnterCB(void *UserData)
{
    TrrSettingFrame *pThis = (TrrSettingFrame*) UserData;
    TThread::Synchronize(NULL, pThis->simulationStarted);
}

void TrrSettingFrame::ThreadExitCB(void *UserData)
{
    TrrSettingFrame *pThis = (TrrSettingFrame*) UserData;
    TThread::Synchronize(NULL, pThis->simulationFinished);
}

void __fastcall TrrSettingFrame::simulationStarted()
{
	Log()<<"Simulation was started at: " << getTime();

}

void __fastcall TrrSettingFrame::simulationFinished()
{
	Log()<<"Simulation was finished at: "<< getTime();
    if(onSimulationFinished)
    {
        onSimulationFinished();
    }
}

