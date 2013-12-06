#ifndef rrCPluginH
#define rrCPluginH
#include "rrPlugin.h"
//---------------------------------------------------------------------------

namespace rrp
{

typedef bool  (*executeF)(void*, bool);
typedef bool  (*destroyF)();

/**
 * The CPlugin class is containing the framework to load plugins that are written in pure C.
 * The shared library need to export an execute and destroy function.
 */
class PLUGINS_API_DECLSPEC CPlugin : public Plugin
{
    friend PluginManager;
    public:
                                //!Create a plugin written in C
                                CPlugin(const string& name, const string& cat = gNoneString,
                                            RoadRunner* aRR = NULL);

                                //!De allocate the plugin
        virtual                ~CPlugin();
        string                  getImplementationLanguage();
        virtual bool            execute(void* userData = NULL, bool useThread = false);

    protected:

        /**
          Function pointer to a execute function. The shared library need to provide this function.
            This function pointer is assigned as the plugin is loaded.
         */
        executeF                executeFunction;

        /**
          Function pointer to a destroy function. The shared library need to provide this function.
            This function pointer is assigned as the plugin is loaded.
         */
        destroyF                destroyFunction;

};

}
#endif
