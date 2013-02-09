
#pragma hdrstop
//---------------------------------------------------------------------------
#include <sstream>
#include "rrParameter.h"
#include "rrRoadRunner.h"
#include "rrPluginManager.h"
#include "rrPlugin.h"
#include "rrCGenerator.h"
#include "rrLogger.h"           //Might be useful for debugging later on
#include "rr_c_api.h"
#include "rr_c_api_support.h"   //Support functions, not exposed as api functions and or data
#include "rrException.h"
#include "rrUtils.h"
#include "rrStringUtils.h"
#include "rrCapability.h"
using namespace rr_c_api;
extern RoadRunner*     gRRHandle;

bool rrCallConv loadPlugins()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
        }

    	return gRRHandle->getPluginManager().load();
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
  	    return false;
    }
}

bool rrCallConv unLoadPlugins()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
        }

    	return gRRHandle->getPluginManager().unload();
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
  	    return false;
    }
}

int rrCallConv getNumberOfPlugins()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
        }

    	return gRRHandle->getPluginManager().getNumberOfPlugins();
    }
    catch(Exception& ex)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<ex.what()<<endl;
        setError(msg.str());
  	    return -1;
    }
}

RRStringArray* rrCallConv getPluginNames()
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
        }

        StringList names = gRRHandle->getPluginManager().getPluginNames();
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

RRStringArray* rrCallConv getPluginCapabilities(const char* pluginName)
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
        }

        Plugin* aPlugin = gRRHandle->getPluginManager().getPlugin(pluginName);
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

RRStringArray* rrCallConv getPluginParameters(const char* pluginName, const char* capability)
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
        }

        Plugin* aPlugin = gRRHandle->getPluginManager().getPlugin(pluginName);
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

RRParameter* rrCallConv getPluginParameter(const char* pluginName, const char* parameterName)
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
        }

        Plugin* aPlugin = gRRHandle->getPluginManager().getPlugin(pluginName);
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

bool rrCallConv setPluginParameter(const char* pluginName, const char* parameterName, const char* value)
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
        }

        Plugin* aPlugin = gRRHandle->getPluginManager().getPlugin(pluginName);
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

char* rrCallConv getPluginInfo(const char* name)
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
        }

        Plugin* aPlugin = gRRHandle->getPluginManager().getPlugin(name);
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

bool rrCallConv executePlugin(const char* name)
{
	try
    {
        if(!gRRHandle)
        {
            setError(ALLOCATE_API_ERROR_MSG);
        }

        Plugin* aPlugin = gRRHandle->getPluginManager().getPlugin(name);
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
