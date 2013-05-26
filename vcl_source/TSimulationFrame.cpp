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
#include "rrParameter.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "mtkFloatLabeledEdit"
#pragma link "mtkIntLabeledEdit"
#pragma resource "*.dfm"
TSimulateFrame *SimulateFrame;

__fastcall TSimulateFrame::TSimulateFrame(TComponent* Owner)
	:
TFrame(Owner),
infoMemo(NULL),
onSimulationFinished(NULL)
{}

//---------------------------------------------------------------------------
void TSimulateFrame::assignRRHandle(RRHandle aHandle)
{
	mRRI = aHandle;
}

bool TSimulateFrame::loadSelectionList()
{
    if(mRRI)
    {
        SelList->Clear();
		paraList->Clear();
        mParameters.clear();

        SelList->Items->Add("Time");
        SelList->Checked[0] = true;
        RRListHandle cSymbols = getAvailableTimeCourseSymbols(mRRI);
		NewArrayList symbolArray = convertCList(cSymbols);

        int count = symbolArray.Count();

        StringList fs       = symbolArray.GetStringList("Floating Species");
        addItemsToListBox(fs, SelList, true);

        StringList bs       = symbolArray.GetStringList("Boundary Species");
        addItemsToListBox(bs, SelList);

        EnableDisableSimulation();
        freeRRList(cSymbols);

        RRStringArrayHandle cSymbolsIDs = getGlobalParameterIds(mRRI);
		StringList symbols = convertCStringArray(cSymbolsIDs);

        for(int i = 0; i < symbols.Count(); i++)
        {
        	string parName = symbols[i];
        	double value;
            if(!getValue(mRRI, parName.c_str(), &value))
            {
            	ML()<<"There was a problem with parameter: "<<parName;
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
        }
	    freeStringArray(cSymbolsIDs);
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
        ML()<<"Enabling simulation..";
    }
    else
    {
        ML()<<"Disabling simulation..";
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
    StringList list = getCheckedItems(SelList);
    string selected = list.AsString();
    setTimeCourseSelectionList(mRRI, selected.c_str());

	reset(mRRI);

    mSimJobH = simulateJobEx(	mRRI,
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
	ML()<<"Simulation was started at: " << getTime();
}

void __fastcall TSimulateFrame::simulationFinished()
{
	ML()<<"Simulation was finished at: "<< getTime();
    if(onSimulationFinished)
    {
        onSimulationFinished();	//
    }
}

void __fastcall TSimulateFrame::paraListClick(TObject *Sender)
{
	//Get the currently selected parameter
    int index = paraList->ItemIndex;
    if(index == -1)
    {
    	return;
    }

    Parameter<double>* para = (Parameter<double>*) paraList->Items->Objects[paraList->ItemIndex];

   	ML()<<"Parameter "<<para->getName()<<" = "<<para->getValue();
	paraEdit->EditLabel->Caption = vclstr(para->getName());
    paraEdit->FNumber = para->getValuePointer();
	paraEdit->Update();

}

void __fastcall TSimulateFrame::paraEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if(Key == VK_RETURN)
    {
    	//Update parameter values
        for(int i = 0; i < mParameters.count(); i++)
        {
        	Parameter<double>* para = (Parameter<double>*) mParameters[i];
            if(!setValue(mRRI, para->getName().c_str(), para->getValue() ))
            {
            	ML()<<"There was a problem with parameter: "<<para->getName();
            }
        }
		//Simulate...
		simulateA->Execute();
    }
}


