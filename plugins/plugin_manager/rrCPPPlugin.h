#ifndef rrCPPPluginH
#define rrCPPPluginH
#include "rrPlugin.h"
//---------------------------------------------------------------------------

namespace rr
{

/**
 * @internal
 */
class RR_DECLSPEC CPPPlugin : public Plugin
{
    private:

    protected:

    public:
                            CPPPlugin(    const std::string& name,
                                        const std::string& cat = gNoneString,
                                        RoadRunner* aRR = NULL,
                                        PluginWorkStartedCB fn1 = NULL,
                                        PluginWorkFinishedCB fn2 = NULL);

        virtual            ~CPPPlugin();
        string                 getImplementationLanguage();
};

}

#endif
