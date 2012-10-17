//---------------------------------------------------------------------------

#pragma hdrstop
#include <tchar.h>
#include <iostream>
//---------------------------------------------------------------------------
extern "C"
{
#include "C:/DataOutput/00173/00173-sbml-l2v4.h"
}

#pragma argsused
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	cout<<"Hello ....\n";

//    gTheModel._gp[0] = 2;
	InitModel();
    cout<<"Global parameter is now: "<<_gp[0]<<"\n";
    cout<<GetModelName();
	return 0;
}

