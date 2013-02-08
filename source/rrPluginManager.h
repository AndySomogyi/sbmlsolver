#ifndef rrPluginManagerH
#define rrPluginManagerH
#include <vector>
#include "rrObject.h"
//---------------------------------------------------------------------------
/* A minimalistic Plugin manager. */



namespace rr
{

//Abstract class for plugins
class RoadRunner;
class Plugin;

class RR_DECLSPEC PluginManager : public rrObject
{
	private:
        string			   			mPluginFolder;
        vector< pair< Poco::SharedLibrary*, Plugin* > >
        					 		mPlugins;
        RoadRunner		   *mRR;		//This is a handle to the roadRunner instance, creating the pluginManager

    public:
	    				           	PluginManager(const std::string& pluginFolder = EmptyString, const bool& autoLoad = false, RoadRunner* aRR = NULL);
        				           ~PluginManager();
		bool			           	SetPluginFolder(const string& dir);
		string			           	GetPluginFolder();
		bool 			           	Load();
		bool 			           	Unload();
        int				           	GetNumberOfPlugins();
		int                         GetNumberOfCategories();
        Plugin*			           	GetPlugin(const int& i);
        Plugin*			           	GetPlugin(const string& name);
        Plugin*	   					operator[](const int& i);
        void						SetRoadRunnerInstance(RoadRunner* aRR);

};
}


#endif
