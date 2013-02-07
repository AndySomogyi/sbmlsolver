//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "rrUtils.h"
#include "Main.h"
#include "rr_c_api.h"
#include "rrException.h"
#include "utils.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMainF *MainF;

//---------------------------------------------------------------------------
__fastcall TMainF::TMainF(TComponent* Owner)
:
TForm(Owner)
{
	startupTimer->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TMainF::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if(Key == VK_ESCAPE)
	{
		Close();
	}
}

void __fastcall TMainF::startupTimerTimer(TObject *Sender)
{
	startupTimer->Enabled = false;
	mTheAPI = getRRInstance();
	if(!mTheAPI)
	{

	}
	else
	{
    	loadSBMLFromFile("R:\\installs\\cg\\xe3\\debug\\models\\test_1.xml");
		apiVersionLBL->Caption = getVersion();
		buildDateLbl->Caption  = getBuildDate();
		buildTimeLbl->Caption  = getBuildTime();
		string info 		   = getInfo();

		vector<string> lines = rr::SplitString(info, "\n");
		for(int i =0; i < lines.size(); i++)
		{
			infoMemo->Lines->Add(lines[i].c_str());
		}
	}
}

//---------------------------------------------------------------------------
void __fastcall TMainF::loadPluginsAExecute(TObject *Sender)
{
	if(!loadPlugins())
	{
		Log() << "failed loading plugins..";
		throw Exception(getLastError());
	}

	//Populate list box with plugins
	RRStringArray* pluginNames = getPluginNames();

	for(int i = 0; i < pluginNames->Count; i++)
	{
		pluginList->AddItem(pluginNames->String[i], NULL);
	}
	Log() << "Loaded plugins..";
    Button1->Action = unloadPlugins;
}

void __fastcall TMainF::unloadPluginsExecute(TObject *Sender)
{
	if(!unLoadPlugins())
	{
		Log() << "failed un-loading plugins..";
		throw Exception(getLastError());
	}

    pluginList->Clear();
	Log() << "Un-Loaded plugins..";
    Button1->Action = loadPluginsA;
}

void __fastcall TMainF::ApplicationEvents1Exception(TObject *Sender, Exception *E)
{
	Log() << std_str(E->ToString()) <<endl;
}
void __fastcall TMainF::pluginListClick(TObject *Sender)
{
	//Retrieve the name of the plugin that was clicked
    if(pluginList->ItemIndex == -1)
    {
    	return;
    }

    string pluginName = std_str(pluginList->Items->Strings[pluginList->ItemIndex]);
    Log()<<std_str(pluginName);

    string test = getPluginInfo(pluginName.c_str());

    infoMemo->Clear();
    Log()<<test;

    //Populate plugin frame (not a frame yet..)
    //Parameter CB
    RRStringArray* paras = getPluginParameters(pluginName.c_str());
    pluginParasCB->Clear();

    if(!paras)
    {
    	Log()<<"No parameters!";
	    pluginParasCBChange(NULL);
    	return;
    }
    for(int i =0; i < paras->Count; i++)
    {
        pluginParasCB->AddItem(paras->String[i], NULL);
    }
    pluginParasCB->ItemIndex = 0;
    pluginParasCBChange(NULL);
}

void __fastcall TMainF::clearMemoExecute(TObject *Sender)
{
	infoMemo->Clear();
}
//---------------------------------------------------------------------------


void __fastcall TMainF::getPluginInfoAExecute(TObject *Sender)
{
	string pName = getCurrentPluginName();
    Log()<<getPluginInfo(pName.c_str());
}

//---------------------------------------------------------------------------
string TMainF::getCurrentPluginName()
{
    if(pluginList->ItemIndex == -1)
    {
    	return "";
    }

    string pluginName = std_str(pluginList->Items->Strings[pluginList->ItemIndex]);
	return pluginName;
}

string TMainF::getCurrentSelectedParameter()
{
    if(pluginParasCB->ItemIndex == -1)
    {
    	return "";
    }

    string pluginParaName = std_str(pluginParasCB->Items->Strings[pluginParasCB->ItemIndex]);
	return pluginParaName;

}

void __fastcall TMainF::pluginParasCBChange(TObject *Sender)
{
	//Query the current plugin for the current value of selected parameter
	RRParameterHandle para = getPluginParameter(getCurrentPluginName().c_str(), getCurrentSelectedParameter().c_str());
    if(!para)
    {
		paraEdit->Enabled = false;
    	return;
    }

	paraEdit->Enabled = true;
	pluginParasCB->Hint = para->mHint;

    if(para->ParaType == ptInteger)
    {
    	paraEdit->Text = rr::ToString(para->data.iValue).c_str();
    }
    else
    {
		paraEdit->Text = "not implemented";
    }

}

//---------------------------------------------------------------------------
void __fastcall TMainF::SetParaBtnClick(TObject *Sender)
{

	setPluginParameter(getCurrentPluginName().c_str(), getCurrentSelectedParameter().c_str(), std_str(paraEdit->Text).c_str());
}
//---------------------------------------------------------------------------

void __fastcall TMainF::executePluginAExecute(TObject *Sender)
{
	executePlugin(getCurrentPluginName().c_str());
}
//---------------------------------------------------------------------------

void __fastcall TMainF::getLastErrorAExecute(TObject *Sender)
{
	Log()<<getLastError();
}
//---------------------------------------------------------------------------

