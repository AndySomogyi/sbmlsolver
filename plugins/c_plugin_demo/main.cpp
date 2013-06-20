#pragma hdrstop
#pragma argsused
#include <iostream>
#include "rrc_core_api.h"
using namespace rrc;
using namespace std;

int main()
{
	try
    {
	    enableLoggingToConsole();
        setLogLevel("DEBUG");
        logMsg(clInfo, "Log message..");

        //Create a RoadRunner instance
        RRHandle rri =  createRRInstanceEx("r:\\temp");

        RRPluginHandle plugin = loadPlugin(rri, "c_plugin_demo");
        if(!plugin)
        {
        	throw("Demo plugin could not be loaded");
        }

        char* rrInfo;
        if(!executePluginEx(plugin, &rrInfo))
        {
        	throw("There was a problem executing the plugin...");
        }

        cout<< endl << "After Execute: "<< rrInfo << endl;
        freeText(rrInfo);

        cout<<getPluginInfo(plugin);
       	unLoadPlugins(rri);
        freeRRInstance(rri);
    }
    catch(const char* msg)
    {
    	clog<<"There was a problem: "<<msg;
    }
	return 0;
}

#pragma comment(lib, "rrc_api.lib")


