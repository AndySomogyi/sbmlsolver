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
using rr::gEmptyString;
using rr::RoadRunner;
using Poco::SharedLibrary;

class Plugin;

/**
 * @internal
 * Abstract class for plugins
 */
class PLUGINS_API_DECLSPEC PluginManager
{
    private:
        string                          mPluginFolder;
        string                          mPluginExtension;    //Different on different OS's

        std::vector< std::pair< Poco::SharedLibrary*, Plugin* > > mPlugins;

        //This is a handle to a roadRunner instance
        RoadRunner                     *mRR;
        bool                            checkImplementationLanguage(SharedLibrary* plugin);
        const char*                     getImplementationLanguage(SharedLibrary* plugin);
        Plugin*                         createCPlugin(SharedLibrary *libHandle);

    public:
                                        PluginManager(RoadRunner* aRR = NULL, const string& pluginFolder = gEmptyString, const bool& autoLoad = false);
        virtual                        ~PluginManager();
        bool                            setPluginDir(const string& dir);
        string                          getPluginDir();
        bool                            load(const string& pluginName = gEmptyString);
        bool                            loadPlugin(const string& sharedLib);
        bool                            unloadAll();
        bool                            unload(Plugin* plugin);

        int                             getNumberOfPlugins();
        int                             getNumberOfCategories();
        Plugin*                         getPlugin(const int& i);
        Plugin*                         getPlugin(const string& name);
        Plugin*                         operator[](const int& i);
        void                            setRoadRunner(RoadRunner* aRR);
        std::vector<string>             getPluginNames();
};

}
#endif
