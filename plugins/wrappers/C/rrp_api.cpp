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
            Log(Logger::LOG_ERROR) << "the handle " << pm << ", " << handle << " is NOT a valid PluginManager";
        }
    }
    else
    {
        Log(Logger::LOG_ERROR) << "the handle is NULL";
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

RRPluginManagerHandle rrp_cc createPluginManager(RRHandle rrHandle)
{
    try
    {
        RoadRunner *rr = static_cast<RoadRunner*>(rrHandle);

        Log(lDebug) << __FUNC__ << "RoadRunner: " << rr;

        std::string pluginDir = joinPath(
                getParentFolder(gDefaultSupportCodeFolder), "plugins");

        PluginManager* pm = new PluginManager(rr, pluginDir, false);
        pluginManagers.insert(pm);

        Log(lDebug) << __FUNC__ << " created plugin manager: " << pm;
        return pm;
    }
    catch_ptr_macro
}

/**
 * create an instance of a plugin managager attached to the given RoadRunner instance.
 */
RRPluginManagerHandle rrp_cc createPluginManagerEx(const char* pluginDir, bool autoLoad, RRHandle rrHandle)
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
bool rrp_cc freePluginManager(RRPluginManagerHandle handle)
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
RRPluginHandle rrp_cc loadPlugin(RRPluginManagerHandle handle, const char* pluginName)
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

bool rrp_cc loadPlugins(RRPluginManagerHandle handle)
{
    try
    {
        PluginManager *pm = castToPluginManager(handle);
        return pm->load();
    }
    catch_bool_macro
}

bool rrp_cc unLoadPlugins(RRPluginManagerHandle handle)
{
    try
    {
        PluginManager *pm = castToPluginManager(handle);
        return pm->unloadAll();
    }
    catch_bool_macro
}

bool rrp_cc unLoadPlugin(RRPluginManagerHandle handle, RRPluginHandle plugin)
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

int rrp_cc getNumberOfPlugins(RRPluginManagerHandle handle)
{
    try
    {
        Log(Logger::LOG_NOTICE) << __FUNC__;
        PluginManager *pm = castToPluginManager(handle);
        return pm->getNumberOfPlugins();
    }
    catch_int_macro
}

RRPluginHandle rrp_cc getPlugin(RRPluginManagerHandle handle, const char* pluginName)
{
    try
    {
        PluginManager *pm = castToPluginManager(handle);
        Plugin* aPlugin = pm->getPlugin(pluginName);
        return aPlugin;
    }
    catch_ptr_macro
}

RRPluginHandle rrp_cc getPluginByID(RRPluginManagerHandle handle, int id)
{
    try
    {
        PluginManager *pm = castToPluginManager(handle);
        Plugin* aPlugin = pm->getPlugin(id);
        return aPlugin;
    }
    catch_ptr_macro
}

RRStringArray* rrp_cc getPluginNames(RRPluginManagerHandle handle)
{
    try
    {
        PluginManager *pm = castToPluginManager(handle);
        StringList names = pm->getPluginNames();
        return createList(names);
    }
    catch_ptr_macro
}

char* rrp_cc getPluginName(RRPluginHandle handle)
{
    try
    {
        Plugin* aPlugin = castToPlugin(handle);
        return rr::createText(aPlugin->getName());
    }
    catch_ptr_macro
}

RRStringArray* rrp_cc getPluginCapabilities(RRPluginHandle handle)
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

RRStringArray* rrp_cc getPluginParameters(RRPluginHandle handle, const char* capability)
{
    try
    {
        Plugin* aPlugin = castToPlugin(handle);
        if(aPlugin)
        {
            string cap;
            if(capability)
            {
                cap = capability;
            }
            else
            {
               cap = ("");
            }
            Parameters* paras = aPlugin->getParameters(cap);
            if(!paras)
            {
                return NULL;
            }

            StringList aList;
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

RRParameterHandle rrp_cc getPluginParameter(RRPluginHandle handle, const char* parameterName, const char* capabilitiesName)
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

bool rrp_cc setPluginParameter(RRPluginHandle handle, const char* parameterName, const char* value)
{
    try
    {
        Plugin* aPlugin = castToPlugin(handle);
        BaseParameter* aParameter = (BaseParameter*) getPluginParameter(aPlugin, parameterName, NULL);
        return setParameter(aParameter, value);
    }
    catch_bool_macro
}

char* rrp_cc getPluginInfo(RRPluginHandle handle)
{
    try
    {
        Plugin* aPlugin = castToPlugin(handle);
           return rr::createText(aPlugin->getInfo());
    }
    catch_ptr_macro
}

char* rrp_cc getPluginStatus(RRPluginHandle handle)
{
    try
    {
        Plugin* aPlugin = castToPlugin(handle);
           return rr::createText(aPlugin->getStatus());
    }
    catch_ptr_macro
}

bool rrp_cc executePlugin(RRPluginHandle handle)
{
    return executePluginEx(handle, NULL, false);
}

bool rrp_cc executePluginEx(RRPluginHandle handle, void* userData, bool inAThread)
{
    try
    {
        Plugin* aPlugin = castToPlugin(handle);
        return (aPlugin) ? aPlugin->execute(userData, inAThread) : false;
    }
    catch_bool_macro
}

bool rrp_cc assignCallbacks(RRPluginHandle handle, pluginCallback startCB, pluginCallback progressCB, pluginCallback finishedCB, void* userData)
{
    try
    {
        Plugin* aPlugin = castToPlugin(handle);
        return (aPlugin) ? aPlugin->assignCallbacks(startCB, progressCB, finishedCB, userData) : false;
    }
    catch_bool_macro
}

char* rrp_cc getPluginResult(RRPluginHandle handle)
{
    try
    {
        Plugin* aPlugin = castToPlugin(handle);
        return rr::createText(aPlugin->getResult());
    }
    catch_ptr_macro
}

bool rrp_cc setPluginInputData(RRPluginHandle handle, void* data)
{
    try
    {
        Plugin* aPlugin = castToPlugin(handle);
        return (aPlugin) ? aPlugin->setInputData(data) : false;
    }
    catch_bool_macro
}

bool rrp_cc resetPlugin(RRPluginHandle handle)
{
    try
    {
        Plugin* aPlugin = castToPlugin(handle);
        return aPlugin->resetPlugin();
    }
    catch_bool_macro
}

bool rrp_cc isPluginWorking(RRPluginHandle handle)
{
    try
    {
        Plugin* aPlugin = castToPlugin(handle);
        return aPlugin->isWorking();
    }
    catch_bool_macro
}

bool rrp_cc setPluginManagerConfigurationXML(RRPluginManagerHandle handle, const char* caps)
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

char* rrp_cc getPluginManagerConfigurationXML(RRPluginManagerHandle handle)
{
    try
    {
        PluginManager *pm = castToPluginManager(handle);
        return rr::createText(pm->getConfigurationXML());
    }
    catch_ptr_macro
}

}

