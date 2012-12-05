//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("P:\mtk\Components\Dune\TFileSelectionFrame.cpp", FileSelectionFrame); /* TFrame: File Type */
USEFORM("MainForm.cpp", MForm);
//---------------------------------------------------------------------------
#pragma comment(lib, "mtkCommon.lib")
#pragma comment(lib, "VCLCommonRTL.bpi")
#pragma comment(lib, "roadrunner.lib")
#pragma comment(lib, "poco_foundation-static.lib")


WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
    try
    {
         Application->Initialize();
         Application->MainFormOnTaskBar = true;
         Application->CreateForm(__classid(TMForm), &MForm);
		Application->CreateForm(__classid(TFileSelectionFrame), &FileSelectionFrame);
		Application->Run();
    }
    catch (Exception &exception)
    {
         Application->ShowException(&exception);
    }
    catch (...)
    {
         try
         {
             throw Exception("");
         }
         catch (Exception &exception)
         {
             Application->ShowException(&exception);
         }
    }
    return 0;
}
//---------------------------------------------------------------------------
