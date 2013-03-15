//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("MainForm.cpp", MForm);
//---------------------------------------------------------------------------
#if defined(STATIC_BUILD)
#pragma comment(lib, "mtkCommon-static.lib")
#pragma comment(lib, "roadrunner-static.lib")
#else
#pragma comment(lib, "mtkCommon.lib")
#pragma comment(lib, "roadrunner.lib")
#endif

//#pragma comment(lib, "VCLCommon.bpi")
#pragma comment(lib, "rr-libstruct-static.lib")
#pragma comment(lib, "poco_foundation-static.lib")


WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
    try
    {
         Application->Initialize();
         Application->MainFormOnTaskBar = true;
         Application->CreateForm(__classid(TMForm), &MForm);
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
