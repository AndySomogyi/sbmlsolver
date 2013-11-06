#ifndef rrCPPPluginH
#define rrCPPPluginH
#include "rrPlugin.h"
//---------------------------------------------------------------------------

namespace rrp
{

/**
 * @internal
 */
class PLUGINS_API_DECLSPEC CPPPlugin : public Plugin
{
    private:

    protected:

    public:
                            CPPPlugin(  const std::string& name,
                                        const std::string& cat = gNoneString,
                                        RoadRunner* aRR = NULL,
                                        PluginCallBackFnc fn1 = NULL,
                                        PluginCallBackFnc fn2 = NULL,
                                        PluginCallBackFnc fn3 = NULL);

        virtual            ~CPPPlugin();
        string              getImplementationLanguage();
};

}

#endif
