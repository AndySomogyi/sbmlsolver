#include <vcl.h>
#pragma hdrstop

USEFORM("..\..\..\vcl_source\TSimulationFrame.cpp", SimulateFrame); /* TFrame: File Type */
USEFORM("..\..\..\vcl_source\TFullSpaceFittingFrame.cpp", FullSpaceFittingFrame);
USEFORM("MainForm.cpp", MainF);
//---------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->CreateForm(__classid(TMainF), &MainF);
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

#pragma comment(lib, "poco_foundation-static.lib")


