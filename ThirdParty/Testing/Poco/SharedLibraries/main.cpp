#pragma hdrstop
#pragma argsused
#if defined(WIN32)
#include <conio.h>
#endif

#include <iostream>
#include "Poco/SharedLibrary.h"

using namespace std;
using namespace Poco;

typedef void (*HelloFunc)();
#pragma comment(lib, "IPHLPAPI.lib")

#if defined(CG_IDE)
#pragma comment(lib, "poco_foundation-static.lib")
#endif

int main(int argc, char* argv[])
{
	string loadLib("SharedLibTest");
	loadLib.append(SharedLibrary::suffix());
 	string theFunc("poco_hello");
    SharedLibrary lib(loadLib);

    if(lib.isLoaded())
    {
        cout<<"The lib: "<<loadLib<<" was loaded\n";
    }
    else
    {
        cout<<"Failed loading lib: "<<loadLib<<"\n";
    }

    if(lib.hasSymbol(theFunc))
    {
        HelloFunc func = (HelloFunc) lib.getSymbol(theFunc);
        func();
    }
    else
    {
        cout<<"Could not find symbol: "<<theFunc<<endl;
    }

#if defined(WIN32)
    cout<<"\nPress any key to exit...";
    cin.ignore(0,'\n');
    getch();
#endif

	return 0;
}
