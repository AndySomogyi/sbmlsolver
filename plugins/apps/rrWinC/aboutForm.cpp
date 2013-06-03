#include <vcl.h>
#pragma hdrstop
#include "aboutForm.h"
#include "rrStringList.h"
#include "rrVCLUtils.h"
#include "rrStringUtils.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TrrAboutForm *rrAboutForm;

using namespace rr;
//---------------------------------------------------------------------------
__fastcall TrrAboutForm::TrrAboutForm(RRHandle handle, TComponent* Owner)
:
TForm(Owner),
mRRI(handle)
{
	//Populate the memo
    Memo1->Lines->Clear();
	Memo1->Lines->Add(vclstr("C API Version: " + string(getAPIVersion())));
	Memo1->Lines->Add(vclstr("Build Date: " + string(getBuildDate())));
	Memo1->Lines->Add(vclstr("Build Time: " + string(getBuildTime())));



	string info 	   = getExtendedAPIInfo();
    StringList strings = splitString(info, "\n");

    for(int i = 0; i < strings.Count(); i++)
    {
		Memo1->Lines->Add(strings[i].c_str());
    }

	Memo1->Lines->Add("");
	Memo1->Lines->Add("=========== Current Instance =============");
	info 	   = getInfo(mRRI);
    strings = splitString(info, "\n");

    for(int i = 0; i < strings.Count(); i++)
    {
		Memo1->Lines->Add(strings[i].c_str());
    }


}
//---------------------------------------------------------------------------

void __fastcall TrrAboutForm::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
 	if(Key == VK_ESCAPE)
    {
        Close();
    }
}


