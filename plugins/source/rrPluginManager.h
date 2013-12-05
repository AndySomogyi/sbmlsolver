#ifndef rrPluginManagerH
#define rrPluginManagerH
#include <vector>
#include <string>
#include "rrPluginsAPIExporter.h"
#include "rrConstants.h"
#include "rrStringList.h"
#include "Poco/SharedLibrary.h"
//---------------------------------------------------------------------------
/* A minimalistic Plugin manager. */

namespace rr
{
class RoadRunner;
}

namespace rrp
{

using std::string;
using std::vector;
using std::pair;
using rr::gEmptyString;
using rr::RoadRunner;
using Poco::SharedLibrary;

class Plugin;

/**
 * @internal
 * Abstract class for plugins
 */

typedef pair< Poco::SharedLibrary*, Plugin* > rrPlugin;

class PLUGINS_API_DECLSPEC PluginManager
{
    private:
        string                          mPluginFolder;
        string                          mPluginExtension;    //Different on different OS's
        vector< rrPlugin >              mPlugins;
        vector< rrPlugin >::iterator    mPluginsIter;

        bool                            checkImplementationLanguage(SharedLibrary* plugin);
        const char*                     getImplementationLanguage(SharedLibrary* plugin);
        Plugin*                         createCPlugin(SharedLibrary *libHandle);
        Plugin*                         getPlugin(const int& i);
        Plugin*                         operator[](const int& i);

    public:
                                        PluginManager(const string& pluginFolder = gEmptyString, const bool& autoLoad = false);
        virtual                        ~PluginManager();
        bool                            setPluginDir(const string& dir);
        string                          getPluginDir();
        bool                            load(const string& pluginName = gEmptyString);
        bool                            loadPlugin(const string& sharedLib);
        bool                            unloadAll();
        bool                            unload(Plugin* plugin);

        int                             getNumberOfPlugins();
        int                             getNumberOfCategories();

        Plugin*                         getFirstPlugin();
        Plugin*                         getNextPlugin();
        Plugin*                         getPreviousPlugin();
        Plugin*                         getCurrentPlugin();
        Plugin*                         getPlugin(const string& name);


        std::vector<string>             getPluginNames();
};

}
#endif
