#ifndef rrCPPPluginH
#define rrCPPPluginH
#include "rrPlugin.h"
//---------------------------------------------------------------------------

namespace rrp
{
class PLUGINS_API_DECLSPEC CPPPlugin : public Plugin
{
    protected:

    public:
                                    CPPPlugin(  const string& name,
                                                const string& cat = gNoneString,
                                                RoadRunner* aRR = NULL,
                                                const PluginManager* pm = NULL);

        virtual                    ~CPPPlugin();
        string                      getImplementationLanguage();
};

}

#endif
