#ifndef rrCPluginH
#define rrCPluginH
#include "rrPlugin.h"
//---------------------------------------------------------------------------

namespace rrp
{

typedef bool  (*executeF)(void*, bool);
typedef bool  (*destroyF)();

/**
 * @internal
 */
class PLUGINS_API_DECLSPEC CPlugin : public Plugin
{
    friend PluginManager;

    protected:
        executeF                executeFunction;
        destroyF                 destroyFunction;

    public:
                                CPlugin(const string& name, const string& cat = gNoneString,
                                            RoadRunner* aRR = NULL);

        virtual                ~CPlugin();
        string                  getImplementationLanguage();
        virtual bool            execute(void* userData = NULL, bool useThread = false);

};

}
#endif
