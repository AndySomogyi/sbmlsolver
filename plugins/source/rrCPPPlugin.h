#ifndef rrCPPPluginH
#define rrCPPPluginH
#include "rrPlugin.h"
//---------------------------------------------------------------------------

namespace rrp
{
/**
 * Plugins written in C++ should inherit from this class.
 */
class PLUGINS_API_DECLSPEC CPPPlugin : public Plugin
{
    protected:

    public:
        /**
         * Create a Plugin, written in  C++.
         */
                                    CPPPlugin(  const string& name,
                                                const string& cat = gNoneString,
                                                RoadRunner* aRR = NULL,
                                                const PluginManager* pm = NULL);

        /**
         * Deallocate the Plugin.
         */
        virtual                    ~CPPPlugin();
        string                      getImplementationLanguage();
};

}

#endif
