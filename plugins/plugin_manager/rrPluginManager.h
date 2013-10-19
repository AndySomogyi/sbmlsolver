#ifndef rrPluginManagerH
#define rrPluginManagerH
#include <vector>
#include "rrExporter.h"
#include "rrStringList.h"
#include "Poco/SharedLibrary.h"
#include "Configurable.h"
//---------------------------------------------------------------------------
/* A minimalistic Plugin manager. */

namespace rr
{


class RoadRunner;
class Plugin;
using Poco::SharedLibrary;

/**
 * @internal
 * Abstract class for plugins
 */
class RR_DECLSPEC PluginManager : public Configurable
{
private:
    std::string                           mPluginFolder;
    std::string                           mPluginExtension;    //Different on different OS's

    std::vector< std::pair< Poco::SharedLibrary*, Plugin* > >    mPlugins;

    RoadRunner                       *mRR;        //This is a handle to the roadRunner instance, creating the pluginManager
    bool                        checkImplementationLanguage(SharedLibrary* plugin);
    const char*                 getImplementationLanguage(SharedLibrary* plugin);
    Plugin*                     createCPlugin(SharedLibrary *libHandle);

public:
    PluginManager(const std::string& pluginFolder = "", const bool& autoLoad = false, RoadRunner* aRR = NULL);
    virtual                           ~PluginManager();
    bool                           setPluginDir(const std::string& dir);
    string                           getPluginDir();
    bool                            load(const std::string& pluginName = "");
    bool                            loadPlugin(const std::string& sharedLib);
    bool                            unload();
    int                               getNumberOfPlugins();
    int                         getNumberOfCategories();
    Plugin*                           getPlugin(const int& i);
    Plugin*                           getPlugin(const std::string& name);
    Plugin*                           operator[](const int& i);
    void                        setRoadRunner(RoadRunner* aRR);
    std::vector<std::string>          getPluginNames();

    virtual _xmlNode *createConfigNode();
    virtual void loadConfig(const _xmlDoc* doc);

    std::string getConfigurationXML();

    void setConfigurationXML(const std::string& xml);


};

}
#endif
