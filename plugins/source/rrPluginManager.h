#ifndef rrPluginManagerH
#define rrPluginManagerH
#include <vector>
#include <string>
#include "rrp_exporter.h"
#include "rrConstants.h"
#include "rrStringList.h"
#include "Poco/SharedLibrary.h"
#include "Configurable.h"
//---------------------------------------------------------------------------
/* A minimalistic Plugin manager. */

namespace rr
{
class RoadRunner;
}

namespace rrp
{

using std::string;
using rr::Configurable;
using rr::gEmptyString;
using rr::RoadRunner;
using Poco::SharedLibrary;

class Plugin;


/**
 * @internal
 * Abstract class for plugins
 */
class RRP_DECLSPEC PluginManager : public Configurable
{
    private:
        std::string                     mPluginFolder;
        std::string                     mPluginExtension;    //Different on different OS's

        std::vector< std::pair< Poco::SharedLibrary*, Plugin* > >
                                        mPlugins;

        //This is a handle to a roadRunner instance
        RoadRunner                     *mRR;
        bool                            checkImplementationLanguage(SharedLibrary* plugin);
        const char*                     getImplementationLanguage(SharedLibrary* plugin);
        Plugin*                         createCPlugin(SharedLibrary *libHandle);

    public:
                                        PluginManager(RoadRunner* aRR = NULL, const string& pluginFolder = gEmptyString, const bool& autoLoad = false);
        virtual                        ~PluginManager();
        bool                            setPluginDir(const std::string& dir);
        string                          getPluginDir();
        bool                            load(const std::string& pluginName = gEmptyString);
        bool                            loadPlugin(const std::string& sharedLib);
        bool                            unloadAll();
        bool                            unload(Plugin* plugin);

        int                             getNumberOfPlugins();
        int                             getNumberOfCategories();
        Plugin*                         getPlugin(const int& i);
        Plugin*                         getPlugin(const std::string& name);
        Plugin*                         operator[](const int& i);
        void                            setRoadRunner(RoadRunner* aRR);
        std::vector<std::string>        getPluginNames();

        virtual _xmlNode*               createConfigNode();
        virtual void                    loadConfig(const _xmlDoc* doc);
        std::string                     getConfigurationXML();
        void                            setConfigurationXML(const std::string& xml);
};

}
#endif
