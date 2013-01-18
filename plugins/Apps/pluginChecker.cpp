#pragma hdrstop
#pragma argsused

#include <iostream>
#include "rrRoadRunner.h"
#include "rrPlugin.h"
using namespace rr;
using namespace std;

int main()
{
	RoadRunner rr("r:\\installs\\cg\\xe3\\debug\\support_code");
    PluginManager& plugins = rr.getPluginManager();
    if(!plugins.Load())
    {
    	clog<<"Failed loading plugins..\n";
    }

    if(plugins.GetNumberOfPlugins() > 0)
    {
    	cout<<"The following plugins were loaded:\n";
        for(int i = 0; i < plugins.GetNumberOfPlugins(); i++)
        {
        	Plugin* aPlugin = plugins[i];
            cout<<"Plugin "<<i<<": "<<aPlugin->GetName()<<"\n";
            cout<<aPlugin->GetInfo();
        }

    }

    plugins.Unload();
	return 0;
}

#pragma comment(lib, "roadrunner.lib")
#pragma comment(lib, "poco_foundation-static.lib")
