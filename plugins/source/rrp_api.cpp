#pragma hdrstop
#include <sstream>
#include "rrParameter.h"
#include "rrRoadRunner.h"
#include "rrPluginManager.h"
#include "rrPlugin.h"
#include "rrLogger.h"           //Might be useful for debugging later on
#include "rrException.h"
#include "rrUtils.h"
#include "rrStringUtils.h"
#include "rrc_api.h"
#include "rrc_macros.h"
#include "rrc_cpp_support.h"
#include "rrp_parameter_api.h"
#include "rrp_api.h"
#include "rrMinimizationData.h"

#include <set>

namespace rrp
{
using namespace std;
using namespace rr;

/**
 * set of all plugin managers we've created,
 *
 * used to check if if a given void* is a plugin manager
 * that we've created.
 */
static std::set<PluginManager*> pluginManagers;

static PluginManager* castToPluginManager(RRPluginManagerHandle handle)
{
    if (handle)
    {
        PluginManager *pm = static_cast<PluginManager*>(handle);
        if (pluginManagers.find(pm) != pluginManagers.end())
        {
            return pm;
        }
        else
        {
            Log(Logger::PRIO_ERROR) << "the handle " << pm << ", " << handle << " is NOT a valid PluginManager";
        }
    }
    else
    {
        Log(Logger::PRIO_ERROR) << "the handle is NULL";
    }

    throw (Exception("Invalid PluginManager handle"));
}

Plugin* castToPlugin(RRPluginHandle handle)
{
    Plugin* plugin = (Plugin*) handle;
    if(plugin) //Will only fail if handle is NULL...
    {
        return plugin;
    }
    else
    {
        Exception ex("Failed to cast to a valid Plugin handle");
        throw(ex);
    }
}

RRPluginManagerHandle rrpCallConv createPluginManager(RRHandle rrHandle)
{
    try
    {
        RoadRunner *rr = static_cast<RoadRunner*>(rrHandle);

        Log(Logger::PRIO_NOTICE) << __FUNC__ << "RoadRunner: " << rr;

        std::string pluginDir = joinPath(
                getParentFolder(gDefaultSupportCodeFolder), "plugins");

        PluginManager* pm = new PluginManager(rr, pluginDir, false);
        pluginManagers.insert(pm);

        Log(Logger::PRIO_NOTICE) << __FUNC__ << " created plugin manager: " << pm;
        return pm;
    }
    catch_ptr_macro
}

/**
 * create an instance of a plugin managager attached to the given RoadRunner instance.
 */
RRPluginManagerHandle rrpCallConv createPluginManagerEx(const char* pluginDir, bool autoLoad, RRHandle rrHandle)
{
    try
    {
        RoadRunner *rr = static_cast<RoadRunner*>(rrHandle);
        PluginManager* pm = new PluginManager(rr, pluginDir, autoLoad);
        pluginManagers.insert(pm);
        return pm;
    }
    catch_ptr_macro
}

/**
 * free the plugin manager
 */
bool rrpCallConv freePluginManager(RRPluginManagerHandle handle)
{
    try
    {
        PluginManager *pm = castToPluginManager(handle);
        pluginManagers.erase(pm);
        delete pm;
        return true;
    }
    catch_bool_macro
}

//PLUGIN Functions
RRPluginHandle rrCallConv loadPlugin(RRPluginManagerHandle handle, char* pluginName)
{
    try
    {
        PluginManager *pm = castToPluginManager(handle);
        if(pm->load(pluginName))
        {
            return pm->getPlugin(pluginName);
        }
        else
        {
            setError("Failed loading plugin: " + string(pluginName));
            return NULL;
        }
    }
    catch_ptr_macro
}

bool rrCallConv loadPlugins(RRPluginManagerHandle handle)
{
    try
    {
        PluginManager *pm = castToPluginManager(handle);
        return pm->load();
    }
    catch_bool_macro
}

bool rrCallConv unLoadPlugins(RRPluginManagerHandle handle)
{
    try
    {
        PluginManager *pm = castToPluginManager(handle);
        return pm->unloadAll();
    }
    catch_bool_macro
}

bool rrCallConv unLoadPlugin(RRPluginManagerHandle handle, RRPluginHandle plugin)
{
    try
    {
        PluginManager *pm = castToPluginManager(handle);
        Plugin* aPlugin = castToPlugin(plugin);
        if(pm->unload(aPlugin))
        {
            aPlugin = NULL;
            return true;
        }
        else
        {
            return false;
        }
    }
    catch_bool_macro
}

int rrCallConv getNumberOfPlugins(RRPluginManagerHandle handle)
{
    try
    {
        Log(Logger::PRIO_NOTICE) << __FUNC__;
        PluginManager *pm = castToPluginManager(handle);
        return pm->getNumberOfPlugins();
    }
    catch_int_macro
}

RRPluginHandle rrpCallConv getPlugin(RRPluginManagerHandle handle, const char* pluginName)
{
    try
    {
        PluginManager *pm = castToPluginManager(handle);
        Plugin* aPlugin = pm->getPlugin(pluginName);
        return aPlugin;
    }
    catch_ptr_macro
}

RRPluginHandle rrpCallConv getPluginByID(RRPluginManagerHandle handle, int id)
{
    try
    {
        PluginManager *pm = castToPluginManager(handle);
        Plugin* aPlugin = pm->getPlugin(id);
        return aPlugin;
    }
    catch_ptr_macro
}

RRStringArray* rrCallConv getPluginNames(RRPluginManagerHandle handle)
{
    try
    {
        PluginManager *pm = castToPluginManager(handle);
        StringList names = pm->getPluginNames();
        return createList(names);
    }
    catch_ptr_macro
}

char* rrCallConv getPluginName(RRPluginHandle handle)
{
    try
    {
        Plugin* aPlugin = castToPlugin(handle);
        return rr::createText(aPlugin->getName());
    }
    catch_ptr_macro
}

RRStringArray* rrCallConv getPluginCapabilities(RRPluginHandle handle)
{
    try
    {
        Plugin* aPlugin = castToPlugin(handle);
        if(aPlugin)
        {
            StringList aList;
            Capabilities* caps = aPlugin->getCapabilities();
            if(!caps)
            {
                return NULL;
            }

            for(int i = 0; i < caps->count(); i++)
            {
                aList.add((*caps)[i]->getName());
            }
            return createList(aList);
        }
        else
        {
            return NULL;
        }
    }
    catch_ptr_macro
}

RRStringArray* rrCallConv getPluginParameters(RRPluginHandle handle, const char* capability)
{
    try
    {
        Plugin* aPlugin = castToPlugin(handle);
        if(aPlugin)
        {
            StringList aList;
            Parameters* paras = aPlugin->getParameters(capability);
            if(!paras)
            {
                return NULL;
            }

            for(int i = 0; i < paras->count(); i++)
            {
                aList.add((*paras)[i]->getName());
            }
            return createList(aList);
        }
        else
        {
            return NULL;
        }
    }
    catch_ptr_macro
}

RRParameterHandle rrCallConv getPluginParameter(RRPluginHandle handle, const char* parameterName, const char* capabilitiesName)
{
    try
    {
        Plugin* aPlugin = castToPlugin(handle);
        BaseParameter *para = NULL;
        if(aPlugin)
        {
            if(capabilitiesName != NULL)
            {
                para = aPlugin->getParameter(parameterName, capabilitiesName);
            }
            else
            {
                para = aPlugin->getParameter(parameterName);
            }
               return para;

        }
        return NULL;

    }
    catch_ptr_macro
}

bool rrCallConv setPluginParameter(RRPluginHandle handle, const char* parameterName, const char* value)
{
    try
    {
        Plugin* aPlugin = castToPlugin(handle);
        BaseParameter* aParameter = (BaseParameter*) getPluginParameter(aPlugin, parameterName, NULL);
        return setParameter(aParameter, value);
    }
    catch_bool_macro
}

char* rrCallConv getPluginInfo(RRPluginHandle handle)
{
    try
    {
        Plugin* aPlugin = castToPlugin(handle);
           return rr::createText(aPlugin->getInfo());
    }
    catch_ptr_macro
}

char* rrCallConv getPluginStatus(RRPluginHandle handle)
{
    try
    {
        Plugin* aPlugin = castToPlugin(handle);
           return rr::createText(aPlugin->getStatus());
    }
    catch_ptr_macro
}

bool rrCallConv executePlugin(RRPluginHandle handle)
{
    return executePluginEx(handle, NULL);
}

bool rrCallConv executePluginEx(RRPluginHandle handle, void* userData)
{
    try
    {
        Plugin* aPlugin = castToPlugin(handle);
        return (aPlugin) ? aPlugin->execute(userData) : false;
    }
    catch_bool_macro
}

bool rrCallConv assignCallbacks(RRPluginHandle handle, pluginCallback cb1, pluginCallback cb2, void* userData)
{
    try
    {
        Plugin* aPlugin = castToPlugin(handle);
        return (aPlugin) ? aPlugin->assignCallbacks(cb1, cb2, userData) : false;
    }
    catch_bool_macro
}

char* rrpCallConv getPluginResult(RRPluginHandle handle)
{
    try
    {
        Plugin* aPlugin = castToPlugin(handle);
        return rr::createText(aPlugin->getResult());
    }
    catch_ptr_macro
}

bool rrCallConv setPluginInputData(RRPluginHandle handle, void* data)
{
    try
    {
        Plugin* aPlugin = castToPlugin(handle);
        return (aPlugin) ? aPlugin->setInputData(data) : false;
    }
    catch_bool_macro
}

bool rrpCallConv resetPlugin(RRPluginHandle handle)
{
    try
    {
        Plugin* aPlugin = castToPlugin(handle);
        return aPlugin->resetPlugin();
    }
    catch_bool_macro
}

bool rrpCallConv isPluginWorking(RRPluginHandle handle)
{
    try
    {
        Plugin* aPlugin = castToPlugin(handle);
        return aPlugin->isWorking();
    }
    catch_bool_macro
}

bool rrpCallConv setPluginManagerConfigurationXML(RRPluginManagerHandle handle, const char* caps)
{
    try
    {
        PluginManager *pm = castToPluginManager(handle);
        if(!caps)
        {
            return false;
        }
        pm->setConfigurationXML(caps);
        return true;
    }
    catch_bool_macro
}

char* rrpCallConv getPluginManagerConfigurationXML(RRPluginManagerHandle handle)
{
    try
    {
        PluginManager *pm = castToPluginManager(handle);
        return rr::createText(pm->getConfigurationXML());
    }
    catch_ptr_macro
}

}

