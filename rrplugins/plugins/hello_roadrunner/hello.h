#ifndef helloH
#define helloH
#include "telProperty.h"
#include "rrplugins/pluginBaseClass/telCPPPlugin.h"
#include "telTelluriumData.h"
#include "rrplugins/core/tel_api.h"

namespace hello
{
    using namespace tlp;

    class Hello : public CPPPlugin
    {
    private:
        char* mVersion;

    public:
        Hello();
        bool                execute(bool inThread = false);
        string              message;
    };

#ifdef EXPORT_HELLO
    extern "C"
    {
        Plugin* plugins_cc createPlugin();
        const char* plugins_cc getImplementationLanguage();
        void         plugins_cc setHostInterface(rrc::THostInterface* _hostInterface);
    }
#endif
}

#endif //helloH

