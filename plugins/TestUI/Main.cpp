//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "rrUtils.h"
#include "Main.h"
#include "rr_c_api.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMainF *MainF;

#pragma comment(lib, "rr_c_api.lib")
#pragma comment(lib, "roadrunner-static.lib")

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
void __fastcall TMainF::loadPluginsExecute(TObject *Sender)
{
	if(!loadPlugins())
    {
    	Log() << "Failed loading plugins..";
    }
    	Log() << "Failed loading plugins..";
}

