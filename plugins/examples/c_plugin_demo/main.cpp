#include <iostream>
#include "rrc_api.h"
#include "rrp_api.h"
using namespace rrc;
using namespace rrp;
using namespace std;

int main()
{
    try
    {
        enableLoggingToConsole();
        setLogLevel("LOG_Debug");
        logMsg(clInfo, "Log message....");

        RRPluginManagerHandle pm = createPluginManager();
        loadPlugins(pm);
        RRStringArray* list = getPluginNames(pm);

        if(!list)
        {
            throw("No Plugins..");
        }

        for(int i = 0; i < list->Count; i++)
        {
            cout << list->String[i] << endl;
        }
        freeStringArray(list);

        RRPluginHandle plugin = getFirstPlugin(pm);
        if(!plugin)
        {
            throw("Demo plugin could not be loaded");
        }

        //Create a RoadRunner instance
        RRHandle rri =  createRRInstanceEx("r:\\temp", "tcc.exe.");
        assignRoadRunnerInstance(plugin, rri);

        char* info;
        if(!executePluginEx(plugin, &info, false))
        {
            throw("There was a problem executing the plugin...");
        }

        cout<< endl << "After Execute: "<< info << endl;

        char* pluginInfo = getPluginInfo(plugin);

        if(pluginInfo)
        {
            cout<<pluginInfo;
            freeText(pluginInfo);
        }

        freeRRInstance(rri);

        //This will also unload all plugins..
        freePluginManager(pm);
    }
    catch(const char* msg)
    {
        clog<<"There was a problem: "<<msg;
    }
    return 0;
}



