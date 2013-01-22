//---------------------------------------------------------------------------
#pragma hdrstop
#include "Poco/SharedLibrary.h"
#include "rrPluginManager.h"
#include "rrPlugin.h"
#include "rrUtils.h"
#include "rrException.h"

using Poco::SharedLibrary;

namespace rr
{

PluginManager::PluginManager(const std::string& folder, const bool& autoLoad, RoadRunner* aRR)
:
mPluginFolder(folder),
mRR(aRR)
{
    if(autoLoad)
    {
        Load();
    }
}

PluginManager::~PluginManager()
{}

void PluginManager::SetRoadRunnerInstance(RoadRunner* aRR)
{
	mRR = aRR;
}

bool PluginManager::SetPluginFolder(const string& dir)
{
	return false;
}

string PluginManager::GetPluginFolder()
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

bool PluginManager::Load()
{
	//Throw if plugin folder don't exist
    try
    {
    	if(!FolderExists(mPluginFolder))
        {
        	throw Exception("Plugin folder do not exists");
        }
        //Look for shared libraries in this folder

        //Load and create the plugins
        SharedLibrary *aLib = new SharedLibrary;
        aLib->load(JoinPath(mPluginFolder, "TestPlugin.dll"));

        //Validate the plugin
        if(aLib->hasSymbol("createRRPlugin"))
        {
            createRRPluginFunc create = (createRRPluginFunc) aLib->getSymbol("createRRPlugin");
        	//This plugin
            Plugin* aPlugin = create(mRR);
            if(aPlugin)
            {
            	pair< Poco::SharedLibrary*, Plugin* > store(aLib, aPlugin);
            	mPlugins.push_back( store );
            }
        }
        else
        {
        	//Log some warnings about a bad plugin...
        }
        return true;
    }
    catch(const Exception& e)
    {
    	stringstream msg;
    	msg<<"RoadRunner exception: "<<e.what()<<endl;
		Exception ex(msg.str());
    	throw ex;
    }
    catch(Poco::LibraryLoadException& ex)
    {
    	Exception test("Test");
    	throw ex;
    }

}

bool PluginManager::Unload()
{
	bool result(true);
	for(int i = 0; i < GetNumberOfPlugins(); i++)
    {
    	pair< Poco::SharedLibrary*, Plugin* >  aPlugin = mPlugins[i];
        if(aPlugin.first)
        {
            SharedLibrary *aLib = aPlugin.first;

        	//Destroy plugin
            if(aLib->hasSymbol("destroyRRPlugin"))
        	{
            	destroyRRPluginFunc destroy = (destroyRRPluginFunc) aLib->getSymbol("destroyRRPlugin");

            	result = destroy(aPlugin.second);

            	//Then unload
        		aPlugin.first->unload();
        	}
        }
    }
    return result;
}

int	PluginManager::GetNumberOfPlugins()
{
	return mPlugins.size();
}

int PluginManager::GetNumberOfCategories()
{
	return -1;
}

Plugin*	PluginManager::GetPlugin(const int& i)
{
	return (*this)[i];
}

Plugin*	PluginManager::GetPlugin(const string& name)
{
	for(int i = 0; i < GetNumberOfPlugins(); i++)
    {
    	pair< Poco::SharedLibrary*, Plugin* >  aPluginLib = mPlugins[i];
        if(aPluginLib.first && aPluginLib.second)
        {
			Plugin* aPlugin = (Plugin*) aPluginLib.second;
            if(aPlugin && aPlugin->GetName() == name)
            {
               	return aPlugin;
            }
        }
    }
    return NULL;
}


}

