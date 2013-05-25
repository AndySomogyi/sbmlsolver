
#pragma hdrstop
//---------------------------------------------------------------------------
#include <sstream>
#include "rrParameter.h"
#include "rrRoadRunner.h"
#include "rrPluginManager.h"
#include "rrPlugin.h"
#include "rrCGenerator.h"
#include "rrLogger.h"           //Might be useful for debugging later on
#include "rrc_api.h"
#include "rrc_support.h"   //Support functions, not exposed as api functions and or data
#include "rrException.h"
#include "rrUtils.h"
#include "rrStringUtils.h"


using namespace rrc;
using namespace rr;
using rr::Exception;
rr::RoadRunner*     gRRHandle;

namespace rrc
{
using namespace std;
using namespace rr;
using namespace rrc;



//PLUGIN Functions
bool rrCallConv loadPlugins(RRHandle handle)
{
	try
    {
        RoadRunner* rri = castFrom(handle);
    	return rri->getPluginManager().load();
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
  	    return false;
    }
}

bool rrCallConv unLoadPlugins(RRHandle handle)
{
	try
    {
        RoadRunner* rri = castFrom(handle);
    	return rri->getPluginManager().unload();
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
  	    return false;
    }
}

int rrCallConv getNumberOfPlugins(RRHandle handle)
{
	try
    {
        RoadRunner* rri = castFrom(handle);
    	return rri->getPluginManager().getNumberOfPlugins();
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
  	    return -1;
    }
}

RRStringArray* rrCallConv getPluginNames(RRHandle handle)
{
	try
    {
        RoadRunner* rri = castFrom(handle);
        StringList names = rri->getPluginManager().getPluginNames();
        return createList(names);
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
  	    return NULL;
    }
}

RRStringArray* rrCallConv getPluginCapabilities(RRHandle handle, const char* pluginName)
{
	try
    {
        RoadRunner* rri = castFrom(handle);
        Plugin* aPlugin = rri->getPluginManager().getPlugin(pluginName);
        if(aPlugin)
        {
        	StringList aList;
            vector<Capability>* caps = aPlugin->getCapabilities();
            if(!caps)
            {
            	return NULL;
            }

            for(int i = 0; i < caps->size(); i++)
            {
            	aList.Add((*caps)[i].getName());
            }
        	return createList(aList);
        }
        else
        {
	        return NULL;
        }
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
  	    return NULL;
    }
}

RRStringArray* rrCallConv getPluginParameters(RRHandle handle, const char* pluginName, const char* capability)
{
	try
    {
        RoadRunner* rri = castFrom(handle);
        Plugin* aPlugin = rri->getPluginManager().getPlugin(pluginName);
        if(aPlugin)
        {
        	StringList aList;
            Parameters* paras = aPlugin->getParameters(capability);
            if(!paras)
            {
            	return NULL;
            }

            for(int i = 0; i < paras->size(); i++)
            {
            	aList.Add((*paras)[i]->getName());
            }
        	return createList(aList);
        }
        else
        {
	        return NULL;
        }
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
  	    return NULL;
    }
}

RRParameter* rrCallConv getPluginParameter(RRHandle handle, const char* pluginName, const char* parameterName)
{
	try
    {
        RoadRunner* rri = castFrom(handle);
        Plugin* aPlugin = rri->getPluginManager().getPlugin(pluginName);
        if(aPlugin)
        {

            rr::BaseParameter *para = aPlugin->getParameter(parameterName);
        	return createParameter( *(para) );
        }
        return NULL;

    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
  	    return NULL;
    }
}

bool rrCallConv setPluginParameter(RRHandle handle, const char* pluginName, const char* parameterName, const char* value)
{
	try
    {
        RoadRunner* rri = castFrom(handle);
        Plugin* aPlugin = rri->getPluginManager().getPlugin(pluginName);
        if(aPlugin)
        {
            return aPlugin->setParameter(parameterName, value);
        }
		return false;

    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
  	    return NULL;
    }
}

char* rrCallConv getPluginInfo(RRHandle handle, const char* name)
{
	try
    {
        RoadRunner* rri = castFrom(handle);
        Plugin* aPlugin = rri->getPluginManager().getPlugin(name);
        if(aPlugin)
        {
        	return createText(aPlugin->getInfo());
        }
        else
        {
	        return createText("No such plugin: " + string(name));
        }
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
  	    return NULL;
    }
}

bool rrCallConv executePlugin(RRHandle handle, const char* name)
{
	try
    {
        RoadRunner* rri = castFrom(handle);
        Plugin* aPlugin = rri->getPluginManager().getPlugin(name);
        if(aPlugin)
        {
        	return aPlugin->execute();
        }
        else
        {
			return false;
        }
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
  	    return NULL;
    }
}

}
