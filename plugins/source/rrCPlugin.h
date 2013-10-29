#ifndef rrCPluginH
#define rrCPluginH
#include "rrPlugin.h"
//---------------------------------------------------------------------------

namespace rrp
{

typedef bool  (rrCallConv *executeFunc)(void*);

/**
 * @internal
 */
class PLUGINS_API_DECLSPEC CPlugin : public Plugin
{
    private:

    protected:
        executeFunc            executeFunction;

    public:
                                CPlugin(const std::string& name,
                                    const std::string& cat = gNoneString,
                                    RoadRunner* aRR = NULL,
                                    PluginWorkStartedCB fn1 = NULL,
                                    PluginWorkFinishedCB fn2 = NULL);

        virtual                ~CPlugin();
        string                  getImplementationLanguage();
        virtual bool            execute(void* userData = NULL);
        void                    assignExecuteFunction(executeFunc func);

        virtual _xmlNode*       createConfigNode();
        virtual void            loadConfig(const _xmlDoc* doc);

};

}

#endif
