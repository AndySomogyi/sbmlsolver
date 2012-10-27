#include <iostream>
#include "poco/SharedLibrary.h"

using namespace std;
using namespace Poco;

typedef void (*HelloFunc)();
#pragma comment(lib, "IPHLPAPI.lib")
int main(int argc, char * argv[])
{
    string theFunc("poco_hello");
    string loadLib("poco_test");
    loadLib.append(SharedLibrary::suffix());

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

    char ch;
    cin>>ch;


    return 0;
}

