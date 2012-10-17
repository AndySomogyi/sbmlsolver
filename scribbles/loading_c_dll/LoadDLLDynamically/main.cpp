//---------------------------------------------------------------------------
#include <windows.h>
#pragma hdrstop
#include <tchar.h>
#include <iostream>
//---------------------------------------------------------------------------
#pragma argsused
using namespace std;

const int MAXMODULE = 512;

typedef int (WINAPI*cfunc)();

cfunc InitModel;

int _tmain(int argc, _TCHAR* argv[])
{
	cout<<"Hello ....\n";

//    wstring dll(L"C:\\RRW\\Testing\\C-program\\LoadModelSource\\Debug\\Win32\\ModelDLL.dll");
	wstring dll(L"C:\\RRW\\Testing\\rr_code_output\\c_from_rr++\\00001-sbml-l2v4.dll");
    HINSTANCE hLib=LoadLibrary(dll.c_str());

    if(hLib==NULL)
    {
    	cout << "Unable to load library!" << endl;
        return -1;
    }

	TCHAR mod[MAXMODULE];

    GetModuleFileName((HMODULE)hLib, (LPTSTR) mod, MAXMODULE);
    wstring name(mod);


    wcout << "Library loaded: " <<name.c_str() << endl;

	InitModel = (cfunc) GetProcAddress((HMODULE) hLib, "InitModel");
    if(InitModel==NULL)
    {
        cout << "Unable to load function." << endl;
        FreeLibrary((HMODULE)hLib);
        return -1;
    }


    cout<<"Calling init...";
    int res = InitModel();
    cout<<"The result was: "<<res;

//    cout<<"Global parameter is now"<<gTheModel._gp[0]<<"\n";
    FreeLibrary((HMODULE)hLib);
	return 0;
}

