//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "rrUtils.h"
#include "rrStringUtils.h"
#include "CAPI.h"
#include "mtkVCLUtils.h"
#include "rrLogger.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMainForm *MainForm;
using namespace rr;

//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
:
TForm(Owner),
mDLLFileName("rr_c_api.dll"),
mDLLFilePath("c:\\rrInstalls\\xe\\bin")
{
    gLog.Init("", gLog.GetLogLevel(), unique_ptr<LogFile>(new LogFile("rrCAPI.log")));
    gLog.SetCutOffLogLevel(lDebug5);
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
    if(Key == VK_ESCAPE)
    {
        Close();
    }
}

void __fastcall TMainForm::LoadDLLExecute(TObject *Sender)
{

    string fName = JoinPath(mDLLFilePath, mDLLFileName);
    if(FileExists(OpenDialog1->FileName))
    {
        mDLLFileName = ToSTDString(OpenDialog1->FileName);
        //Load the DLL and its functions and data..
        if(!rr::LoadDLL(mDLLFileName))
        {

        }
        Log(lDebug)<<"The C API DLL was loaded"<<mDLLFilePath;
    }
    else
    {
        Log(lDebug)<<"The C API DLL was not found in folder: "<<mDLLFilePath;
    }

}

