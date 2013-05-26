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
typedef bool    (*destroyRRPluginFunc)(Plugin* );

bool PluginManager::load()
{
	bool result = true;
    //Throw if plugin folder don't exist
    if(!folderExists(mPluginFolder))
    {
        Log(lError)<<"Plugin folder: "<<mPluginFolder<<" do not exist..";
        return false;
    }

 	//Get all plugins in plugin folder
    std::set<std::string> files;
    string globPath =  joinPath(mPluginFolder, "*." + mPluginExtension);
    Glob::glob(globPath, files);
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
            	//Find out what was wrong..?
				Log(lError)<<"There was a slight problem loading plugin: "<<plugin;
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

bool PluginManager::loadPlugin(const string& sharedLib)
{
	try
    {
        SharedLibrary *aLib = new SharedLibrary;
        aLib->load(joinPath(mPluginFolder, sharedLib));

        //Validate the plugin
        if(aLib->hasSymbol("createPlugin"))
        {
            createRRPluginFunc create = (createRRPluginFunc) aLib->getSymbol("createPlugin");
            //This plugin
            Plugin* aPlugin = create(mRR);
            if(aPlugin)
            {
            	//Add plugins capabilities to roadrunner
                Capabilities *caps = aPlugin->getCapabilities();

                pair< Poco::SharedLibrary*, Plugin* > storeMe(aLib, aPlugin);
                mPlugins.push_back( storeMe );

                mRR->addCapabilities(*(caps));
            }
        }
        else
        {
	        stringstream msg;
            msg<<"The plugin library: "<<sharedLib<<" do not have a createPlugin function. Can't load";
            Log(lError)<<msg.str();
            return false;
        }
        return true;
    }
    catch(const Exception& e)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<e.what()<<endl;
		Log(lError)<<msg.str();
		return false;
    }
    catch(const Poco::Exception& ex)
    {
		stringstream msg;
    	msg<<"Poco exception: "<<ex.displayText()<<endl;
   		Log(lError)<<msg.str();
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
            SharedLibrary *aLib 	= aPluginLib->first;
            Plugin*		   aPlugin 	= aPluginLib->second;

            destroyRRPlugin(aPlugin);

            //Then unload
			if(aLib)
			{
				aLib->unload();
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

