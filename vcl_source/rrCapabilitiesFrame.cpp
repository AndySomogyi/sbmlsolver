#include <vcl.h>
#pragma hdrstop
#include "rrMemoLogger.h"
#include "rrCapabilitiesFrame.h"
#include "rrVCLUtils.h"


//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TcapFrame *capFrame;

using namespace rr;
using namespace rrc;

__fastcall TcapFrame::TcapFrame(TComponent* Owner)
: TFrame(Owner),
infoMemo(NULL)
{
}

void TcapFrame::Populate(RRHandle rr, const StringList& caps)
{
	mRR = rr;
    if(!mRR)
    {
    	return;
    }

    for(int i = 0; i < caps.Count(); i++)
    {
        capDropDown->AddItem(vclstr(caps[i]), NULL);
    }

    capDropDown->ItemIndex = 0;
	capDropDownChange(capDropDown);
}

void __fastcall TcapFrame::capDropDownClick(TObject *Sender)
{
//    //Populate plugin frame
//	capDropDown->Clear();
//    pluginParasCB->Clear();
//
//    RRStringArray* caps = getPluginCapabilities(mCurrentlySelectedPlugin);
//
//    if(!caps)
//    {
//	    GroupBox5->Enabled = false;
//    	return;
//    }
//
//    GroupBox5->Enabled = true;

}
//---------------------------------------------------------------------------
//string TMainF::getCurrentSelectedParameter()
//{
//    if(pluginParasCB->ItemIndex == -1)
//    {
//    	return "";
//    }
//
//    string pluginParaName = stdstr(pluginParasCB->Items->Strings[pluginParasCB->ItemIndex]);
//	return pluginParaName;
//}

//
////---------------------------------------------------------------------------
//void __fastcall TMainF::SetParaBtnClick(TObject *Sender)
//{
//	int iVal;
//    double dVal;
//	string paraText = stdstr(paraEdit->Text);
//
//	switch(RadioGroup1->ItemIndex)
//    {
//    	case 0:
//        break;
//        case 1:
//        break;
//        case 3:
//        	dVal =  rr::ToDouble(paraText);
//			setPluginParameter(mCurrentlySelectedPlugin, getCurrentSelectedParameter().c_str(), (char*) &dVal);
//        break;
//    }
//}


void __fastcall TcapFrame::capDropDownChange(TObject *Sender)
{
    //Change available parameters in the parasdropDown...
    parameterDropDown->Clear();

    string cap = stdstr(capDropDown->Items->Strings[capDropDown->ItemIndex]);
    RRStringArray* paras = getListOfParameters(mRR, cap.c_str());
    parameterDropDown->Clear();

    if(!paras)
    {
        ML()<<"No parameters!";
        return;
    }

    for(int i =0; i < paras->Count; i++)
    {
        parameterDropDown->AddItem(paras->String[i], NULL);
    }
    parameterDropDown->ItemIndex = 0;
	parameterDropDownChange(Sender);
}

void __fastcall TcapFrame::parameterDropDownChange(TObject *Sender)
{
	string capName  = getCurrentCapabilityName();
   	string paraName = getCurrentParameterName();
    RRParameterHandle para = getParameter(mRR, capName.c_str(), paraName.c_str());
    if(!para)
    {
        paraEdit->Enabled = false;
	    SetParaBtn->Enabled = false;
        return;
    }

    paraEdit->Enabled = true;
	SetParaBtn->Enabled = true;
    parameterDropDown->Hint = getParameterHint(para);

    string value = stdstr(getParameterValueAsString(para));
    if(value.size())
    {
		paraEdit->Text = vclstr(value);
    }
    else
    {
		paraEdit->Text = "<none>";
    }
}

//---------------------------------------------------------------------------
string TcapFrame::getCurrentCapabilityName()
{
    return stdstr(capDropDown->Items->Strings[capDropDown->ItemIndex]);
}

string TcapFrame::getCurrentParameterName()
{
    return stdstr(parameterDropDown->Items->Strings[parameterDropDown->ItemIndex]);
}

void __fastcall TcapFrame::SetParaBtnClick(TObject *Sender)
{
	string capName  = getCurrentCapabilityName();
   	string paraName = getCurrentParameterName();

    RRParameterHandle para = getParameter(mRR, capName.c_str(), paraName.c_str());
    if(!para)
    {
	    SetParaBtn->Enabled = false;
        paraEdit->Enabled = false;
        return;
    }
    string text = stdstr(paraEdit->Text);

    if(!setParameter(para, text.c_str()))
    {
    	ML()<<"Bad....";
    }
}


