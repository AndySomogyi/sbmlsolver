#ifndef rrPluginManagerH
#define rrPluginManagerH
#include <string>
#include "rrExporter.h"
#include "rrIniFile.h"
//---------------------------------------------------------------------------
/* A minimalistic Plugin manager. */

namespace rr
{

//Abstract class for plugins
class Plugin;
class IniSection;
class IniKey;

class RR_DECLSPEC PluginManager : public rrObject
{
	private:
		IniFile			    mIniFile;	//Read/Write plugin manager info, e.g. where to find plugins..
        IniSection		   *mSection;	//Section in the ini file where plugin manager info is stored
        IniKey			   *mPluginFolder;
        vector<Plugin*>		mPlugins;

    public:
	    					PluginManager(const std::string& fName = EmptyString);
        				   ~PluginManager();
		bool				SetPluginFolder(const string& dir);
		string				GetPluginFolder();
		int 				Load();
		int 				Unload();
        int					GetNumberOfPlugins();
		int                 GetNumberOfCategories();
        Plugin*				GetPlugin(const int& i);
        Plugin*				GetPlugin(const string& name);

};
}


#endif
