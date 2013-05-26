#pragma hdrstop
#pragma argsused
#include <iostream>
#include "rrException.h"
#include "rrRoadRunner.h"
#include "rrPlugin.h"
#include "rrUtils.h"
using namespace rr;
using namespace std;

int main()
{
	try
    {
        //Create a RoadRunner object
        RoadRunner rr("r:\\temp");

        //Get the plugin manager
        PluginManager& plugins = rr.getPluginManager();

        if(!plugins.load())
        {
            clog<<"Failed loading plugins..\n";
        }

        if(plugins.getNumberOfPlugins() > 0)
        {
            cout<<"The following plugins are loaded:\n";
            for(int i = 0; i < plugins.getNumberOfPlugins(); i++)
            {
                Plugin* aPlugin = plugins[i];
                cout<<"Plugin "<<i<<": "<<aPlugin->getName()<<"\n";
                cout<<aPlugin->getInfo();
                aPlugin->execute();
            }
        }

        plugins.unload();
        pause(true);
    }
    catch(const rr::Exception& ex)
    {
    	clog<<"There was a problem: "<<ex.what();
    }
	return 0;
}

#pragma comment(lib, "roadrunner.lib")

#pragma comment(lib, "rr-libstruct-static.lib")
#pragma comment(lib, "poco_foundation-static.lib")
