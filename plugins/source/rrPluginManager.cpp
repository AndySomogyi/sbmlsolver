#pragma hdrstop
#include <sstream>
#include <string>
#include "Poco/Glob.h"
#include "Poco/SharedLibrary.h"
#include "rrPluginManager.h"
#include "rrPlugin.h"
#include "rrUtils.h"
#include "rrException.h"
#include "rrCapabilities.h"
#include "rrLogger.h"
#include "rrRoadRunner.h"
#include "rrCPlugin.h"
#include "rrStringList.h"
//---------------------------------------------------------------------------

namespace rrp
{
static bool  hasFileExtension(const string& fName);
static char* getPluginExtension();

using namespace std;
using Poco::SharedLibrary;
using Poco::Glob;

//Convenient function pointers
typedef Plugin*     (*createRRPluginFunc)(RoadRunner*, PluginManager*);
typedef char*       (*charStarFnc)();
typedef bool        (*setupCPluginFnc)(Plugin*);
typedef bool        (*destroyRRPluginFunc)(Plugin* );

bool destroyRRPlugin(Plugin *plugin);

PluginManager::PluginManager(const std::string& folder)
:
mPluginFolder(folder),
mPluginExtension(getPluginExtension())
{}

PluginManager::~PluginManager()
{
    //No matter what.. here shared libs need to be unloaded and deleted
    unloadAll();
}

bool PluginManager::setPluginDir(const string& dir)
{
    mPluginFolder = dir;
    return folderExists(mPluginFolder);
}

string PluginManager::getPluginDir()
{
    return mPluginFolder;
}

Plugin* PluginManager::getFirstPlugin()
{
    mPluginsIter = mPlugins.begin();
    if(mPluginsIter != mPlugins.end())
    {
        return (*mPluginsIter).second;
    }
    return NULL;
}

Plugin* PluginManager::getCurrentPlugin()
{
    if(mPluginsIter != mPlugins.end())
    {
        return (*mPluginsIter).second;
    }
    return NULL;
}

Plugin* PluginManager::getNextPlugin()
{
    mPluginsIter++;
    if(mPluginsIter != mPlugins.end())
    {
        return (*mPluginsIter).second;
    }
    return NULL;
}

Plugin* PluginManager::getPreviousPlugin()
{
    mPluginsIter--;
    if(mPluginsIter != mPlugins.end())
    {
        return (*mPluginsIter).second;
    }
    return NULL;
}

Plugin* PluginManager::getPlugin(const int& i)
{
    return (*this)[i];
}

Plugin* PluginManager::operator[](const int& i)
{
    if(i >= 0 && i < mPlugins.size())
    {

        return mPlugins[i].second;
    }
    else
    {
        return NULL;
    }
}

bool PluginManager::load(const string& pluginName)
{
    Log(lInfo) << "load: " << pluginName;

    bool result = true;

    //Throw if plugin folder don't exist
    if(!folderExists(mPluginFolder))
    {

        Log(lError)<<"Plugin folder: "<<mPluginFolder<<" do not exist..";        
        throw(Exception("Plugin folder don't exist"));
    }

    set<string> files;
    string globPath =  joinPath(mPluginFolder, "*." + mPluginExtension);

    if(pluginName.size())
    {
        files.insert(joinPath(mPluginFolder, pluginName + "." + mPluginExtension));
     }
    else
    {
        //Get all plugins in plugin folder
        Glob::glob(globPath, files);
    }

    set<string>::iterator it = files.begin();
    //Try to load.. failing to load a plugin do not mean we abort. Catch and report the problem
    for (; it != files.end(); ++it)
    {
        string plugin = getFileName(*it);
        Log(lInfo)<<"Loading plugin: "<<plugin;
        try
        {
                            
            bool res = loadPlugin(plugin);
            if(!res)
            {
                Log(lError)<<"There was a problem loading plugin: "<<plugin;
                result = false;
            }
        }
        catch(...)
        {
            Log(lError)<<"There was a serious problem loading plugin: "<<plugin;
            result = false;
        }
        //catch(poco exception....
    }
    return result;
}

bool PluginManager::loadPlugin(const string& _libName)
{
    stringstream msg;
    try
    {
        //Make sure the plugin is prefixxed with rrp, if not ignore
        string prefix("rrp_");
        if(_libName.substr(0, prefix.size()) != prefix)
        {
            Log(lWarning)<<"The Plugin: "<<_libName<<" lack the rrp_ prefix. Can't be loaded";    
            return false;
        }
        string libName(_libName);
        if(!hasFileExtension(libName))
        {
            libName = libName + "." + getPluginExtension();
        }

        //Check if Plugin is already loaded first
        if(getPlugin(libName))
        {
            Log(lWarning)<<"The Plugin: "<<libName<<" is already loaded";    
            return true;
        }

        SharedLibrary *libHandle = new SharedLibrary;
        libHandle->load(joinPath(mPluginFolder, libName));

        //Validate the plugin
        if(!checkImplementationLanguage(libHandle))
        {
            return false;
        }

        //Check plugin language
        const char* language = getImplementationLanguage(libHandle);

        if(strcmp(language, "C") == 0)
        {
            //Gather enough library data in order to create a CPlugin object
            //We need at least name, category and an execute function in order to setup a C plugin
            Plugin* aPlugin = createCPlugin(libHandle);
            if(!aPlugin)
            {
                return false;
            }
            aPlugin->setLibraryName(getFileNameNoExtension(libName));

            rrPlugin storeMe(libHandle, aPlugin);
            mPlugins.push_back( storeMe );
            return true;
        }
        else if(libHandle->hasSymbol("createPlugin"))
        {
            createRRPluginFunc create = (createRRPluginFunc) libHandle->getSymbol("createPlugin");

            //This plugin
            Plugin* aPlugin = create(NULL, this);
            if(aPlugin)
            {
                aPlugin->setLibraryName(getFileNameNoExtension(libName));

                rrPlugin storeMe(libHandle, aPlugin);
                mPlugins.push_back( storeMe );
            }
            return true;
        }
        else
        {
            stringstream msg;
            msg<<"The plugin library: "<<libName<<" do not have enough data in order to create a plugin. Can't load";
            Log(lWarning)<<msg.str();
            return false;
        }
    }
    catch(const Exception& e)
    {
        msg<<"RoadRunner exception: "<<e.what()<<endl;
        Log(lError)<<msg.str();
        return false;
    }
    catch(const Poco::Exception& ex)
    {
        msg<<"Poco exception: "<<ex.displayText()<<endl;
        Log(lError)<<msg.str();
        return false;
    }
    catch(...)
    {
        return false;
    }
}

//Todo: Clean up the unload process..
bool PluginManager::unloadAll()
{
    bool result(true);
    int nrPlugins = getNumberOfPlugins();
    for(int i = 0; i < nrPlugins; i++)
    {
        rrPlugin* aPluginLib = &(mPlugins[i]);
        if(aPluginLib)
        {
            SharedLibrary *pluginLibHandle    = aPluginLib->first;
            Plugin        *aPlugin            = aPluginLib->second;

            destroyRRPlugin(aPlugin);

            //Then unload
            if(pluginLibHandle)
            {
                pluginLibHandle->unload();
                delete pluginLibHandle;
            }
            //And remove from container
            aPluginLib->first = NULL;
            aPluginLib->second = NULL;
            aPluginLib = NULL;
        }
    }

    //Remove all from container...
    mPlugins.clear();
    return result;
}

bool PluginManager::unload(Plugin* plugin)
{
    if(!plugin)
    {
        return unloadAll();
    }

    bool result(false);
    int nrPlugins = getNumberOfPlugins();

    for(vector< rrPlugin >::iterator it = mPlugins.begin(); it != mPlugins.end(); it++)
    {
        rrPlugin *aPluginLib = &(*it);
        if(aPluginLib)
        {
            SharedLibrary *pluginLibHandle    = aPluginLib->first;
            Plugin        *aPlugin            = aPluginLib->second;

            if(aPlugin == plugin)
            {
                destroyRRPlugin(aPlugin);

                plugin = NULL;
                //Then unload
                if(pluginLibHandle)
                {
                    pluginLibHandle->unload();
                }

                //And remove from container
                aPluginLib->first = NULL;
                aPluginLib->second = NULL;
                aPluginLib = NULL;

                //Reset the plugins iterator to a valid one
                mPluginsIter = mPlugins.erase(it);
                result = true;
                break;
            }
        }
    }
    return result;
}

bool PluginManager::checkImplementationLanguage(Poco::SharedLibrary* plugin)
{
    //Check that the plugin has a getImplementationLanguage function
    try
    {
        plugin->getSymbol("getImplementationLanguage");
        return true;
    }
    catch(const Poco::Exception& ex)
    {
        stringstream msg;
        msg<<"Poco exception: "<<ex.displayText()<<endl;
        Log(lError)<<msg.str();
        return false;
    }
}

const char* PluginManager::getImplementationLanguage(Poco::SharedLibrary* plugin)
{
    //Check that the plugin has a getImplementationLanguage function
    try
    {
        charStarFnc func =     (charStarFnc) plugin->getSymbol("getImplementationLanguage");
        return func();
    }
    catch(const Poco::Exception& ex)
    {
        stringstream msg;
        msg<<"Poco exception: "<<ex.displayText()<<endl;
        Log(lError)<<msg.str();
        return NULL;
    }
}

StringList PluginManager::getPluginNames()
{
    StringList names;
    int nrPlugins = getNumberOfPlugins();
    for(int i = 0; i < nrPlugins; i++)
    {
        pair< Poco::SharedLibrary*, Plugin* >  *aPluginLib = &(mPlugins[i]);
        if(aPluginLib)
        {
            Plugin* aPlugin     = aPluginLib->second;
            if(aPlugin)
            {
                names.add(aPlugin->getName());
            }
        }
    }
    return names;
}

StringList PluginManager::getPluginLibraryNames()
{
    StringList names;
    int nrPlugins = getNumberOfPlugins();
    for(int i = 0; i < nrPlugins; i++)
    {
        pair< Poco::SharedLibrary*, Plugin* >  *aPluginLib = &(mPlugins[i]);
        if(aPluginLib)
        {
            Plugin* aPlugin     = aPluginLib->second;
            if(aPlugin)
            {
                names.add(aPlugin->getLibraryName());
            }
        }
    }
    return names;
}

int PluginManager::getNumberOfPlugins()
{
    return mPlugins.size();
}

int PluginManager::getNumberOfCategories()
{
    return -1;
}

Plugin* PluginManager::getPlugin(const string& name)
{
    for(int i = 0; i < getNumberOfPlugins(); i++)
    {
        rrPlugin aPluginLib = mPlugins[i];
        if(aPluginLib.first && aPluginLib.second)
        {
            Plugin* aPlugin = (Plugin*) aPluginLib.second;
            if(aPlugin && aPlugin->getName() == name)
            {
                   return aPlugin;
            }
            if(aPlugin && aPlugin->getLibraryName() == name)
            {
                   return aPlugin;
            }
        }
    }
    return NULL;
}

Plugin* PluginManager::createCPlugin(SharedLibrary *libHandle)
{
    try
    {
        //Minimum bare bone plugin need these
        charStarFnc         getName             = (charStarFnc)        libHandle->getSymbol("getName");
        charStarFnc         getCategory         = (charStarFnc)        libHandle->getSymbol("getCategory");

        char* name  = getName();
        char* cat   = getCategory();
        CPlugin* aPlugin = new CPlugin(name, cat);

        aPlugin->executeFunction = (executeF)         libHandle->getSymbol("executePlugin");
        aPlugin->destroyFunction = (destroyF)         libHandle->getSymbol("destroyPlugin");

        setupCPluginFnc     setupCPlugin        = (setupCPluginFnc)    libHandle->getSymbol("setupCPlugin");

        //This give the C plugin an opaque Handle to the CPlugin object
        setupCPlugin(aPlugin);
        return aPlugin;
    }
    catch(const Poco::NotFoundException& ex)
    {
        Log(lError)<<"Error in createCPlugin: " <<ex.message();
        return NULL;
    }
    return NULL;
}


// Plugin cleanup function
bool destroyRRPlugin(Plugin *plugin)
{
    //we allocated in the create function with new, delete the passed object
    try
    {
        delete plugin;
        return true;
    }
    catch(...)
    {
        //Bad stuff!
        clog<<"Failed deleting RoadRunner plugin..";
        return false;
    }
}


char* getPluginExtension()
{

#if defined(_WIN32) || defined(__WIN32__)
    return "dll";
#elif defined(UNIX)
    return "a";
#else
    // OSX
    return "dylib";
#endif
}

bool hasFileExtension(const string& fName)
{
    return (fName.find_last_of(".") != string::npos) ? true : false;
}

}
