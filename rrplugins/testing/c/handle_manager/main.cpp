#include <iostream>
#include "telException.h"
#include "telUtils.h"
#include "telPluginManager.h"
#include "telPlugin.h"
#include "telplugins_c_api.h"
#pragma hdrstop

using namespace std;
using namespace tlp;
using namespace tlpc;
int main()
{
    try
    {
        string fldr(joinPath("..", "plugins"));
        TELHandle pm = tpCreatePluginManager(fldr.c_str());

        if(!tpLoadPlugins(pm))
        {
            cout<<tpGetLastError();
        }

        TELHandle aPlugin = tpGetFirstPlugin(pm);

        //Pass a handle of wrong type
        TELHandle aPlugin2 = tpGetFirstPlugin(aPlugin);
        if(!aPlugin2)
        {
            cout<<tpGetLastError();
        }

        //Plugin handles are supported too
        int bad = 123;
        char* name = tpGetPluginName(&bad);
        if(!name)
        {
            cout<<tpGetLastError();
        }


        return 0;
    }
    catch(const std::exception& ex)
    {
        cout<<ex.what();
    }
}
