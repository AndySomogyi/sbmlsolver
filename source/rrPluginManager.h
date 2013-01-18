#ifndef rrPluginManagerH
#define rrPluginManagerH
#include <map>
#include <vector>

#include "rrObject.h"
//---------------------------------------------------------------------------
/* A minimalistic Plugin manager. */

using std::map;
namespace rr
{

//Abstract class for plugins
class Plugin;

class RR_DECLSPEC PluginManager : public rrObject
{
	private:
        string			   			mPluginFolder;
        vector< pair< Poco::SharedLibrary*, Plugin* > >
        					 		mPlugins;

    public:
	    				           	PluginManager(const std::string& pluginFolder = EmptyString, const bool& autoLoad = false);
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

};
}


#endif
