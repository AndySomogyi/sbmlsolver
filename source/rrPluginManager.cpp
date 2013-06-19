//---------------------------------------------------------------------------
#pragma hdrstop
#include <sstream>
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

namespace rr
{

using namespace std;
using Poco::SharedLibrary;
using Poco::Glob;

bool destroyRRPlugin(Plugin *plugin);

PluginManager::PluginManager(const std::string& folder, const bool& autoLoad, RoadRunner* aRR)
:
mPluginFolder(folder),
mRR(aRR)
{
#if defined(WIN32)
mPluginExtension = "dll";
#elif defined(UNIX)
mPluginExtension = "a";
#else
mPluginExtension = "b";
#endif

    if(autoLoad)
    {
        load();
    }
}

PluginManager::~PluginManager()
{}

void PluginManager::setRoadRunnerInstance(RoadRunner* aRR)
{
	mRR = aRR;
}

bool PluginManager::setPluginFolder(const string& dir)
{
	return false;
}

string PluginManager::getPluginFolder()
{
	return mPluginFolder;
}

Plugin*	PluginManager::operator[](const int& i)
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

typedef Plugin* (*createRRPluginFunc)(RoadRunner*);
typedef char* 	(*getLangFunc)();
typedef bool    (*destroyRRPluginFunc)(Plugin* );

bool PluginManager::load(const string& pluginName)
{
	bool result = true;
    //Throw if plugin folder don't exist
    if(!folderExists(mPluginFolder))
    {
        Log(lError)<<"Plugin folder: "<<mPluginFolder<<" do not exist..";
        return false;
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

    std::set<std::string>::iterator it = files.begin();

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

bool PluginManager::loadPlugin(const string& pluginName)
{
	stringstream msg;
	try
    {
        SharedLibrary *libHandle = new SharedLibrary;
        libHandle->load(joinPath(mPluginFolder, pluginName));

        //Validate the plugin
        if(!checkImplementationLanguage(libHandle))
        {
			return false;
        }

		//Check plugin language
        char* language = getImplementationLanguage(libHandle);

		if(strcmp(language, "C") == 0)
        {
        	//Gather enough library data in order to create a CPlugin object
            //We need at least name, category and an execute function in order to setup a C plugin
            Plugin* aPlugin = createCPlugin(libHandle);
            Capabilities *caps = aPlugin->getCapabilities();
            mRR->addCapabilities(*(caps));

            pair< Poco::SharedLibrary*, Plugin* > storeMe(libHandle, aPlugin);
            mPlugins.push_back( storeMe );


        }
        else if(libHandle->hasSymbol("createPlugin"))
        {
            createRRPluginFunc create = (createRRPluginFunc) libHandle->getSymbol("createPlugin");
            //This plugin
            Plugin* aPlugin = create(mRR);
            if(aPlugin)
            {
            	//Add plugins capabilities to roadrunner
                Capabilities *caps = aPlugin->getCapabilities();
                mRR->addCapabilities(*(caps));

                pair< Poco::SharedLibrary*, Plugin* > storeMe(libHandle, aPlugin);
                mPlugins.push_back( storeMe );
            }
		    return true;
        }
        else
        {
	        stringstream msg;
            msg<<"The plugin library: "<<pluginName<<" do not have enough data in order to create a plugin. Can't load";
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

bool PluginManager::unload()
{
	bool result(true);
    int nrPlugins = getNumberOfPlugins();
	for(int i = 0; i < nrPlugins; i++)
    {
    	pair< Poco::SharedLibrary*, Plugin* >  *aPluginLib = &(mPlugins[i]);
        if(aPluginLib)
        {
            SharedLibrary *pluginLibHandle	= aPluginLib->first;
            Plugin*		   aPlugin 			= aPluginLib->second;

            destroyRRPlugin(aPlugin);

            //Then unload
			if(pluginLibHandle)
			{
				pluginLibHandle->unload();
			}
            //And remove from container
            aPluginLib->first = NULL;
            aPluginLib->second = NULL;
        }
    }

    //Remove all from container...
    mPlugins.clear();
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

char* PluginManager::getImplementationLanguage(Poco::SharedLibrary* plugin)
{
	//Check that the plugin has a getImplementationLanguage function
    try
    {
	    getLangFunc func = 	(getLangFunc) plugin->getSymbol("getImplementationLanguage");
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
            Plugin*		   aPlugin 	= aPluginLib->second;

            //Then unload
            names.add(aPlugin->getName());
        }
    }
    return names;
}

int	PluginManager::getNumberOfPlugins()
{
	return mPlugins.size();
}

int PluginManager::getNumberOfCategories()
{
	return -1;
}

Plugin*	PluginManager::getPlugin(const int& i)
{
	return (*this)[i];
}

Plugin*	PluginManager::getPlugin(const string& name)
{
	for(int i = 0; i < getNumberOfPlugins(); i++)
    {
    	pair< Poco::SharedLibrary*, Plugin* >  aPluginLib = mPlugins[i];
        if(aPluginLib.first && aPluginLib.second)
        {
			Plugin* aPlugin = (Plugin*) aPluginLib.second;
            if(aPlugin && aPlugin->getName() == name)
            {
               	return aPlugin;
            }
        }
    }
    return NULL;
}

typedef char* 		(rrCallConv *charStar)();
typedef void* 		(rrCallConv *CPluginData)(RoadRunner*);
typedef bool 		(rrCallConv *exec)(void*);

Plugin* PluginManager::createCPlugin(SharedLibrary *libHandle)
{
	try
    {
        //Minimum bare bone plugin need these
    	charStar 				getName 			= (charStar) 		libHandle->getSymbol("getName");
        charStar 				getCategory 		= (charStar) 		libHandle->getSymbol("getCategory");
		CPluginData		  		createCPluginData	= (CPluginData) 	libHandle->getSymbol("createCPluginData");
		exec			  		executeFunc			= (exec) 			libHandle->getSymbol("execute");
        char* name 	= getName();
        char* cat 	= getCategory();
        void* cDataHandle = createCPluginData(mRR);
        CPlugin* aPlugin = new CPlugin(name, cat);
        aPlugin->assignExecuteFunction(executeFunc);

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
bool destroyRRPlugin(rr::Plugin *plugin)
{
	//we allocated in the factory with new, delete the passed object
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

}

