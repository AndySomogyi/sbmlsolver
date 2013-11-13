#ifndef rrCPluginH
#define rrCPluginH
#include "rrPlugin.h"
//---------------------------------------------------------------------------

namespace rrp
{

typedef bool  (*executeFnc)(void*, bool);

/**
 * @internal
 */
class PLUGINS_API_DECLSPEC CPlugin : public Plugin
{
    private:

    protected:
        executeFnc              executeFunction;

    public:
                                CPlugin(const std::string& name,
                                    const std::string& cat = gNoneString,
                                    RoadRunner* aRR = NULL);

        virtual                ~CPlugin();
        string                  getImplementationLanguage();
        virtual bool            execute(void* userData = NULL, bool useThread = false);

        void                    assignExecuteFunction(executeFnc func);

        virtual _xmlNode*       createConfigNode();
        virtual void            loadConfig(const _xmlDoc* doc);
};

}
#endif
