#pragma hdrstop
#pragma argsused
#include <iostream>
#include "rrRoadRunner.h"
#include "rrPlugin.h"
using namespace rr;
using namespace std;

int main()
{
    //Create a RoadRunner object
	RoadRunner rr("r:\\installs\\cg\\xe3\\debug\\support_code");

    //Get the plugin manager
    PluginManager& plugins = rr.getPluginManager();

    if(!plugins.Load())
    {
    	clog<<"Failed loading plugins..\n";
    }

    if(plugins.GetNumberOfPlugins() > 0)
    {
    	cout<<"The following plugins are loaded:\n";
        for(int i = 0; i < plugins.GetNumberOfPlugins(); i++)
        {
        	Plugin* aPlugin = plugins[i];
            cout<<"Plugin "<<i<<": "<<aPlugin->GetName()<<"\n";
            cout<<aPlugin->GetInfo();
            aPlugin->Execute();
        }
    }

    plugins.Unload();
    Pause(true);
	return 0;
}

#pragma comment(lib, "roadrunner-static.lib")
#pragma comment(lib, "poco_foundation-static.lib")
