//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("..\..\..\vcl_source\TSimulationFrame.cpp", SimulateFrame); /* TFrame: File Type */
USEFORM("..\..\..\vcl_source\TLMFittingFrame.cpp", LMFittingFrame);
USEFORM("..\..\..\vcl_source\rrCapabilitiesFrame.cpp", capFrame); /* TFrame: File Type */
USEFORM("MainForm.cpp", MainF);
USEFORM("aboutForm.cpp", rrAboutForm);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->CreateForm(__classid(TMainF), &MainF);
		Application->CreateForm(__classid(TrrAboutForm), &rrAboutForm);
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
#if defined(STATIC_RR)
//	#pragma comment(lib, "rrc_api-static.lib")
//	#pragma comment(lib, "roadrunner-static.lib")
#else
	#pragma comment(lib, "rrc_api.lib")
	#pragma comment(lib, "roadrunner.lib")
#endif

#pragma comment(lib, "mtkCommon-static.lib")
#pragma comment(lib, "poco_foundation-static.lib")


