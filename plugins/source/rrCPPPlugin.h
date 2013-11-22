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
                                        const PluginManager* pm = NULL,
                                        PluginCallBackFnc fn1 = NULL,
                                        PluginCallBackFnc fn2 = NULL,
                                        PluginCallBackFnc fn3 = NULL);

        virtual                    ~CPPPlugin();
        string                      getImplementationLanguage();
        virtual _xmlNode*           createConfigNode();
        virtual void                loadConfig(const _xmlDoc* doc);

};

}

#endif
